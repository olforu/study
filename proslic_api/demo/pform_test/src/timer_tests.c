/******************************************************************************
 * Copyright (c) 2015-2016 by Silicon Laboratories
 *
 * $Id: timer_tests.c 5665 2016-05-16 22:21:32Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements some basic timer API tests.
 *
 */

#include "timer_tests.h"
#ifndef PROSLIC_LINUX_KERNEL
#include <stdlib.h> /* for labs */
#include "proslic_timer.h" /* timeStamp definition */
#define SI_ABS        labs
#else
#include <linux/kernel.h>
#include "proslic_sys.h"
typedef proslic_timeStamp timeStamp;
#define SI_ABS       abs
#endif

#define SI_TIMER_SLOP 5 /* How may mSec can we be off by? */

#define SI_TIMER_DELTA_OK(ACTUAL, EXPECTED) ( SI_ABS(((ACTUAL) - (EXPECTED))) <= SI_TIMER_SLOP)

/**********************************************************
 * Check that the elapsed timer/delay time relationship is OK
 */
static int timerElapsedTest(controlInterfaceType *hwIf, uInt8 channel,
                            int delayInMsec)
{
  timeStamp timeStart;
  int rc;
  int delta_time; /* In terms of mSec */

  SILABS_UNREFERENCED_PARAMETER(channel);

  hwIf->getTime_fptr(hwIf->hTimer, &timeStart);
  hwIf->Delay_fptr(hwIf->hTimer, delayInMsec);

  hwIf->timeElapsed_fptr(hwIf->hTimer, &timeStart, &delta_time);

  if(SI_TIMER_DELTA_OK(delta_time, delayInMsec))
  {
    rc = 0;
  }
  else
  {
    TEST_LOG("measured: %d mSec, expected: %d +/-%d mSec\n",
             delta_time, delayInMsec, SI_TIMER_SLOP);
    rc = 1;
  }

  return rc;
}

/**********************************************************
 * Check that the elapsed timer/delay time relationship is OK
 */
static int timerElapsed_1Sec(controlInterfaceType *hwIf, uInt8 channel)
{
  int rc;
  rc = timerElapsedTest(hwIf, channel, 1000);
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 * Check that the elapsed timer/delay time relationship is OK
 */
static int timerElapsed_10mSec(controlInterfaceType *hwIf, uInt8 channel)
{
  int rc;
  rc = timerElapsedTest(hwIf, channel, 10);
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 * Check that the elapsed timer/delay time relationship is OK
 */
static int timerElapsed_2015mSec(controlInterfaceType *hwIf, uInt8 channel)
{
  int rc;
  rc = timerElapsedTest(hwIf, channel, 2015);
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 * Check that the elapsed timer/delay time relationship is OK
 */
static int timerElapsed_5mSec(controlInterfaceType *hwIf, uInt8 channel)
{
  int rc;
  rc = timerElapsedTest(hwIf, channel, 5);
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 *  Run through some basic timer tests.
 */

static silabs_test testList[] =
{
  timerElapsed_1Sec,
  timerElapsed_10mSec,
  timerElapsed_2015mSec,
  timerElapsed_5mSec,
  NULL
};

int timerTests(controlInterfaceType *hwIf, uInt8 channel)
{
  return runTests(hwIf, channel, testList);
}

