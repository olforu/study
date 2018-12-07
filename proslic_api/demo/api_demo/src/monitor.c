/*
** Copyright (c) 2013-2016 by Silicon Laboratories, Inc.
**
** $Id: monitor.c 7054 2018-04-06 20:57:58Z nizajerk $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** line monitoring resources
**
*/

#include "api_demo.h"
#include "proslic.h"
#include "macro.h"
#include "user_intf.h"

/*****************************************************************************************************/

static void lineMonitor(demo_state_t *pState)
{
  proslicMonitorType monitor;

  ProSLIC_SetPowersaveMode(pState->currentChanPtr, PWRSAVE_DISABLE);
  Delay(pProTimer,50);

  ProSLIC_LineMonitor((pState->currentChanPtr), &monitor);

  print_banner("ProSLIC LINE MONITOR");
  printf ("VTR      = %d.%d  v\n", (int)(monitor.vtr/1000),
          (int)(abs(monitor.vtr - monitor.vtr/1000*1000)));
  printf ("VTIP     = %d.%d  v\n", (int)(monitor.vtip/1000),
          (int)(abs(monitor.vtip - monitor.vtip/1000*1000)));
  printf ("VRING    = %d.%d  v\n", (int)(monitor.vring/1000),
          (int)(abs(monitor.vring - monitor.vring/1000*1000)));
  printf ("VLONG    = %d.%d  v\n", (int)(monitor.vlong/1000),
          (int)(abs(monitor.vlong - monitor.vlong/1000*1000)));
  printf ("VBAT     = %d.%d  v\n", (int)(monitor.vbat/1000),
          (int)(abs(monitor.vbat - monitor.vbat/1000*1000)));
  printf ("VDC      = %d.%d  v\n", (int)(monitor.vdc/1000),
          (int)(abs(monitor.vdc - monitor.vdc/1000*1000)));
  printf ("ITR      = %d.%d  mA\n", (int)(monitor.itr/1000),
          (int)(abs(monitor.itr - monitor.itr/1000*1000)));
  printf ("ITIP     = %d.%d  mA\n", (int)(monitor.itip/1000),
          (int)(abs(monitor.itip - monitor.itip/1000*1000)));
  printf ("IRING    = %d.%d  mA\n", (int)(monitor.iring/1000),
          (int)(abs(monitor.iring - monitor.iring/1000*1000)));
  printf ("ILONG    = %d.%d  mA\n", (int)(monitor.ilong/1000),
          (int)(abs(monitor.ilong - monitor.ilong/1000*1000)));
  printf ("P_HVIC   = %d mW\n", (int)(monitor.p_hvic));
}

/*****************************************************************************************************/
void testMonitorMenu(demo_state_t *pState)
{
#ifdef TSTIN_SUPPORT
  int testResult = 0;
#endif

  const char *menu_items[] =
  {
    "FXS Line Monitor",
#ifdef TSTIN_SUPPORT
    "(Inward) Toggle PCM Loopback",
    "(Inward) DC Feed Test",
    "(Inward) Ringing Test",
    "(Inward) Battery Test",
    "(Inward) Audio Test",
    "Print Inward Test Limits",
    "Simulated hook state change.",
#endif
    NULL
  };

  int user_selection;


  do
  {
    user_selection = get_menu_selection( display_menu("Monitor Menu", menu_items),
                                         pState->currentChannel);
    printf("\n");

    switch(user_selection)
    {
      case 0:
        lineMonitor(pState);
        break;

#ifdef TSTIN_SUPPORT
      case 1:
        if(pState->currentPort->pTstin->pcmLpbkTest.pcmLpbkEnabled)
        {
          ProSLIC_testInPCMLpbkDisable(pState->currentChanPtr,
                                       pState->currentPort->pTstin);
          printf("PCM Loopback Disabled\n");
        }
        else
        {
          ProSLIC_testInPCMLpbkEnable(pState->currentChanPtr,pState->currentPort->pTstin);
          printf("PCM Loopback Enabled\n");
        }
        break;

      case 2:
        testResult = ProSLIC_testInDCFeed(pState->currentChanPtr,
                                          pState->currentPort->pTstin);

        if(testResult == RC_TEST_PASSED)
        {
          printf("\nTEST PASSED!!\n");
        }
        else if(testResult == RC_LINE_IN_USE)
        {
          printf("\nLINE IN USE!!\n");
        }
        else
        {
          printf("\nTEST FAILED!!\n");
        }
        break;

      case 3:
        printf("TEST %s\n",
               TEST_RESULT( ProSLIC_testInRinging(pState->currentChanPtr,
                            pState->currentPort->pTstin) ));
        break;

      case 4:
        printf("TEST %s\n",
               TEST_RESULT( ProSLIC_testInBattery(pState->currentChanPtr,
                            pState->currentPort->pTstin) ));
        break;

      case 5:
        printf("TEST %s\n",
               TEST_RESULT( ProSLIC_testInAudio(pState->currentChanPtr,
                                                pState->currentPort->pTstin) ));
        break;

      case 6:
        ProSLIC_testInPrintLimits(pState->currentChanPtr, pState->currentPort->pTstin);
      case 7: /* Simulated hook state change demo code */
        {
          uInt8 preTestHookState, postTestHookState;

          /* Clear interrupts */
          irq_demo_check_interrupts(pState, &preTestHookState);

          ProSLIC_testLoadTest( pState->currentChanPtr, TRUE, 200);
          
          /* Now check hook state */
          irq_demo_check_interrupts(pState, &postTestHookState);

          if( preTestHookState != postTestHookState )
          {
            printf("Test passed - detected hook state change\n");
          }
          else
          {
            printf("Test failed- did not detect hook state change\n");
          }

          /* Restore state */
          ProSLIC_testLoadTest( pState->currentChanPtr, FALSE, 200);

        }
        break;
#endif

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);

}

