/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo_fxs_hook.c 6238 2017-01-20 21:51:17Z nizajerk $
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
** This file implements the hook qualification code...
**
*/


#include "si_voice_datatypes.h"

#ifdef CID_ONLY_DEMO
#include "si_cid.h"
#include "si_cid_fsm.h"
#include "si_cid_int.h"
#include "si_cid_sys_srvc.h"
#endif

#include "si_voice.h"
#include "proslic.h"
#include "pbx_demo.h"

/****************************************************************************/
void hook_debounce_init(pbx_demo_t *main)
{
  int i;



  for(i = 0; i < main->demo_state.totalChannelCount; i++)
  {
    ProSLIC_InitializeHookChangeDetect( &(main->chan_info[i].hookDetection),
                                        &(main->chan_info[i].hookTimer) );
  }
}

/****************************************************************************/
int hook_debounce(pbx_demo_t *main, unsigned int chan,
                  BOOLEAN interrupt_detected)
{
  int result_value;
  SiVoiceChanType_ptr chanPtr;

  /* Pulse Dial/Hookflash Config */
  static hookChange_Cfg hook_change_cfg = { 20,    /* Min Break Time */
                                            80,    /* Max Break Time */
                                            20,    /* Min Make Time */
                                            80,    /* Max Make Time */
                                            90,    /* Min Inter-digit Delay */
                                            100,   /* Min hookflash */
                                            800,   /* Max hookflash */
                                            850
                                          }; /* Min Idle Time */

  if(interrupt_detected ||
      ( (main->chan_info[chan].hookDetection.lookingForTimeout) ) )
  {
    chanState_t *cid_ptr = &(main->cid_states[chan]);
    chanPtr = demo_get_cptr( &(main->demo_state), chan);

    result_value = ProSLIC_HookChangeDetect(chanPtr, &hook_change_cfg,
        &(main->chan_info[chan].hookDetection) );

    /* Check if we had a pulse digit detected */
    if ( SI_HC_DIGIT_DONE(result_value) )
    {

      if(cid_ptr->digitCount < 15)
      {
        cid_ptr->digits[cid_ptr->digitCount] = result_value;
        cid_ptr->digitCount++;
      }
      return 1;
    }

    switch(result_value)
    {
      case SI_HC_HOOKFLASH:
      {
        /* Pop up a menu */
        pbx_menu(main);
      }
      break;

      case SI_HC_ONHOOK_TIMEOUT:
       cid_ptr->cid_is_offhook = FALSE;
       cid_ptr->hook_change = 1;
       /* LOGPRINT("%schan: %d is onhook\n", LOGPRINT_PREFIX, chan); */
       return 1;

      case SI_HC_OFFHOOK_TIMEOUT:
        cid_ptr->cid_is_offhook = TRUE;
        cid_ptr->cid_quit = TRUE;
        cid_ptr->hook_change = 1;
        /* LOGPRINT("%schan: %d is offhook\n", LOGPRINT_PREFIX, chan); */
        return 1;

    }
  }
  return 0;

}

