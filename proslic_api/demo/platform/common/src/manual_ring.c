/*
** Copyright (c) 2017 by Silicon Laboratories
**
** $Id: manual_ring.c 6466 2017-05-01 16:43:18Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** Manual ring cadence/distinctive ring header file.
**
*/

#include "manual_ring.h"
#include "proslic.h"

/******************************************************************************/
int ProSLIC_RingCadenceInit(ProSLIC_ringCadenceState_t *state, ProSLIC_ringCadence_t *cadence )
{
  state->cadence = *cadence;
  state->state   = PROSLIC_RING_CADENCE_OFF;

  return RC_NONE;
}

/******************************************************************************/
int ProSLIC_RingCadencePoll(SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state)
{
  switch(state->state)
  {
    case PROSLIC_RING_CADENCE_RING_ON:
    {
      (state->ticks)--; 
      if( state->ticks == 0 )
      {
        state->state = PROSLIC_RING_CADENCE_RING_OFF;
        state->ticks = state->cadence.timing[state->current_period].offTime;
        return ProSLIC_RingStop(pProslic);
      }
    }
    break;

    case PROSLIC_RING_CADENCE_RING_OFF:
    {
      (state->ticks)--; 
      if( state->ticks == 0 )
      {
        state->state = PROSLIC_RING_CADENCE_RING_ON;

        /* Go to next period, if we hit the end, restart */
        (state->current_period)++;
        if( state->current_period >= state->cadence.num_periods )
        {
          state->current_period = 0;
        }
        state->ticks = state->cadence.timing[state->current_period].onTime;
        return ProSLIC_RingStart(pProslic);
    }
    break;
    }

    default:
      /* Do nothing */
      break;
  }

  return RC_NONE;
}

/******************************************************************************/
int ProSLIC_RingCadenceStart(SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state)
{
  state->state = PROSLIC_RING_CADENCE_RING_ON;
  state->ticks = state->cadence.timing[0].onTime;
  state->current_period = 0;

  return ProSLIC_RingStart(pProslic);
}

/******************************************************************************/
int ProSLIC_RingCadenceStop(SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state)
{
  state->state = PROSLIC_RING_CADENCE_OFF;
  return ProSLIC_RingStop(pProslic);
}

