/*
** Copyright (c) 2013-2016 by Silicon Laboratories, Inc.
**
** $Id: linefeed.c 6526 2017-05-08 19:08:33Z elgeorge $
**
** linefeed.c
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** linefeed resources
**
*/

#include "api_demo.h"
#include "proslic.h"
#include "macro.h"
#include "user_intf.h"


static const char *linefeed_states[] =
{
  "LF_OPEN",
  "LF_FWD_ACTIVE",
  "LF_FWD_OHT",
  "LF_TIP_OPEN",
  "LF_RINGING",
  "LF_REV_ACTIVE",
  "LF_REV_OHT",
  "LF_RING_OPEN",
  NULL
};

/*****************************************************************************************************/
static void changeLinefeedState(demo_state_t *state)
{
  int user_selection;

  user_selection = get_menu_selection( display_menu("Linefeed State",
                                       linefeed_states), state->currentChannel);

  if(user_selection < 8)
  {
    ProSLIC_SetLinefeedStatus(state->currentChanPtr, user_selection);
  }
}

/*****************************************************************************************************/
static void changeLinefeedStateBcast(demo_state_t *state)
{
  int user_selection;

  user_selection = get_menu_selection( display_menu("Linefeed State",
                                       linefeed_states), state->currentChannel);

  if(user_selection < 8)
  {
    ProSLIC_SetLinefeedStatusBroadcast(state->currentChanPtr, user_selection);
  }
}

/*****************************************************************************************************/
void linefeedMenu(demo_state_t *pState)
{
  const char *menu_items[] =
  {
    "Read Linefeed State",
    "Change Linefeed State",
    "Load DC Feed Preset",
    "POLREV",
    "WINK",
    "Toggle POLREV Type",
    "Toggle PWRSAVE Mode",
    "Change Linefeed State - Broadcast",
    NULL
  };

  int user_selection, presetNum;
  uInt8 regValue;
  uInt8 abrupt = 0;

  do
  {
    presetNum = display_menu("Linefeed Menu", menu_items);
    regValue = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_POLREV);
    printf("POLREV: %s %s\n",
           GET_ACTIVE_STATE(((regValue&0x06))==0x02), GET_ABRUPT_STATE(abrupt));

    printf("WINK: %s\n", GET_ACTIVE_STATE(((regValue&0x06))==0x06));

    regValue = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_ENHANCE);
    printf("Power Savings mode = %s\n\n", GET_ENABLED_STATE( (regValue & 0x10)));

    user_selection = get_menu_selection( presetNum, pState->currentChannel);
    printf("\n\n");

    switch( user_selection)
    {
      case 0:
        regValue = SiVoice_ReadReg( pState->currentChanPtr, PROSLIC_REG_LINEFEED);
        printf("LF State = %s (0x%02X)\n", linefeed_states[regValue&0x07],regValue);
        break;

      case 1:
        changeLinefeedState(pState);
        break;

      case 2:
        presetNum = demo_get_preset(DEMO_DCFEED_PRESET);
        ProSLIC_DCFeedSetup(pState->currentChanPtr, presetNum);
        break;

      case 3:
        regValue = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_POLREV);
        if ( regValue & 2 )
        {
          printf("POLREV STOP\n");
          ProSLIC_PolRev(pState->currentChanPtr, abrupt, POLREV_STOP);
        }
        else
        {
          printf("POLREV START\n");
          ProSLIC_PolRev(pState->currentChanPtr, abrupt, POLREV_START);
        }
        break;

      case 4:
        regValue = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_POLREV);
        if ( (regValue&0x06) == 0x06)
        {
          printf("WINK STOP\n");
          ProSLIC_PolRev(pState->currentChanPtr, abrupt, WINK_STOP);
        }
        else
        {
          printf("WINK START\n");
          ProSLIC_PolRev(pState->currentChanPtr, abrupt, WINK_START);
        }
        break;

      case 5:
        abrupt = (abrupt == 0);
        break;

      case 6:
        if(SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_ENHANCE)&0x10)
        {
          ProSLIC_SetPowersaveMode(pState->currentChanPtr, PWRSAVE_DISABLE);
        }
        else
        {
          ProSLIC_SetPowersaveMode(pState->currentChanPtr, PWRSAVE_ENABLE);
        }
        break;

      case 7:
        changeLinefeedStateBcast(pState);
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);

}

