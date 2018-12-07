/*
** Copyright (c) 2013-2017 by Silicon Laboratories, Inc.
**
** $Id: tonegen.c 6480 2017-05-03 03:00:36Z nizajerk $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** tone generator resources
**
*/

#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"

#ifndef DISABLE_TONE_SETUP
/*****************************************************************************************************/
static int get_tonegen_osc_state(demo_state_t *pState)
{
  uInt8 regtmp;

  regtmp = SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_OCON);
  return ( (regtmp & 0x77) != 0);
}

/*****************************************************************************************************/
static int toggle_tonegen_osc(demo_state_t *pState)
{
  if(get_tonegen_osc_state(pState))
  {
    SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_OCON,0x00);
    return 0;
  }
  else
  {
    SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_OCON,0x77);
    return 1;
  }
}

/*****************************************************************************************************/
static void get_tonegen_osc_timers(demo_state_t *pState, int *ton, int *toff)
{
  uInt16 tmp;

  /* Read only from O1 since timers identical in this demo */
  tmp = (SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_O1TAHI)&0x00FF)<<8;
  tmp |= SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_O1TALO);
  *ton = (int)(tmp / 8);

  tmp = (SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_O1TIHI)&0x00FF)<<8;
  tmp |= SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_O1TILO);
  *toff = (int)(tmp / 8);
}

/*****************************************************************************************************/
static void set_tonegen_osc_timers(demo_state_t *pState, int ton, int toff)
{
  int tmp;
  uInt8 reg_val;

  /* Active Time */
  tmp = ton * 8;
  reg_val = (uInt8)(tmp&0x00FF);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O1TALO, reg_val);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O2TALO, reg_val);
  reg_val = (uInt8)((tmp&0xFF00)>>8);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O1TAHI, reg_val);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O2TAHI, reg_val);

  /* Inactive Time */
  tmp = toff * 8;
  reg_val = (uInt8)(tmp&0x00FF);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O1TILO, reg_val);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O2TILO, reg_val);
  reg_val = (uInt8)((tmp&0xFF00)>>8);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O1TIHI, reg_val);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_O2TIHI, reg_val);
}
/*****************************************************************************************************/
void toneGenMenu(demo_state_t *pState)
{
  int presetNum, user_selection;
  int tone_gen_timer_en = 0;
  int ton = 500;
  int toff = 500;

  const char *menu_items[] =
  {
    "Load Tone Generator Preset",
    "Enable Tone Generator",
    "Disable Tone Generator",
    "Set Tone Gen Timers",
    "Toggle Tone Gen Timers",
    NULL
  };

  do
  {
    tone_gen_timer_en = get_tonegen_osc_state(pState);
    get_tonegen_osc_timers(pState,&ton,&toff);

    presetNum =  display_menu("ToneGen Menu", menu_items);

    printf("Tone gen timers(%s): (%d ms, %d ms)\n\n",
           GET_ENABLED_STATE(tone_gen_timer_en),ton, toff);

    user_selection = get_menu_selection( presetNum, pState->currentChannel);
    printf("\n\n");

    switch( user_selection )
    {
      case 0:
        presetNum = demo_get_preset(DEMO_TONEGEN_PRESET);
        ProSLIC_ToneGenSetup(pState->currentChanPtr, presetNum);
        break;

      case 1:
        ProSLIC_ToneGenStart(pState->currentChanPtr,0);
        break;

      case 2:
        ProSLIC_ToneGenStop(pState->currentChanPtr);
        break;

      case 3:
        do
        {
          printf("Enter ToneGen Active Time (ms) %s", PROSLIC_PROMPT);
          ton = get_int(0, 0xFFFF);
        }
        while(ton > 0xFFFF);

        do
        {
          printf("Enter ToneGen Inactive Time (ms) %s", PROSLIC_PROMPT);
          toff = get_int(0, 0xFFFF);
        }
        while(toff > 0xFFFF);

        set_tonegen_osc_timers(pState,ton,toff);
        break;

      case 4:
        toggle_tonegen_osc(pState);
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);

}
#else
void toneGenMenu(demo_state_t *pState)
{
  SILABS_UNREFERENCED_PARAMETER(pState);
  printf("%sDISABLE_TONE_SETUP set in proslic_api_config.h - aborting request\n",
         LOGPRINT_PREFIX);
}
#endif

