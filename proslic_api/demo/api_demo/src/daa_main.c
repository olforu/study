/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: daa_main.c 5659 2016-05-16 16:15:59Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the DAA main menu & support functions.
**
*/

#include "api_demo.h"
#include "user_intf.h"
#include "vdaa.h"
#include "vdaa_registers.h"
#include "macro.h"

#define PPS_TIME 100 /* How long is a pulse in terms of mSec - 10 PPS */

extern void daa_ToggleHookState(demo_state_t *state);

/*****************************************************************************************************/
/* break ratio = %of being onhook, interdigit delay = mSec between digits */
static void pulse_dial(demo_state_t *pState, char *digits, uInt8 break_ratio,
                       int interdigit_delay)
{
  char *cPtr;
  int makeDelay = ((100-break_ratio) * PPS_TIME)/100;
  int breakDelay = (break_ratio * PPS_TIME)/100;
  int i,digit;
  uInt8 regData;

  printf("%sReseting line state - going onhhok and then offhook for 1s each\n",
         LOGPRINT_PREFIX);
  Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_ONHOOK);
  Delay(pProTimer,1000);
  Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_OFFHOOK);
  regData = SiVoice_ReadReg(pState->currentChanPtr, RES_CALIB);
  SiVoice_WriteReg(pState->currentChanPtr, RES_CALIB,
                   (regData & ~(1<<5))); /* Disable resistor calibration */
  Delay(pProTimer,1000);

  printf("%sMake time = %d, Break time = %d, Interdigit delay = %d\n",
         LOGPRINT_PREFIX,
         makeDelay, breakDelay, interdigit_delay);

  for(cPtr = digits; *cPtr; cPtr++)
  {
    digit = *cPtr - '0';
    if(digit == 0)
    {
      digit = 10;
    }
    printf("%sDialing %d\n", LOGPRINT_PREFIX, digit);
    for(i = 0; i < digit; i++)
    {
      Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_ONHOOK);
      Delay(pProTimer, breakDelay);
      Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_OFFHOOK);
      Delay(pProTimer, makeDelay);
    }
    Delay(pProTimer, interdigit_delay);
  }
  SiVoice_WriteReg(pState->currentChanPtr, RES_CALIB,
                   regData ); /* Restore resistor calibration */
}

/*****************************************************************************************************/
static void daa_pulse_dial(demo_state_t *pState)
{
  const char *menu_items[] =
  {
    "Set pulse dial timing values",
    "Show pulse dial values",
    "Enter dial string",
    "Pulse Dial",
    "Set hook flash time",
    "Hook flash",
    "Toggle Hook State",
    NULL
  };

  int break_ratio = 60;
  int interdigitTime = 700;
  int hook_flash = 200;
  int user_selection;
  char dialString[80];
  uInt8 regData;

  /* Set the dial string to 12 for a default */
  dialString[0] = '1';
  dialString[1] = '2';
  dialString[2] =  0;

  do
  {
    user_selection = get_menu_selection( display_menu("Pulse Dial Menu",
                                         menu_items), pState->currentChannel);
    switch(user_selection)
    {
      case 0:
        do
        {
          printf("Enter break/make ratio (10-90%%) %s",PROSLIC_PROMPT);
          break_ratio = get_int(10,90);
        }
        while(break_ratio > 90);

        do
        {
          printf("Enter interdigit time (10-900 mSec) %s",PROSLIC_PROMPT);
          interdigitTime = get_int(10,900);
        }
        while(interdigitTime > 900);
        break;

      case 1:
        printf("%sBreak/Make ratio: %d %%\n", LOGPRINT_PREFIX, break_ratio);
        printf("%sInterdigit delay: %d mSec\n", LOGPRINT_PREFIX, interdigitTime);
        printf("%sDial string = %s\n", LOGPRINT_PREFIX, dialString);
        break;

      case 2:
        printf("Enter dial string %s", PROSLIC_PROMPT);
        scanf("%s", dialString);
        break;

      case 3:
        pulse_dial(pState, dialString, break_ratio, interdigitTime);
        break;

      case 4:
        do
        {
          printf("Enter hook flash time (10-900 mSec) %s",PROSLIC_PROMPT);
          hook_flash = get_int(10,900);
        }
        while(hook_flash >900);
        break;

      case 5:
        Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_OFFHOOK);
        regData = SiVoice_ReadReg(pState->currentChanPtr, RES_CALIB);
        SiVoice_WriteReg(pState->currentChanPtr, RES_CALIB,
                         (regData & ~(1<<5))); /* Disable resistor calibration */
        Delay(pProTimer,1000);
        Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_ONHOOK);
        Delay(pProTimer,hook_flash);
        SiVoice_WriteReg(pState->currentChanPtr, RES_CALIB,
                         regData ); /* Restore resistor calibration */
        Vdaa_SetHookStatus(pState->currentChanPtr, VDAA_OFFHOOK);
        break;

      case 6:
        daa_ToggleHookState(pState);
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

/*****************************************************************************************************/
void daa_main_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Select Channel",
    "Debug Menu",
    "Audio Menu",
    "Change Country Preset",
    "Change Ring Validation Preset",
    "Report Ring Detect Status",
    "Pulse dial Menu",
    "Line state Menu",
    NULL
  };

  int user_selection, aux_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("DAA Menu", menu_items),
                                         state->currentChannel);

    switch(user_selection)
    {
      case 0: /* Change channel */
        change_channel(state);
        if( state->currentChanPtr->channelType != DAA)
        {
          return;
        }
        break;

      case 1: /* Debug menu */
        debugMenu(state);
        break;

      case 2: /* Audio menu */
        daa_audio_menu(state);
        break;

      case 3:
        aux_selection = demo_get_preset(DEMO_VDAA_COUNTRY_PRESET);
        Vdaa_CountrySetup(state->currentChanPtr, aux_selection);
        break;

      case 4:
        aux_selection = demo_get_preset(DEMO_VDAA_RING_VALIDATION_PRESET);
        Vdaa_RingDetectSetup(state->currentChanPtr, aux_selection);
        break;

      case 5:
        {
          vdaaRingDetectStatusType ringDetectState;

          Vdaa_ReadRingDetectStatus(state->currentChanPtr, &ringDetectState);

          printf("%soffhook = %s\tRing Detected: %s\tOnhookLine monitor: %s\n",
                 LOGPRINT_PREFIX,
                 GET_BOOL_STATE(ringDetectState.offhook),
                 GET_BOOL_STATE(ringDetectState.ringDetected),
                 GET_BOOL_STATE(ringDetectState.onhookLineMonitor));

          printf("%sRing Detect Signal Positive: %s\tRing Detect Signal Negative:%s\n",
                 LOGPRINT_PREFIX,
                 GET_BOOL_STATE(ringDetectState.ringDetectedPos),
                 GET_BOOL_STATE(ringDetectState.ringDetectedNeg));
        }
        break;

      case 6:
        daa_pulse_dial(state);
        break;

      case 7:
        daa_linestate_menu(state);
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

