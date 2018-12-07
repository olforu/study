/******************************************************************************
 * Copyright (c) 2014-2016 by Silicon Laboratories
 *
 * $Id: spi_tests.c 6007 2016-10-03 16:26:33Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements some basic SPI communications tests to the ProSLIC.
 *
 */

#include "spi_tests.h"

#define REV_REG_ADDR      0
#define WRV_REG_ADDR      12
#define MSTRSTAT_REG_ADDR 3
#define PATCH_ID          448
#define RD_RAM_ADDR       634


/**********************************************************
 * Read the revision ID and see if it might be valid.
 */
static int spiReadRevIDTest(controlInterfaceType *hwIf, uInt8 channel)
{
  return spiReadRevIDTestCommon(hwIf, channel, REV_REG_ADDR);
}

/**********************************************************
 * Write a test pattern to a register, read back to see if
 * the value changed.
 */

static int spiBasicWriteTest(controlInterfaceType *hwIf, uInt8 channel)
{
  return spiBasicWriteTestCommon(hwIf, channel, WRV_REG_ADDR);
}
/**********************************************************
 * Check the master status register to see if it indicates
 * PCLK/FS are OK.
 */

static int spiMasterStatusTest(controlInterfaceType *hwIf, uInt8 channel)
{
  uInt8 data;
  int rc = 0;

  hwIf->WriteRegister_fptr(hwIf->hCtrl, channel, MSTRSTAT_REG_ADDR,
                           0xFF); /* Clear status */

  data = hwIf->ReadRegister_fptr(hwIf->hCtrl, channel, MSTRSTAT_REG_ADDR);

  if( (data & 0xFF) != 0x1F)
  {
    rc = 1;
    TEST_LOG("Expected: 0x1F, read: 0x%02X\n", data);
  }
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 *  Read from a RAM location that shouldn't be zero
 *  after bringup
 */

static int spiBasicRAMReadTest(controlInterfaceType *hwIf, uInt8 channel)
{
  int rc = 0;
  ramData data;

  data = hwIf->ReadRAM_fptr(hwIf->hCtrl, channel, RD_RAM_ADDR);

  if( (data == 0x0L)  || (data == 0xFFFFFFFFL))
  {
    rc = 1;
    TEST_LOG("Expected != 0 and 0xFFFFFFFF, have: 0x%08X", data);
  }
  REPORT_LOG(rc);
  return rc;
}

/**********************************************************
 *  Check RAM access in unprotected space.
 */
static int spiBasicRAMWriteTest(controlInterfaceType *hwIf, uInt8 channel)
{
  const ramData test_pattern[] = {0x10UL, 0x5A5A5A5AUL, 0xA5A5A5A5UL,
                                  0xFFFFFFFFUL, 0x01020304UL,0xA5005AFFUL, 0UL
                                 };
  int i;
  int rc =0;
  ramData data;

  for(i = 0; test_pattern[i] != 0; i++)
  {
    hwIf->WriteRAM_fptr(hwIf->hCtrl, channel, PATCH_ID, test_pattern[i]);
    data = hwIf->ReadRAM_fptr(hwIf->hCtrl, channel,PATCH_ID);
    if(data != (test_pattern[i] &0x1FFFFFFFUL)) /* 29 bits */
    {
      TEST_LOG("Expected: %08X read: %08X\n", test_pattern[i], data);
      rc = 1;
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
static int spiResetTest(controlInterfaceType *hwIf, uInt8 channel)
{
  return spiResetTestCommon(hwIf, channel, WRV_REG_ADDR);
}

/**********************************************************
 *  Run through some basic tests. This code is NOT compatible
 *  with Si321x or Si3050.
 */

static silabs_test spiTestList[] =
{
  spiReadRevIDTest, spiBasicWriteTest, spiMasterStatusTest,
  spiBasicRAMReadTest,spiBasicRAMWriteTest,
  spiResetTest,
  NULL
};

int spiTests(controlInterfaceType *hwIf, uInt8 channel)
{
  return runTests(hwIf, channel, spiTestList);
}

/**********************************************************
 * Timed tests start here...
 *
 */

static int spiTimedReadRegTest(controlInterfaceType *hwIf, uInt8 channel)
{
  return spiReadTimedTestCommon(hwIf, channel, REV_REG_ADDR, LOOP_COUNT);

}

static int spiTimedWriteRegTest(controlInterfaceType *hwIf, uInt8 channel)
{
  return spiWriteTimedTestCommon(hwIf, channel, WRV_REG_ADDR, LOOP_COUNT);
}

/**********************************************************
 *  Run through some basic tests and see how long they took.
 *  NOT compatbile with Si321x or Si3050.
 */

static silabs_test spiTimedTestList[] =
{
  spiTimedReadRegTest,
  spiTimedWriteRegTest,
  NULL
};

int spiTimedTests(controlInterfaceType *hwIf, uInt8 channel)
{
  return runTests(hwIf, channel, spiTimedTestList);
}


/**********************************************************
 *  Check for register/RAM access across multiple channels.
 * 
 *
 */

int SpiMultiChanTest(controlInterfaceType *hwIf, uInt8 channel_count)
{
  int i;
  int rc = 0;
  ramData RData;
  uInt8   rData;

  /* Do a simple write to 1 register with different values for each
   * channel.  Verify the values when read back match what we wrote.
   */
  for(i = 0; i < channel_count; i++)
  {
    hwIf->WriteRegister_fptr(hwIf->hCtrl, i, WRV_REG_ADDR, i);
  }

  for(i = 0; i < channel_count; i++)
  {
    rData = hwIf->ReadRegister_fptr(hwIf->hCtrl, i, WRV_REG_ADDR);

    if(  rData != i)
    {
      TEST_LOG("Expected: %08X read: %08X\n", i, rData);
      rc = 1;     
    }
  }
  
  /* Do a simple write to 1 RAM location with different values for each
   * channel.  Verify the values when read back match what we wrote.
   */
  for(i = 0; i < channel_count; i++)
  {
    hwIf->WriteRAM_fptr(hwIf->hCtrl, i, PATCH_ID, i);
  }

  for(i = 0; i < channel_count; i++)
  {
    RData = hwIf->ReadRAM_fptr(hwIf->hCtrl, i, PATCH_ID);

    if(  RData != i)
    {
      TEST_LOG("Expected: %08X read: %08X\n", i, RData);
      rc = 1;     
    }
  }
  REPORT_LOG(rc);
  return rc;
}
