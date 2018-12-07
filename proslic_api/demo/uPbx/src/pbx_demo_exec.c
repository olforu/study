/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo_exec.c 6476 2017-05-03 01:08:55Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file contains routines to execute the PBX statemachine at
** a high level.
**
**
*/

#include "si_voice_datatypes.h"

#ifdef CID_ONLY_DEMO
#include "cid_demo.h"
#endif

#include "pbx_demo.h"
#include "proslic.h"

#ifdef PROSLIC_MANUAL_RING
#include "manual_ring.h"
#endif

#define SI_CTRLIF(PCHAN) ((PCHAN)->deviceId->ctrlInterface)
#define si_msleep(PCHAN, mSecSleep) (SI_CTRLIF(PCHAN)->Delay_fptr(SI_CTRLIF(PCHAN)->hTimer, (mSecSleep)))


int hook_debounce(pbx_demo_t *main, unsigned int chan,
                  BOOLEAN interrupt_detected);

static int num_irqs = 0; /* Since we can't change cid_demo_check_interrupts prototype for the CID framework,
                            we need to convey to pbx_demo_poll_chan_fxs the number of interrupts
                            by this method.
                         */
static int has_hc = 0;

/****************************************************************************
A simple interrupt handler for Caller ID - DTMF detection (if equipped),
hook state change, ring complete, and FSK transmission (if needed).

@warn
This does not handle certain events such as thermal alarms, etc. that would
be needed in a "production" environment.
*/
static int handle_ints(chanState_t *pState, SiVoiceChanType_ptr chanPtr, ProslicInt eInput)
{
  switch(eInput)
  {
    case IRQ_DTMF: /* Detected a DTMF */
      if(pState->digitCount < 15)
      {
        ProSLIC_DTMFReadDigit(chanPtr ,(uInt8 *)(&(pState->digits[pState->digitCount])));
        pState->digitCount++;
      }
      //LOGPRINT("INTERRUPT: DTMF %d %d\n",chanPtr->channel, pState->digitCount);
      return 1;

    case IRQ_LOOP_STATUS: /* Hook change during caller ID, abort and return back to normal state */
    case IRQ_RING_TRIP:
      if(pState->ignore_hook_change == TRUE) /* This is mainly for debug.. */
      {
        return 0;
      }

      //LOGPRINT("INTERRUPT: HOOK change chan: %d\n",chanPtr->channel);
      has_hc = 1;
      break;

    case IRQ_RING_T1: /* We completed a ring cycle */
      pState->ringCount++;
      //LOGPRINT("INTERRUPT: RING\n");
      break;

    case IRQ_FSKBUF_AVAIL:
      //LOGPRINT("INTERRUPT: FSK\n");
      pState->cid_fsk_interrupt = TRUE;
      return 1;

    default:
      break;
  }

  return 0;
}

/****************************************************************************/
/* Check if interrupts exist and to collect them. */
void cid_demo_check_interrupts(chanState_t *channel, SiVoiceChanType_ptr chanPtr)
{
  proslicIntType irqs;
  ProslicInt arrayIrqs[MAX_PROSLIC_IRQS];
  unsigned int i;

  irqs.irqs = arrayIrqs;
  num_irqs = 0;
  has_hc = 0;

  if (ProSLIC_GetInterrupts(chanPtr,&irqs) != 0)
  {
    /* Iterate through the interrupts and call handle_ints to process them */
    for(i = 0; i < irqs.number; i++)
    {
      num_irqs += handle_ints(channel, chanPtr, irqs.irqs[i]);
    }
  }
}

/****************************************************************************/
/* Checks interrupt sources and returns TRUE if an event occurred */
static int pbx_demo_poll_chan_fxs(pbx_demo_t *pbx_demo, SiVoiceChanType_ptr slicPtr,
                                      unsigned int channel)
{
  int rc;
  pbx_demo->cid_states[channel].cid_fsk_interrupt = FALSE;

  cid_demo_check_interrupts( &(pbx_demo->cid_states[channel]),
      slicPtr);

  num_irqs +=
      hook_debounce(pbx_demo, channel, has_hc);
#if defined(PROSLIC_MANUAL_RING) || defined( CID_MANUAL_RING_ENABLE)
  ProSLIC_RingCadencePoll(slicPtr, &(pbx_demo->chan_info[channel].ringCadence) );
#endif
  return num_irqs;
}

/****************************************************************************/
BOOLEAN pbx_demo_exec(pbx_demo_t *pbx_demo)
{
  int i;

  do
  {
    for(i = 0; i < pbx_demo->demo_state.totalChannelCount; i++)
    {
      SiVoiceChanType_ptr cptr = demo_get_cptr( &(pbx_demo->demo_state), i);

      pbx_demo->demo_state.currentChanPtr = cptr;

      if(cptr->channelType == PROSLIC)
      {
        pbx_demo_sm_fxs_chan(pbx_demo, i, pbx_demo_poll_chan_fxs(pbx_demo, cptr, i));
      }
      /* Nothing for DAAA - yet */
    }
    si_msleep( pbx_demo->demo_state.currentChanPtr, SI_PBX_POLL_PERIOD); /* We don't care which channel here... */
  }
  while(pbx_demo->is_not_done == FALSE);

  return TRUE;
}

