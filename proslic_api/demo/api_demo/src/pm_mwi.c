/*
** Copyright (c) 2013-2017 by Silicon Laboratories, Inc.
**
** $Id: pm_mwi.c 6501 2017-05-04 20:02:32Z elgeorge $
**
** pm_mwi.c
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** pulse metering and message waiting resources
**
*/
#include "api_demo.h"
#include "proslic.h"
#include "macro.h"
#include "user_intf.h"


/*****************************************************************************************************/
#ifndef DISABLE_PULSEMETERING
static int get_pm_state(demo_state_t *pState)
{
  return (SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_PMCON) & 0x1);
}
#endif

#ifdef SIVOICE_NEON_MWI_SUPPORT
static int get_mwi_state(demo_state_t *pState)
{
  return (SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_USERSTAT) & 0x4);
}
#endif

/*****************************************************************************************************/
void pmMwiMenu(demo_state_t *pState)
{
  const char *menu_items[] =
  {
    "MWI Setup",
    "Toggle MWI Enable",
    "Toggle MWI State",
    "Set MWI Toggle Interval",
    "Run Auto MWI",
    "Run Auto RAMP MWI",
    "Load Pulse Metering Preset",
    "Toggle Pulse metering Enable",
    "Start Pulse Metering",
    "Stop Pulse Metering",
    NULL
  };

  int user_selection, presetNum;
  uInt8 val;
  int vpk = 85;
  int mask_time = 70;
  int mwi_toggle_interval = 1000;  /* ms */
  uInt8 mwi_enable = 0;
  uInt8 mwi_state = 0;
  uInt8 metering_enable = 0;
  
  do
  {
#ifndef DISABLE_PULSEMETERING
    metering_enable = get_pm_state(pState);
#endif
#ifdef SIVOICE_NEON_MWI_SUPPORT
    mwi_enable = get_mwi_state(pState);
#endif
    presetNum =  display_menu("MWI & Pulse Metering Menu", menu_items);
    printf("MWI setup: (%d v, mask: %d ms interval: %d ms) %s %s\n", vpk, mask_time,
           mwi_toggle_interval,GET_ENABLED_STATE(mwi_enable), GET_ACTIVE_STATE(mwi_state));
    printf("Pulse metering: %s\n\n", GET_ENABLED_STATE(metering_enable));
    user_selection = get_menu_selection( presetNum, pState->currentChannel);
    printf("\n\n");

    switch( user_selection)
    {
#ifdef SIVOICE_NEON_MWI_SUPPORT
      case 0:
        do
        {
          printf("Enter MWI Vpk (%d-%d v) %s", SIL_MWI_VPK_MIN,SIL_MWI_VPK_MAX,
                 PROSLIC_PROMPT);
          vpk = get_int(SIL_MWI_VPK_MIN, SIL_MWI_VPK_MAX);
        }
        while(vpk > SIL_MWI_VPK_MAX);
        do
        {
          printf("Enter MWI Mask Time (%d - %d ms) %s", SIL_MWI_LCRMASK_MIN,
                 SIL_MWI_LCRMASK_MAX, PROSLIC_PROMPT );
          mask_time = get_int(SIL_MWI_LCRMASK_MIN, SIL_MWI_LCRMASK_MAX);
        }
        while(mask_time > SIL_MWI_LCRMASK_MAX);
        ProSLIC_MWISetup(pState->currentChanPtr,vpk,mask_time);
        break;

      case 1:
        if(mwi_enable)
        {
          ProSLIC_MWIDisable(pState->currentChanPtr);
          mwi_enable = 0;
        }
        else
        {
          ProSLIC_MWIEnable(pState->currentChanPtr);
          mwi_enable = 1;
        }
        break;

      case 2:
        val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_LINEFEED);
        if(val == 0x11)
        {
          ProSLIC_SetMWIState(pState->currentChanPtr,SIL_MWI_FLASH_ON);
          mwi_state = 1;
        }
        else
        {
          ProSLIC_SetMWIState(pState->currentChanPtr,SIL_MWI_FLASH_OFF);
          mwi_state = 0;
        }
        break;

      case 3:
        do
        {
          printf("Enter MWI Interval (ms) %s",PROSLIC_PROMPT);
          mwi_toggle_interval = get_int(0, 0xFFFF);
        } while(mwi_toggle_interval > 0xFFFF);
        break;

      case 4:
        printf("\n\nHit Enter to Stop MWI\n\n");
        ProSLIC_MWIEnable(pState->currentChanPtr);
        val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_LCRRTP);

        while(!(kbhit())&&!val)
        {
          ProSLIC_SetMWIState(pState->currentChanPtr,SIL_MWI_FLASH_ON);
          printf("%sMWI Flash OFF\n", LOGPRINT_PREFIX);
          Delay(pProTimer,mwi_toggle_interval);
          ProSLIC_SetMWIState(pState->currentChanPtr,SIL_MWI_FLASH_OFF);
          printf("%sMWI Flash ON\n", LOGPRINT_PREFIX);
          Delay(pProTimer,mwi_toggle_interval);
          val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_LCRRTP);
        }
        ProSLIC_MWIDisable(pState->currentChanPtr);
        (void)getchar();
        break;
        
      case 5:
        printf("\n\nHit Enter to Stop MWI\n\n");
        ProSLIC_MWIEnable(pState->currentChanPtr);
        val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_LCRRTP);
        
        ProSLIC_MWIRampStart(pState->currentChanPtr,10,100,100);
        
        while(!(kbhit())&&!val)
        {
          ProSLIC_MWIRampPoll(pState->currentChanPtr);
          Delay(pProTimer,5);
          val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_LCRRTP);
        }
        
        ProSLIC_MWIRampStop(pState->currentChanPtr);
        ProSLIC_MWIDisable(pState->currentChanPtr);
        (void)getchar();
        break;
#else
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
        printf("%s: feature disabled in proslic_api_config.h\n", LOGPRINT_PREFIX);
        break;
#endif /* MWI */
#ifndef DISABLE_PULSEMETERING
      case 6:
        presetNum = demo_get_preset(DEMO_PM_PRESET);
        ProSLIC_PulseMeterSetup(pState->currentChanPtr,presetNum);
        break;

      case 7:
        if(get_pm_state(pState))
        {
          ProSLIC_PulseMeterDisable(pState->currentChanPtr);
        }
        else
        {
          ProSLIC_PulseMeterEnable(pState->currentChanPtr);
          val = SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_PMCON);
          if(!(val&0x01))
          {
            printf("\nPulse Metering not supported\n");
          }
        }
        break;

      case 8:
        ProSLIC_PulseMeterStart(pState->currentChanPtr);
        break;

      case 9:
        ProSLIC_PulseMeterStop(pState->currentChanPtr);
        break;
#else
      case 6:
      case 7:
      case 8:
      case 9:
        printf("%s: feature disabled in proslic_api_config.h\n", LOGPRINT_PREFIX);
        break;
#endif /* PULSE metering */

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

