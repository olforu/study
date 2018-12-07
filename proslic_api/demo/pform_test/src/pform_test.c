/******************************************************************************
 * Copyright (c) 2015-2016 by Silicon Laboratories
 *
 * $Id: pform_test.c 6007 2016-10-03 16:26:33Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements a simple platform validation tool.
 *
 */

#include <stdio.h>
#include "proslic.h"
#include "proslic_timer.h"

#include "pform_test.h"
#include "spi_main.h"
#include "spi_tests.h"
#include "timer_tests.h"
#include "user_intf.h"

/**********************************************************
 * main entry point. 0 = OK, < 0 = error.
 */
int main(int argc, char *argv[])
{
  ctrl_S spiGciObj;              /* Link to host spi obj (os specific) */
  systemTimer_S timerObj;        /* Link to host timer obj (os specific)*/
  controlInterfaceType	hwIf;    /* No dynamic memory allocation */
  int rc = -2;
  int channel_count = 1;
  int i;

  SETUP_STDOUT_FLUSH;

  printf("\nBasic platform validation tool version 0.1.0\n");
  printf("Copyright 2015-2016, Silicon Laboratories\n--------------------------------------------------------\n");

  /* Initialize platform */
  if(SPI_Init(&spiGciObj) == FALSE)
  {
    printf("Cannot connect to %s\n", VMB_INFO);
    return -1;
  }

  if( argc > 1 )
  {
    channel_count = atoi(argv[1]);
  }

  /*
   ** Establish linkage between host objects/functions
   ** and ProSLIC API (required)
   */
  printf("Demo: Linking function pointers...\n");
  initControlInterfaces(&hwIf, &spiGciObj, &timerObj);

  /* Now start testing */
  for(i = 0; i < channel_count; i++)
  {
    printf("Testing channel %d\n", i);

    rc = spiTests(&hwIf,i);

    if(rc != 0)
    {
      return rc;
    }
  }

  printf("\n");
  if((channel_count > 1) && (rc == 0) )
  {
    rc = SpiMultiChanTest(&hwIf, channel_count);
    printf("\n");
  }

  if(rc == 0)
  {
    /* TimerInit isn't part of the API specification, but may be used for our demo
       code.  This function may be removed for your specific target/OS.
     */
    TimerInit(hwIf.hTimer);
    rc += timerTests(&hwIf,0);
  }

  printf("\n");

  /* See how fast we can transfer data */
  if(rc == 0)
  {
    for(i = 0; i < channel_count; i++)
    {
      printf("Testing channel %d\n", i);
      rc = spiTimedTests(&hwIf,i);

      if(rc != 0)
      {
        return rc;
      }
    }
  }

  /* TODO: if you did implement a TimerInit and did allocate resources, you should
    free them here.
   */

  /* Close any system resources */
  destroyControlInterfaces(&hwIf);

  return rc;
}

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
    FLUSH_STDOUT;
  }
  while((*test != NULL) && (rc == 0));

  return rc;
}

