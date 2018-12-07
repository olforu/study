/******************************************************************************
 * Copyright (c) 2015-2016 by Silicon Laboratories
 *
 * $Id: pform_test_module.c 6086 2016-10-27 21:31:17Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements a simple platform validation tool.
 * This module relies upon the ProSLIC system services module in that
 * it tests to ensure compliance with the ProSLIC API requirements.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include "spi_main.h"
#include "pform_test.h"
#include "spi_tests.h"
#include "timer_tests.h"

#define PROSLIC_PFORM_TEST_VER     "0.0.1" /* NOTE: Not the same version as the userspace version! */
#define DRIVER_DESCRIPTION         "ProSLIC API platform test module"
#define DRIVER_AUTHOR              "Silicon Laboratories"
#define SUCCESS                    0


/**********************************************************
 * Test runner
 *
 */

int runTests(controlInterfaceType *hwIf, uInt8 channel, silabs_test *testList)
{
  int rc = 0;
  silabs_test *test = testList;

  do
  {
    rc = (*test)(hwIf, channel);
    test++;
  }
  while((*test != NULL) && (rc == 0));

  return rc;
}

/*****************************************************************************************************/
int init_module(void)
{
  int rc;
  void *hCtrl;

  printk( KERN_INFO "%s ProSLIC API platform test module loaded, version: %s\n",
          PROSLIC_TEST_PREFIX, PROSLIC_PFORM_TEST_VER);
  printk( KERN_INFO "%s Copyright 2015, Silicon Laboratories\n",
          PROSLIC_TEST_PREFIX);

  /*
   * Since the ProSLIC systems services module, as delivered from Silabs does not need further initialization
   * we skip any allocation of timer and spi resources in this module... we do check if there was a device detected
   * though..
   */

  if(proslic_get_channel_count() == 0)
  {
    return -EIO;
  }

  hCtrl = proslic_get_hCtrl(0);

  if(hCtrl)
  {
    controlInterfaceType hwIf;
    initControlInterfaces(&hwIf, hCtrl, NULL);

    rc = spiTests(&hwIf, 0);

    rc += timerTests(&hwIf,0);

    /* See how fast we can transfer data */
    if(rc == 0)
    {
      rc = spiTimedTests(&hwIf,0);
    }

  }
  else
  {
    return -EIO;
  }

  return rc;

}
/*****************************************************************************************************/
void cleanup_module(void)
{
  /* Since we did not allocate any resources here, we really do nothing here */
  printk( KERN_INFO "%s ProSLIC API platform test module unloaded\n",
          PROSLIC_TEST_PREFIX);
}

MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("Proprietary");

