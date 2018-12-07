/******************************************************************************
 * Copyright (c) 2015-2016 by Silicon Laboratories
 *
 * $Id: spi_tests_common.c 5532 2016-02-09 17:28:11Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements some basic SPI communications tests common to all parts.
 *
 */

#include "spi_tests.h"
#ifndef PROSLIC_LINUX_KERNEL
#include "proslic_timer.h"
#else
#include <linux/kernel.h>
#include "proslic_sys.h"
typedef proslic_timeStamp timeStamp;
#endif

/**********************************************************
 * Read the revision ID and see if it might be valid.
 */

int spiReadRevIDTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                           uInt8 regAddr)
{
  uInt8 data, lastData;
  int rc = 0;

  data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, regAddr);

  if( (data == 0) || (data == 0xFF) )
  {
    TEST_LOG("Expected: != 0 && != 0xFF, read: 0x%02X\n",data);
    rc = 1;
  }

  lastData = data;

  /* Check for consistency of the read */
  if(rc == 0)
  {
    int i;
    for(i = 0; i < 3; i++)
    {
      data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, regAddr);

      if(data != lastData)
      {
        TEST_LOG("Expected: 0x%02X read: 0x%02X\n", lastData, data);
        rc = 1;
      }
    }
  }

  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 * Write a test pattern to a register, read back to see if
 * the value changed.
 */

int spiBasicWriteTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                            uInt8 regAddr)
{
  const uInt8 test_pattern[] = {0x5A, 0xA5, 0xFF, 0x55,0};
  uInt8 data;
  int i;
  int rc = 0;

  for(i = 0; test_pattern[i] != 0; i++)
  {
    hwIf->WriteRegister_fptr(hwIf->hCtrl, channel, regAddr, test_pattern[i]);

    data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, regAddr);

    if( (data & 0xFF) != test_pattern[i])
    {
      rc = 1;
      TEST_LOG("Expected: 0x%02X, read: 0x%02X\n",test_pattern[i], data);
      break;
    }
  }
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 *  Check if reset pin is working by writing to a
 *  register and reseting the device and see if the value
 *  changed.
 */

int spiResetTestCommon(controlInterfaceType *hwIf, uInt8 channel, uInt8 regAddr)
{
  uInt8 data;
  int rc = 0;

  hwIf->WriteRegister_fptr(hwIf->hCtrl, channel, regAddr, 0x12);

  /* Now reset the device */

  hwIf->Reset_fptr(hwIf->hCtrl,TRUE);
  hwIf->Delay_fptr(hwIf->hTimer,250);
  hwIf->Reset_fptr(hwIf->hCtrl,FALSE);
  hwIf->Delay_fptr(hwIf->hTimer,250);

  data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, regAddr);

  if(data == 0x12)
  {
    TEST_LOG("Expected: !=0x12 read: %02X\n", data);
    rc = 1;
  }

  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 *  Run a read test on 1 register, make sure it's consistent
 *  report how long the time it took to read N times...
 */

int spiReadTimedTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                           uInt8 regAddr, uInt32 count)
{
  uInt8 lastData, data;
  timeStamp timeStart;
  int delta_time;
  uInt32 error_count = 0;
  uInt32 i;

  lastData = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel,
                                     regAddr); /* This is our "expected value" */
  hwIf->getTime_fptr(hwIf->hTimer, &timeStart); /* We'll start our timing here */

  for(i = count; i; i--)
  {
    data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, regAddr);
    if(data != lastData)
    {
      error_count++;
    }
  }
  hwIf->timeElapsed_fptr(hwIf->hTimer, &timeStart, &delta_time);

  TEST_LOG("measured: %d mSec, for %u runs.  Had %u errors\n", delta_time, count,
           error_count);

  REPORT_LOG((error_count != 0));
  return(error_count != 0);
}

/**********************************************************
 *  Run a write test on 1 register (ASSUMES consistent)
 *  report how long the time it took to read N times...
 */

int spiWriteTimedTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                            uInt8 regAddr, uInt32 count)
{
  timeStamp timeStart;
  int delta_time;
  uInt32 i;

  hwIf->getTime_fptr(hwIf->hTimer, &timeStart); /* We'll start our timing here */

  for(i = count; i; i--)
  {
    hwIf->WriteRegister_fptr(hwIf->hCtrl, channel, regAddr, i%0xFF);
  }

  hwIf->timeElapsed_fptr(hwIf->hTimer, &timeStart, &delta_time);

  TEST_LOG("measured: %d mSec, for %u runs.\n", delta_time, count);
  REPORT_LOG(0); /* Not really doing a test, just a benchmark */
  return 0;
}

