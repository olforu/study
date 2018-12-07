/******************************************************************************
 * Copyright (c) 2015-2017 by Silicon Laboratories
 *
 * $Id: proslic_timer.h 6800 2017-09-14 22:23:55Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This implements the POSIX compatible timer interface.
 *
*/

#ifndef POSIX_TIMER_H
#define POSIX_TIMER_H

#include "proslic_platform_cfg.h"

#ifdef __GNUC__
#include <unistd.h>
#include <time.h>

#ifdef SILABS_USE_TIMEVAL
#include <sys/time.h>
#else

#include <time.h>
#ifndef SILABS_CLOCK
#error "Please select clock source for timespec source"
#endif

#endif /* SILABS_USE_TIMEVAL */
#else
#error Unsupported compiler
#endif /* __GNUC__ */

typedef struct
{
  int dummy; /* put a placeholder here to avoid compiler warnings */
} systemTimer_S ;

/*
** System time stamp
*/
typedef struct
{
#ifdef SILABS_USE_TIMEVAL
  struct timeval timerObj;
#else
  struct timespec timerObj;
#endif
} timeStamp;

/*
** Function: SYSTEM_TimerInit
**
** Description:
** Initializes the timer used in the delay and time measurement functions
** by doing a long inaccurate sleep and counting the ticks
**
** Input Parameters:
**
** Return:
** none
*/

void TimerInit (systemTimer_S *pTimerObj);

/*
** Function: DelayWrapper
**
** Description:
** Waits the specified number of ms
**
** Input Parameters:
** hTimer: timer pointer
** timeInMs: time in ms to wait
**
** Return:
** none
*/
int time_DelayWrapper (void *hTimer, int timeInMs);


/*
** Function: TimeElapsedWrapper
**
** Description:
** Calculates number of ms that have elapsed
**
** Input Parameters:
** hTImer: pointer to timer object
** startTime: timer value when function last called
** timeInMs: pointer to time elapsed
**
** Return:
** timeInMs: time elapsed since start time
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs);

int time_GetTimeWrapper (void *hTimer, void *time);
#endif

