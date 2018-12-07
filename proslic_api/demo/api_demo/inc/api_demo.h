/*
** Copyright (c) 2014-2018 by Silicon Laboratories
**
** $Id: api_demo.h 7054 2018-04-06 20:57:58Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the header file for the API demo
**
*/

#ifndef API_DEMO_H
#define API_DEMO_H

#include "proslic.h"
#include "proslic_timer.h"
#include "proslic_tstin.h"
#include "vdaa.h"
#include "demo_config.h"
#include "demo_common.h"

/*
** Describe Hardware (single, standard EVB)
** See below if using custom/partner hardware
*/

#define SI3217XB_NUMBER_OF_CHAN (SI3217XB_NUMBER_OF_DEVICE*SI3217X_CHAN_PER_DEVICE)
#define SI3217XC_NUMBER_OF_CHAN (SI3217XC_NUMBER_OF_DEVICE*SI3217X_CHAN_PER_DEVICE)
#define SI3218X_NUMBER_OF_CHAN (SI3218X_NUMBER_OF_DEVICE*SI3218X_CHAN_PER_DEVICE)
#define SI3219X_NUMBER_OF_CHAN (SI3219X_NUMBER_OF_DEVICE*SI3219X_CHAN_PER_DEVICE)
#define SI3226X_NUMBER_OF_CHAN (SI3226X_NUMBER_OF_DEVICE*SI3226X_CHAN_PER_DEVICE)
#define SI3228X_NUMBER_OF_CHAN (SI3228X_NUMBER_OF_DEVICE*SI3228X_CHAN_PER_DEVICE)
#define SI3050_NUMBER_OF_CHAN (SI3050_NUMBER_OF_DEVICE*SI3050_CHAN_PER_DEVICE)

/*
** If using custom hardware or stacking evb's (adding more channels)
** explicitly undef and redefine number of device below.
*/

/* eg.  4 Si3226x EVBs Stacked or 8 channel design
#undef SI3226X_NUMBER_OF_DEVICE
#define SI3226X_NUMBER_OF_DEVICE 4
*/

/*
** CONSTANTS
*/
#define PSTN_CHECK_AVG_THRESH       5000    /* 5ma */
#define PSTN_CHECK_SINGLE_THRESH    40000   /* 40mA */
#define PSTN_CHECK_SAMPLES          8

#define LOGPRINT_PREFIX "APIDEMO: "

/*
** API Demo Functions
*/
void change_channel(demo_state_t *state);
void proslic_main_menu(demo_state_t *state);
void daa_main_menu(demo_state_t *state);
void daa_audio_menu(demo_state_t *state);
void daa_linestate_menu(demo_state_t *state);
void debugMenu(demo_state_t *state);
void testMonitorMenu(demo_state_t *state);
void linefeedMenu(demo_state_t *state);
void converterMenu(demo_state_t *state);
void ringingMenu(demo_state_t *state);
void audioMenu(demo_state_t *state);
void toneGenMenu(demo_state_t *state);
void pmMwiMenu(demo_state_t *state);
void interruptMenu(demo_state_t *state);
int irq_demo_check_interrupts(demo_state_t *pState, uInt8 *hook_det);

#endif

