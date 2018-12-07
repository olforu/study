/*
** Copyright (c) 2013-2017 by Silicon Laboratories, Inc.
**
** $Id: ringing.c 6480 2017-05-03 03:00:36Z nizajerk $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** ringing resources
**
*/

#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"

#define MAX_RING_TIMERS 6

typedef struct
{
  int onTime; /* 10mSec ticks */
  int offTime; /* 10mSec ticks */
} ringOnOff_t;
#ifndef DISABLE_RING_SETUP
extern  char *intMapStrings[];

/*****************************************************************************************************/
/* Make sure the ProSLIC isn't in transition from one linefeed state to another.  This will block
   until it is done.
*/
static void chk_safeLineFeedStateChange(SiVoiceChanType_ptr chanPtr)
{
  uInt8 regValue;

  do
  {
    regValue = SiVoice_ReadReg(chanPtr, PROSLIC_REG_LINEFEED);
  } while( (regValue & 0x7) != ( (regValue >> 4) & 0x7) );

}

/*****************************************************************************************************/
static void distinctive_ringing(demo_state_t *pState, ringOnOff_t *ringTimers,
                                int num_ringphases)
{
  int ringPhase=0;
  int inOnPhase = 1;
  int timerTick=0; /* 10Msec poll ticks */
  uInt8 oldIRQEN1, oldIRQEN2;
  proslicIntType irqs;
  ProslicInt arrayIrqs[MAX_PROSLIC_IRQS];
  irqs.irqs = arrayIrqs;

  /* Clear interrupts */
  ProSLIC_GetInterrupts(pState->currentChanPtr, &irqs);

  /* For this demo to work, we need to disable the 2 Ringer interrupts - RING_TA & RING_TI */
  oldIRQEN1 = SiVoice_ReadReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1));
  SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1),
                   (oldIRQEN1 & 0xCF) );

  /* Save current IRQEN2 state & enable ring trip and loop closure interrupts */
  oldIRQEN2 = SiVoice_ReadReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1+1));
  SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1+1),
                   (oldIRQEN2|3) );

  ProSLIC_SetLinefeedStatus(pState->currentChanPtr, LF_RINGING);

  do
  {
    ProSLIC_GetInterrupts(pState->currentChanPtr, &irqs);

    if(!irqs.number)
    {
      if( inOnPhase)
      {
        if(timerTick == ringTimers[ringPhase].onTime)
        {
          chk_safeLineFeedStateChange(pState->currentChanPtr);
          ProSLIC_SetLinefeedStatus(pState->currentChanPtr, LF_FWD_ACTIVE);

          /* The ProSLIC_SetLinefeedStatus restores what was in the general config,
             restore what we need in the demo...
          */
          SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1),
                   (oldIRQEN1 & 0xCF) );
          SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1+1),
                   (oldIRQEN2|3) );
          /* Brute force to clear IRQ1 - for ring timer */
          (void)SiVoice_ReadReg(pState->currentChanPtr, PROSLIC_REG_IRQ1);

          timerTick = 0;
          inOnPhase = 0;
        }
      }
      else
      {
        if(timerTick == ringTimers[ringPhase].offTime)
        {
          chk_safeLineFeedStateChange(pState->currentChanPtr);
          ProSLIC_SetLinefeedStatus(pState->currentChanPtr, LF_RINGING);

          timerTick = 0;
          inOnPhase = 1;
          ringPhase++;

          if(ringPhase == num_ringphases)
          {
            ringPhase = 0;
          }
        }
      }
    }

    Delay(pProTimer, 10);
    timerTick++;

  }
  while(irqs.number == 0);

  ProSLIC_SetLinefeedStatus(pState->currentChanPtr, LF_FWD_ACTIVE);

  printf("%s:interrupt(s) detected: %d\n", LOGPRINT_PREFIX, irqs.number);

  /* We are reusing ring phase for a counter here.. */
  for(ringPhase = 0; ringPhase < irqs.number; ringPhase++)
  {
    printf("%sinterupt(%d) = %s\n", LOGPRINT_PREFIX, ringPhase,
      intMapStrings[ irqs.irqs[ringPhase] ] );
  }

  /* Make sure we've cleared all the interrupts */
  ProSLIC_GetInterrupts(pState->currentChanPtr, &irqs);

  /* Restore registers */
  SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1), oldIRQEN1 );
  SiVoice_WriteReg(pState->currentChanPtr, (PROSLIC_REG_IRQEN1+1), oldIRQEN2 );

}

/*****************************************************************************************************/
static int get_ring_osc_state(demo_state_t *pState)
{
  uInt8 regtmp;

  regtmp = SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGCON);
  return( ((regtmp &0x18) != 0) );
}

/*****************************************************************************************************/
static void get_ring_osc_timers(demo_state_t *pState, int *ton, int *toff)
{
  uInt16 tmp;

  tmp = (SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGTAHI)&0x00FF)<<8;
  tmp |= SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGTALO);
  *ton = (int)(tmp / 8);

  tmp = (SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGTIHI)&0x00FF)<<8;
  tmp |= SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGTILO);
  *toff = (int)(tmp / 8);
}

/*****************************************************************************************************/
static void set_ring_osc_timers(demo_state_t *pState, int ton, int toff)
{
  int tmp;
  uInt8 reg_val;

  /* Active Time */
  tmp = ton * 8;
  reg_val = (uInt8)(tmp&0x00FF);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_RINGTALO, reg_val);
  reg_val = (uInt8)((tmp&0xFF00)>>8);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_RINGTAHI, reg_val);

  /* Inactive Time */
  tmp = toff * 8;
  reg_val = (uInt8)(tmp&0x00FF);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_RINGTILO, reg_val);
  reg_val = (uInt8)((tmp&0xFF00)>>8);
  SiVoice_WriteReg(pState->currentChanPtr, PROSLIC_REG_RINGTIHI, reg_val);
}

/*****************************************************************************************************/
static int toggle_ring_osc(demo_state_t *pState)
{
  uInt8 regtmp;

  regtmp = SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGCON);
  if(regtmp & 0x18)
  {
    SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_RINGCON,regtmp&~0x18);
    return 1;
  }
  else
  {
    SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_RINGCON,regtmp|0x18);
    return 0;
  }
}

/*****************************************************************************************************/
void ringingMenu(demo_state_t *pState)
{
  const char *menu_items[] =
  {
    "Start Ringing",
    "Stop Ringing",
    "Load Ringing Preset",
    "Toggle Ring Cadence Timers",
    "Set Ringing Cadence",
    "Distinctive Ringing setup",
    "Distinctive Ring",
    "Enable/disable fast ring start",
    NULL
  };

  int ton = 2000;
  int toff = 4000;
  uInt8 ring_osc_en = 0;
  int user_selection, presetNum, num_items;
  ringOnOff_t ringTimers[MAX_RING_TIMERS];
  int ringPhases=0;
  int i;

  do
  {
    ring_osc_en = get_ring_osc_state(pState);
    get_ring_osc_timers(pState,&ton,&toff);
    num_items = display_menu("Ringing Menu", menu_items);

    printf("Ring Cadence Timers (%s)\n", GET_ENABLED_STATE(ring_osc_en));
    if(ring_osc_en)
    {
      printf("Ringing Cadence: (%d ms, %d ms)\n",ton,toff);
    }
    printf("\n");
    user_selection = get_menu_selection( num_items, pState->currentChannel);

    switch(user_selection)
    {
      case 0:
        ProSLIC_RingStart(pState->currentChanPtr);
        break;

      case 1:
        ProSLIC_RingStop(pState->currentChanPtr);
        break;

      case 2:
        presetNum = demo_get_preset(DEMO_RING_PRESET);
        ProSLIC_RingSetup(pState->currentChanPtr, presetNum);
        break;

      case 3:
        toggle_ring_osc(pState);
        break;

      case 4:
        ton = get_prompted_int(0, 0xFFFF, "Enter Ring Active Time (ms)");
        toff = get_prompted_int(0, 0xFFFF, "Enter Ring Inactive Time (ms)");
        set_ring_osc_timers(pState,ton,toff);
        break;

      case 5:
        ringPhases = get_prompted_int(1,MAX_RING_TIMERS,
                                      "Please enter number of on/off ring phases (1-6)");

        for(i = 0; i < ringPhases; i++)
        {
          printf("For ring phase %d ", i);
          ringTimers[i].onTime = get_prompted_int(1,800, "enter on time (10mSec ticks)");
          printf("For ring phase %d ", i);
          ringTimers[i].offTime = get_prompted_int(1,800,
                                  "enter off time (10mSec ticks)");
        }
        break;

      case 6:
        if(ringPhases == 0)
        {
          printf("%sYou must setup ring cadence first!\n", LOGPRINT_PREFIX);
        }
        else
        {
          printf("\n%sGo off hook to abort ringing\n\n", LOGPRINT_PREFIX);
          for(i=0; i < ringPhases; i++)
          {
            printf("%sphase:%d on: %d off: %d\n", LOGPRINT_PREFIX, i, ringTimers[i].onTime,
                   ringTimers[i].offTime);
          }
          distinctive_ringing(pState, ringTimers, ringPhases);
        }
        break;

      case 7:
        {
          int portIndex = pState->currentChannel - pState->currentPort->channelBaseIndex;
          pState->currentPort->demo_info[portIndex].isFRS_enabled = 
            (pState->currentPort->demo_info[portIndex].isFRS_enabled == 0);

          printf("%sFast ring start is now : %s\n", LOGPRINT_PREFIX, 
            GET_ENABLED_STATE(pState->currentPort->demo_info[portIndex].isFRS_enabled) );
          ProSLIC_EnableFastRingStart(pState->currentChanPtr, (pState->currentPort->demo_info[portIndex].isFRS_enabled));
        }
        break;

      default:
        break;

    }
  }
  while(user_selection != QUIT_MENU);
}
#else
void ringingMenu(demo_state_t *pState)
{
  SILABS_UNREFERENCED_PARAMETER(pState);
  printf("%sDISABLE_RING_SETUP set in proslic_api_config.h - aborting request\n",
         LOGPRINT_PREFIX);
}
#endif

