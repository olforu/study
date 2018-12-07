/*
** Copyright (c) 2017 by Silicon Laboratories
**
** $Id: manual_ring.h 6466 2017-05-01 16:43:18Z nizajerk $
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

#ifndef __PROSLIC_MANUAL_CADENCE_HDR__
#define __PROSLIC_MANUAL_CADENCE_HDR__ 1

#include "si_voice_datatypes.h"
#include "si_voice.h"
#include "demo_config.h"

#ifndef PROSLIC_MAX_RING_PERIODS
#define PROSLIC_MAX_RING_PERIODS 1 
#endif 

/* Define the on/off period for 1 ring period */
typedef struct
{
 uInt16 onTime;  /* In ticks */
 uInt16 offTime; /* "" */
} ProSLIC_ringPeriod_t;

/* Define a complete ring cadence */
typedef struct
{
  uInt8 num_periods;
  ProSLIC_ringPeriod_t timing[PROSLIC_MAX_RING_PERIODS];
} ProSLIC_ringCadence_t;

typedef enum 
{
  PROSLIC_RING_CADENCE_UNKNOWN, 
  PROSLIC_RING_CADENCE_OFF,
  PROSLIC_RING_CADENCE_RING_ON,
  PROSLIC_RING_CADENCE_RING_OFF
} ProSLIC_ringCadenceStates_t;

typedef struct
{
  ProSLIC_ringCadence_t        cadence;
  ProSLIC_ringCadenceStates_t  state;
  uInt16                       ticks; /* How many ticks left to next state? */
  uInt8                        current_period;
} ProSLIC_ringCadenceState_t;

int ProSLIC_RingCadenceInit( ProSLIC_ringCadenceState_t *state, ProSLIC_ringCadence_t *cadence );
int ProSLIC_RingCadencePoll( SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state );
int ProSLIC_RingCadenceStart( SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state );
int ProSLIC_RingCadenceStop( SiVoiceChanType_ptr pProslic, ProSLIC_ringCadenceState_t *state );

#endif /* __PROSLIC_MANUAL_CADENCE_HDR__ */

