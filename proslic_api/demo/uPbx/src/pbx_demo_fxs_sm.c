/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo_fxs_sm.c 6476 2017-05-03 01:08:55Z nizajerk $
**
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file implements the internals of the PBX demo action states.
**
*/
#include <time.h>

#include "si_voice_datatypes.h"

#ifdef CID_ONLY_DEMO
#include "cid_demo.h"
#include "si_cid.h"
#include "si_cid_fsm.h"
#include "si_cid_int.h"
#include "si_cid.h"
#include "si_cid_sys_srvc.h"
#include "cid_demo_proslic.h"
#endif

#include "si_voice.h"
#include "proslic.h"
#include "pbx_demo.h"
#include "pbx_demo_cfg.h"

#include FXS_CONSTANTS_HDR

#define GetTime         main->demo_state.currentChanPtr->deviceId->ctrlInterface->getTime_fptr

#define SI_CTRLIF(PCHAN) ((PCHAN)->deviceId->ctrlInterface)
#define si_diffTime(PCHAN, TMR, MS_DIFF) (SI_CTRLIF(PCHAN)->timeElapsed_fptr(SI_CTRLIF(PCHAN)->hTimer, (TMR), (int *)(MS_DIFF)))

/* Forward declarations */
static void pbx_state_cidgen(pbx_demo_t *main, unsigned int chan,
                             BOOLEAN interrupt_detected);

/****************************************************************************/
/* We setup 1 end of the call - the other is handled when we check the
 * state of the other end of the call...
 */
static void do_cross_connect(pbx_demo_t *main, unsigned int chan)
{
  int dest_chan;

  dest_chan = main->chan_info[chan].chan_connect;

  /* Set both channels to connect state */
  main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_CONNECT;

  /* Cross connect the audio timeslots */
  ProSLIC_PCMTimeSlotSetup(main->demo_state.currentChanPtr,
     PBX_TIMESLOTS(chan), PBX_TIMESLOTS(dest_chan));


  /* Make sure we're unmuted */
  ProSLIC_SetMuteStatus(main->demo_state.currentChanPtr, PROSLIC_MUTE_NONE);

  /* Make sure we have PCM audio */
  ProSLIC_PCMStart(main->demo_state.currentChanPtr);

}
/****************************************************************************/
static int setupTone(SiVoiceChanType_ptr cPtr, int toneIndex, int isPeriodic)
{
  ProSLIC_ToneGenStop(cPtr);

  if( ProSLIC_ToneGenSetup(cPtr, toneIndex) == RC_NONE)
  {
    return ProSLIC_ToneGenStart(cPtr, isPeriodic);
  }
  else
  {
    LOGPRINT("%sSomething went wrong here %s %d\n",
        LOGPRINT_PREFIX, __FUNCTION__, __LINE__);
  }

  return RC_UNSUPPORTED_FEATURE;
}

/****************************************************************************/
static void goto_idle_state(pbx_demo_t *main, unsigned int chan)
{
  pbx_chan_t *port = &(main->chan_info[chan]);
  int dest_chan = port->chan_connect;

  main->cid_states[chan].ringCount = 0;
  main->chan_info[chan].chan_connect = SI_PBX_DEMO_DIGCOL_NOMATCH;

  ProSLIC_ToneGenStop(main->demo_state.currentChanPtr);
  ProSLIC_SetMuteStatus(main->demo_state.currentChanPtr, PROSLIC_MUTE_ALL);
  ProSLIC_PCMStop(main->demo_state.currentChanPtr);
  main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_IDLE;

#ifdef PBX_DEMO_ENABLE_MWI
  ProSLIC_MWIEnable(main->demo_state.currentChanPtr);
  ProSLIC_SetMWIState(main->demo_state.currentChanPtr, SIL_MWI_FLASH_ON);
  GetTime(NULL, &(main->chan_info[chan].timers[0])); /* For MWI toggle */
  main->chan_info[chan].mwiFlashState = SIL_MWI_FLASH_ON;
#endif
}

/****************************************************************************/
static void goto_reorder_state(pbx_demo_t *main, unsigned int chan)
{
  if( setupTone(main->demo_state.currentChanPtr,
      TONEGEN_FCC_REORDER, TRUE) == RC_NONE)
  {
    main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_REORDER;
  }
  else
  {
    goto_idle_state(main, chan);
  }
}

/****************************************************************************/
static void goto_ringback_state(pbx_demo_t *main, unsigned int chan)
{
  if( setupTone(main->demo_state.currentChanPtr,
      TONEGEN_FCC_RINGBACK, TRUE) == RC_NONE)
  {
    main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_RINGBACK;
  }
  else
  {
    goto_idle_state(main, chan);
  }
}

/****************************************************************************/
static void goto_busy_state(pbx_demo_t *main, unsigned int chan)
{
  if( setupTone(main->demo_state.currentChanPtr,
        TONEGEN_FCC_BUSY, TRUE) == RC_NONE)
  {
    main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_REORDER; /* Same code for busy */
  }
  else
  {
    goto_idle_state(main, chan);
  }
}

/****************************************************************************/
static void goto_cid_state(pbx_demo_t *main, unsigned int chan)
{
  /* NOTE current chan ptr is the originator, not destination of the call... chan = dest*/

  /*NOTE: this only supports NA CID - for now...*/
#ifdef CID_ONLY_DEMO
  int dest = main->chan_info[chan].chan_connect;
  pbx_chan_t *port = &(main->chan_info[chan]);
  time_t rawtime;
  struct tm *timeinfo;
  char buf[10];
  si_cid_na_params_t  *my_data;
  cid_demo_cid_data_t *orig_data; /* Data ptr for caller */
  int err;
#endif

  main->cid_states[chan].hook_change = FALSE;
  main->cid_states[chan].cid_is_offhook = FALSE;
  main->cid_states[chan].digitCount = 0;
  main->cid_states[chan].cid_fsk_interrupt = FALSE;
  main->cid_states[chan].cid_quit = FALSE;

  /* Turn off audio toward the PCM bus */
  ProSLIC_SetMuteStatus(main->demo_state.currentChanPtr, PROSLIC_MUTE_TX);


#ifdef CID_ONLY_DEMO
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  sprintf(buf,"%02d%02d%02d%02d",
          (timeinfo->tm_mon)+1,
          timeinfo->tm_mday,
          timeinfo->tm_hour,
          timeinfo->tm_min);

  /* NA specifc */
  SIVOICE_MEMCPY( &(main->cid_states[chan].init_data.encoder_data.na.packed_date),
      buf, SI_CID_ENC_NA_PACKED_DATE_LEN);

  /* Configure what to display */
  orig_data = &(main->cid_states[dest].cid_data);
  my_data = &(main->cid_states[chan].init_data.encoder_data.na);
  my_data->calling_name   = orig_data->name;
  my_data->calling_number = orig_data->number;
  my_data->dialable_number = orig_data->number;

  cid_demo_check_interrupts( &(main->cid_states[chan]), main->demo_state.currentChanPtr);
  err = si_cid_start( (uInt8)chan,
                  &(main->cid_states[chan].init_data));
  if (err != SI_CID_RC_OK)
  {
    LOGPRINT("%sERROR: failed to init CID SM rc = %d\n", LOGPRINT_PREFIX, err);
  }
#else
  {
    SiVoiceChanType_ptr dest_ptr = demo_get_cptr( &(main->demo_state), chan) ;

    /*
     * Since we do not have the CID framework to ring the phone, start
     * to ring the phone and wait...
     */
#ifdef PBX_DEMO_ENABLE_MWI
     ProSLIC_MWIDisable( dest_ptr );
#endif
#ifdef PROSLIC_MANUAL_RING
    ProSLIC_RingCadenceStart( dest_ptr,
      &(main->chan_info[chan].ringCadence) );
#else
    ProSLIC_RingStart( dest_ptr );
#endif
  }
#endif /* CID_ONLY_DEMO */

  main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_CID;
}

/****************************************************************************/
/* In idle state - should either transition to dialtone or ringing */
static void pbx_state_idle(pbx_demo_t *main, unsigned int chan,
                           BOOLEAN interrupt_detected)
{
  /* Did we just go off hook? */
  if( (main->cid_states[chan].hook_change == TRUE)
    &&  (main->cid_states[chan].cid_is_offhook == TRUE ) )
  {
#ifdef PBX_DEMO_ENABLE_MWI
     ProSLIC_MWIDisable( main->demo_state.currentChanPtr );    
#endif
     ProSLIC_SetMuteStatus(main->demo_state.currentChanPtr, PROSLIC_MUTE_NONE);

     if( setupTone(main->demo_state.currentChanPtr, TONEGEN_FCC_DIAL, FALSE) == RC_NONE)
     {
       main->cid_states[chan].digitCount = 0;
       GetTime(NULL, &(main->chan_info[chan].timers[0])); /* for digit collection timeout */
       main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_DIALTONE;

     }
     else
     {
       goto_idle_state(main, chan); /* Clear out any values */
     }
  }
#ifdef PBX_DEMO_ENABLE_MWI
  else
  {
    uInt32 elapsed_time;
    si_diffTime(main->demo_state.currentChanPtr,
      &(main->chan_info[chan].timers[0]),&elapsed_time);
    
    if( ( elapsed_time >= PBX_MWI_ON_TIME ) 
      && (main->chan_info[chan].mwiFlashState == SIL_MWI_FLASH_ON) )
    {
      ProSLIC_SetMWIState(main->demo_state.currentChanPtr, SIL_MWI_FLASH_OFF); 
      main->chan_info[chan].mwiFlashState = SIL_MWI_FLASH_OFF;
    }
    else if( elapsed_time >= (PBX_MWI_ON_TIME + PBX_MWI_OFF_TIME) 
      && (main->chan_info[chan].mwiFlashState == SIL_MWI_FLASH_OFF) )
    {
      ProSLIC_SetMWIState(main->demo_state.currentChanPtr, SIL_MWI_FLASH_ON);
      /* Reset the timer for MWI toggle */
      GetTime(NULL, &(main->chan_info[chan].timers[0])); 
      main->chan_info[chan].mwiFlashState = SIL_MWI_FLASH_ON;
    }
  }
#endif
}

/****************************************************************************/
static void pbx_state_dialtone(pbx_demo_t *main, unsigned int chan,
                               BOOLEAN interrupt_detected)
{
  uInt32 elapsed_time;

  /* Check if someone went back onhook */
  if( (main->cid_states[chan].hook_change == TRUE)
    &&  (main->cid_states[chan].cid_is_offhook == FALSE ) )
  {
    goto_idle_state(main,chan);
  }

  si_diffTime(main->demo_state.currentChanPtr,
      &(main->chan_info[chan].timers[0]),&elapsed_time);

  if( elapsed_time >= SI_SEC_TO_MSEC(SI_PBX_TIMEOUT_DIALTONE))
  {
    goto_reorder_state(main,chan);
    return;
  }

  /* Check if some digits were collected - if so stop dialtone and start to process them*/
  if(main->cid_states[chan].digitCount != 0)
  {
    ProSLIC_ToneGenStop(main->demo_state.currentChanPtr);

      GetTime(NULL, &(main->chan_info[chan].timers[0])); /* For digit collection timeout */
      main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_DIGIT_COLLECT;
  }
}

/****************************************************************************/
static void pbx_state_digitcollect(pbx_demo_t *main, unsigned int chan,
                                   BOOLEAN interrupt_detected)
{
  uInt32 elapsed_time;

  /* Check if someone went back onhook */
    if( (main->cid_states[chan].hook_change == TRUE)
      &&  (main->cid_states[chan].cid_is_offhook == FALSE ) )
  {
    goto_idle_state(main,chan);
  }

  if(interrupt_detected)
  {
    int dest;

    dest = si_pbx_digit_map(main,chan);

    if(dest >= 0)
    {
      main->chan_info[chan].chan_connect = dest;
      goto_ringback_state(main,chan);

      main->chan_info[dest].chan_connect = chan;
      goto_cid_state(main,dest);

      return;
    }

    if(dest == SI_PBX_DEMO_DIGCOL_BUSY)
    {
      goto_busy_state(main,chan);
      return;
    }
  }

  si_diffTime(main->demo_state.currentChanPtr,
      &(main->chan_info[chan].timers[0]),&elapsed_time);

  /* Did the user not finish entering a valid dial string? */
  if(elapsed_time >= SI_PBX_TIMEOUT_DIGITCOLLECT)
  {
    goto_reorder_state(main,chan);
  }
}

/****************************************************************************/
static void pbx_state_ringback(pbx_demo_t *main, unsigned int chan,
                               BOOLEAN interrupt_detected)
{
  int dest_port;
  dest_port = main->chan_info[chan].chan_connect;

  if(dest_port >= 0)
  {
    /* Check if someone went back onhook */
    if( (main->cid_states[chan].hook_change == TRUE)
        &&  (main->cid_states[chan].cid_is_offhook == FALSE ) )
    {
      /* Tell the destination we had aborted the call */
      if(main->chan_info[dest_port].state == SI_PBX_DEMO_FXS_STATE_CID)
      {
        chanState_t *cidState = &(main->cid_states[dest_port]);

        cidState->cid_quit = TRUE;
        cidState->hook_change = TRUE;
        pbx_state_cidgen(main, dest_port, TRUE);
      }
      else
      {
        goto_idle_state(main,dest_port);
}

      goto_idle_state(main, chan);
    }
    else
    {
      /* Did the far end go offhook? */
      if(main->cid_states[dest_port].cid_is_offhook == TRUE)
      {
        /* Turn off ringback tone */
        ProSLIC_ToneGenStop(main->demo_state.currentChanPtr);
        do_cross_connect(main, chan);
      }
    }
  }
}

/****************************************************************************/
static void pbx_state_connect(pbx_demo_t *main, unsigned int chan,
                              BOOLEAN interrupt_detected)
{
  int dest_port;

  /* Check if someone went back onhook */
  if( (main->cid_states[chan].hook_change == TRUE)
      &&  (main->cid_states[chan].cid_is_offhook == FALSE ) )
  {
    dest_port = main->chan_info[chan].chan_connect;

    if (dest_port >= 0)
    {
      GetTime(NULL, &(main->chan_info[dest_port].timers[0])); /* For power deny */
      main->chan_info[dest_port].state = SI_PBX_DEMO_FXS_STATE_DISCONNECT;
    }
    goto_idle_state(main,chan);
  }
}

/****************************************************************************/
static void pbx_state_disconnect(pbx_demo_t *main, unsigned int chan,
                                 BOOLEAN interrupt_detected)
{
  uInt32 elapsed_time;
  uInt8  hookStat;

  si_diffTime(main->demo_state.currentChanPtr,
        &(main->chan_info[chan].timers[0]),&elapsed_time);

  /* See how long we've been in power deny */
  if( elapsed_time >= SI_PBX_TIMEOUT_POWERDENY )
  {
    ProSLIC_SetLinefeedStatus(main->demo_state.currentChanPtr, LF_FWD_ACTIVE);

    /*
     * Check if we're offhook, if so, start the idle code immediately instead
     * of poll..
     */
    ProSLIC_ReadHookStatus(main->demo_state.currentChanPtr,&hookStat);

    if(hookStat == PROSLIC_OFFHOOK)
    {
      main->cid_states[chan].hook_change = TRUE;
      main->chan_info[chan].chan_connect = SI_PBX_DEMO_DIGCOL_NOMATCH;
      main->chan_info[chan].state = SI_PBX_DEMO_FXS_STATE_IDLE;
      main->cid_states[chan].cid_is_offhook = TRUE;
      pbx_state_idle(main, chan, interrupt_detected);
    }
    else
    {
      goto_idle_state(main,chan);
    }
  }
}

/****************************************************************************/
static void pbx_state_cidgen(pbx_demo_t *main, unsigned int chan,
                             BOOLEAN interrupt_detected)
{
  chanState_t *cidState = &(main->cid_states[chan]);
  int rc;

  if(cidState->cid_quit == TRUE)
  {
#ifdef PROSLIC_MANUAL_RING
  {
    SiVoiceChanType_ptr dest_ptr = demo_get_cptr( &(main->demo_state), chan) ;
    ProSLIC_RingCadenceStop( dest_ptr, &(main->chan_info[chan].ringCadence) );
  }
#endif

#ifdef CID_ONLY_DEMO
    if( (cidState->hook_change == TRUE) )
    {
      SiVoiceChanType_ptr dest_ptr = demo_get_cptr( &(main->demo_state), chan) ;
      /* Tell the state machine we're done... */
      si_cid_stop_cid(chan);
      ProSLIC_RingCadenceStop( dest_ptr, &(main->chan_info[chan].ringCadence) );

      /* Wait for the state machine to go through it's paces */
      rc = si_cid_poll(chan);

#else
    {
#endif
      /* Check if the destination (origin of the call in this case is still up */
      int dest = main->chan_info[chan].chan_connect;
      if( ( dest >=0) && (cidState->cid_is_offhook == TRUE) )
      {
        do_cross_connect(main, chan);
      }
      else
      {
        /* Current ChanPtr is pointing to the origination, so if we
         * do not go offhook on the destination, we need to call goto_idle_state
         * first on the origination and then change to the destination (chan) ptr
         */
        goto_idle_state(main,chan);
        main->demo_state.currentChanPtr = demo_get_cptr( &(main->demo_state), chan) ;
        goto_idle_state(main,chan);
        return;
      }
    }
  }
  else /* cidState->cid_quit == FALSE */
  {
#ifdef CID_ONLY_DEMO
    rc = si_cid_poll(chan);

    if (rc != SI_CID_RC_NEED_MORE_POLLS)
    {
#ifdef CID_MANUAL_RING_ENABLE
      SiVoiceChanType_ptr dest_ptr = demo_get_cptr( &(main->demo_state), chan) ;
      /* we will have a slight glitch in ring cadence timing here... */
      ProSLIC_RingCadenceStart( dest_ptr,
                               &(main->chan_info[chan].ringCadence) );
#endif
      cidState->cid_quit = TRUE;
    }
#endif
  }
}

/****************************************************************************/
static void pbx_state_reorder(pbx_demo_t *main, unsigned int chan,
                              BOOLEAN interrupt_detected)
{
  if( (main->cid_states[chan].hook_change == TRUE)
      &&  (main->cid_states[chan].cid_is_offhook == FALSE ) )
  {
    goto_idle_state(main,chan);
  }
}

/****************************************************************************/
typedef void (*fxs_sm)(pbx_demo_t *main, unsigned int chan,
                       BOOLEAN interrupt_detected);

const static fxs_sm fxs_states[SI_PBX_DEMO_LAST_FXS_STATE] =
{
  pbx_state_idle,
  pbx_state_dialtone,
  pbx_state_digitcollect,
  pbx_state_ringback,
  pbx_state_connect,
  pbx_state_disconnect,
  pbx_state_cidgen,
  pbx_state_reorder,
};

const char *fxs_state_names[] = {"Idle","Dial Tone", "Digit Collect",
                                 "Ring Back", "Connect", "Disconnect", "CID", "Reorder/Busy"
                                };
/****************************************************************************/
const char *pbx_demo_get_fxsstate_name(int state)
{
  return(fxs_state_names[state]);
}

/****************************************************************************/
void pbx_demo_sm_fxs_chan(pbx_demo_t *main, unsigned int chan,
                          BOOLEAN interrupt_detected)
{
  if(main->chan_info[chan].state != main->chan_info[chan].scratch_var[1])
  {
    printf("State changed from %s to %s for chan %d\n",
           pbx_demo_get_fxsstate_name(main->chan_info[chan].scratch_var[1]),
           pbx_demo_get_fxsstate_name(main->chan_info[chan].state),
           chan);
    main->chan_info[chan].scratch_var[1] = main->chan_info[chan].state;
  }

  fxs_states[main->chan_info[chan].state](main,chan, interrupt_detected);
}

