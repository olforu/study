/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo.h 6476 2017-05-03 01:08:55Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** PBX demo header file.
**
*/

#ifndef __PBX_DEMO_HDR__
#define __PBX_DEMO_HDR__ 1

#ifdef CID_ONLY_DEMO
#include "cid_demo.h"
#endif

#include "demo_common.h"
#include "proslic_timer.h"

#ifdef PROSLIC_MANUAL_RING
#include "manual_ring.h"
#endif

#define SI_PBX_POLL_PERIOD 5                            /* in mSec */
#define SI_PBX_MSEC_TO_TICKS(MSEC) ((MSEC)/SI_PBX_POLL_PERIOD)
#define SI_SEC_TO_MSEC(SEC) ((SEC)*1000)
#define SI_PBX_TIMEOUT_DIALTONE 15                      /** In Seconds */
#define SI_PBX_TIMEOUT_DIGITCOLLECT SI_SEC_TO_MSEC(25)  /** In mSeconds */
#define SI_PBX_TIMEOUT_POWERDENY SI_SEC_TO_MSEC(5)      /** In Seconds */

#define SI_PBX_DEMO_MAX_TIMERS	1

/* Digit collection return codes */
#define SI_PBX_DEMO_DIGCOL_BUSY -2
#define SI_PBX_DEMO_DIGCOL_NOMATCH -1

#define PROSLIC_FSK_BUFSZ 7

#define SI_CID_ENC_NA 1

#undef  LOGPRINT_PREFIX  /* get rid of compiler warning */
#define LOGPRINT_PREFIX "PBXDEMO:"


#define PBX_TIMESLOTS(TS) (PCM_PRESET_NUM_8BIT_TS*(TS)*8)

typedef enum
{
  SI_PBX_DEMO_FXS_STATE_IDLE,
  SI_PBX_DEMO_FXS_STATE_DIALTONE,
  SI_PBX_DEMO_FXS_STATE_DIGIT_COLLECT,
  SI_PBX_DEMO_FXS_STATE_RINGBACK,
  SI_PBX_DEMO_FXS_STATE_CONNECT,
  SI_PBX_DEMO_FXS_STATE_DISCONNECT,
  SI_PBX_DEMO_FXS_STATE_CID,
  SI_PBX_DEMO_FXS_STATE_REORDER, /* Same as Busy in terms of operation*/
  SI_PBX_DEMO_LAST_FXS_STATE
} pbx_chan_state_t;

/* If we do not have the CID framework, put in a minimal substitution */
#ifndef CID_ONLY_DEMO
#define SI_DEMO_MAX_PH_NUM 24

typedef struct
{
    uInt8  number[SI_DEMO_MAX_PH_NUM];
} cid_demo_cid_data_t;


typedef struct
{
    unsigned int digitCount; /* How many digits did we collect? */
    char digits[15];
    BOOLEAN cid_is_offhook; /* Is the device off hook ? */
    BOOLEAN cid_quit;       /* Do we quit ? */
    BOOLEAN cid_fsk_interrupt; /* Did we detect a FSK interrupt */
    BOOLEAN ignore_hook_change; /* Do we ignore hook state change? */
    BOOLEAN hook_change; /* Did the hook state change during the execution? */
    uInt8 ringCount; /* How many rings did we do? */

    cid_demo_cid_data_t cid_data; /* init_data buffers */

} chanState_t;
#endif


typedef struct
{
  pbx_chan_state_t state; /** Current state (ringing, dial tone, etc) */
  int chan_connect;       /** Which channel is being attempted to be connected */
  int scratch_var[2];     /** Used by states for temp storage. 1 is reserved to store prior state */
  hookChangeType          hookDetection;  /* Hook debounce structure */
  timeStamp               hookTimer;
  timeStamp               timers[SI_PBX_DEMO_MAX_TIMERS];
#if defined(PROSLIC_MANUAL_RING) || defined( CID_MANUAL_RING_ENABLE)
  ProSLIC_ringCadenceState_t ringCadence;
#endif
#ifdef PBX_DEMO_ENABLE_MWI
  int                     mwiFlashState;
#endif
} pbx_chan_t;

/* Main structure for the demo - we reuse a bit from the CID
   demo struct...
*/
typedef struct
{
  demo_state_t demo_state; /* Generic demo structures */
  chanState_t  *cid_states; /* CID state info */

  pbx_chan_t   *chan_info;
  BOOLEAN      is_not_done;
} pbx_demo_t;

/**
 * @retval: either a non zero value to indicate a port to connect to or
 * SI_PBX_DEMO_DIGCOL_BUSY or SI_PBX_DEMO_DIGCOL_NOMATCH for error
 */
int si_pbx_digit_map(pbx_demo_t *main,unsigned int chan);

/**
 * Main entry point for PBX demo polling function...
 */
void pbx_demo_sm_fxs_chan(pbx_demo_t *main, unsigned int chan,
                          BOOLEAN interrupt_detected);

/**
 * Main entry point for demo.
 */
BOOLEAN pbx_demo_exec(pbx_demo_t *pbx_demo);

/* Just we need to tell the user outside the FSM code what state we're in... */
const char *pbx_demo_get_fxsstate_name(int state);

void pbx_menu(pbx_demo_t *main);

#endif /* __PBX_DEMO_HDR__ */

