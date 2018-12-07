/*
** Copyright (c) 2013-2017 by Silicon Laboratories, Inc.
**
** $Id: audio.c 6477 2017-05-03 02:19:47Z nizajerk $
**
** audio.c
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** audio resources
**
*/

#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"

#ifndef DISABLE_FSK_SETUP
/*****************************************************************************************************/
static uInt8 checkSum(uInt8 *str)
{
  int i=0;
  uInt8 sum = 0;

  while(str[i] != 0)
  {
    sum += str[i++];
  }

  return -sum;
}

/*****************************************************************************************************/
/* Wait for FSK buffer to be available... */
static void waitForCIDDone(SiVoiceChanType_ptr pChan)
{
  uInt8 tmp = 0;

  do
  {
    ProSLIC_CheckCIDBuffer(pChan, &tmp);
  }
  while(tmp == 0);

}

/*****************************************************************************************************/
static void fskSetup(demo_state_t *pState)
{
  ProSLIC_FSKSetup(pState->currentChanPtr,0);
}
#define FSK_DEPTH_TRIG 7 /* This is 7-FSKBUFDEPTH = number of bytes we can safely send after getting
                            and interrupt that the buffer is "empty"
                         */
/*****************************************************************************************************/
static void sendFSKData(demo_state_t *pState, uInt8 *stream, int preamble_enable)
{
  int i;
  uInt8 *cptr;

  uInt8 cid_preamble[] = {'U','U','U','U','U','U','U','U'};

  ProSLIC_EnableCID(pState->currentChanPtr);
  Delay(pProTimer,133);   /* > 130ms of mark bits */

  /* Enable FSKBUF interrupt so we can check it later. */
  demo_save_slic_irqens(pState); 
  SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_IRQEN1,0x40);
  (void)SiVoice_ReadReg(pState->currentChanPtr,
                        PROSLIC_REG_IRQ1); /* Clear IRQ1 */

  if(preamble_enable)
  {
    /* Send preamble */
    for(i=0; i<30; i+=FSK_DEPTH_TRIG)
    {
      if(i >= 8) /* The FIFO depth is 8 bytes, start waiting for it to empty */
      {
        waitForCIDDone(pState->currentChanPtr);
      }
      ProSLIC_SendCID(pState->currentChanPtr, cid_preamble, FSK_DEPTH_TRIG);
    }
    if (30%FSK_DEPTH_TRIG)
    {
      waitForCIDDone(pState->currentChanPtr);
    }
    ProSLIC_SendCID(pState->currentChanPtr,cid_preamble,30%FSK_DEPTH_TRIG);
    waitForCIDDone(pState->currentChanPtr);

    /* Delay > 130ms for idle mark bits */
    Delay(pProTimer,133);
  }

  stream[stream[1]+2] = 0;
  stream[stream[1]+2] = checkSum(stream);
  cptr = stream;

  /* At this point the FIFO buffer is empty, fill it with the maximum of 8 bytes. 
     The +3 is for header, checksum + mark bits 
   */
  ProSLIC_SendCID(pState->currentChanPtr, cptr, SI_MIN(stream[1]+3, 8));
  cptr += SI_MIN(stream[1]+3, 8);

  /* now loop until we've sent all the data.  */
  for(i = stream[1]-3; i>0; i-= FSK_DEPTH_TRIG)
  {
    if( i >= FSK_DEPTH_TRIG )
    {
      /* Since we filled the CID FIFO buffer earlier, wait for it to become "free" to put more data*/
      waitForCIDDone(pState->currentChanPtr);
      ProSLIC_SendCID(pState->currentChanPtr, cptr, FSK_DEPTH_TRIG);
      cptr += FSK_DEPTH_TRIG;
    }
    else
  {
      ProSLIC_SendCID(pState->currentChanPtr, cptr, i);
    }   
  }

  /* Make sure the last byte is shifted out prior to disabling CID.
   * We're assuming 8 10bit values @ 1200 BPS, rounded up, 10 mSec
   * resolution.
   */
  Delay(pProTimer, 70);
  ProSLIC_DisableCID(pState->currentChanPtr);
  demo_restore_slic_irqens(pState); 

}

/*****************************************************************************************************/
/*
** Sequential (blocking) example of CID transmission
*/
static void sendCIDStream(demo_state_t *pState)
{

  uInt8 cid_msg[] =
    "\x80"           /* MDMF Type */
    "\x27"           /* Message Length */
    "\x01"           /* Date/Time Param */
    "\x08"           /* 8-byte Date/Time */
    "07040815"       /* July 4th 08:15 am */
    "\x02"           /* Calling Number Param */
    "\x0A"           /* 10-byte Calling Number */
    "5124168500"     /* Phone Number */
    "\x07"           /* Calling Name Param */
    "\x0F"           /* 15-byte Calling Name */
    "Nice"           /* Calling Name */
    "\x20"           /* Calling Name (whitespace) */
    "ProSLIC!!!"     /* Calling Name */
    "\x00"           /* Placeholder for Checksum */
    "\x00"           /* Markout */
    ;
  uInt8 reg_tmp;

  fskSetup(pState);

  ProSLIC_RingSetup(pState->currentChanPtr,1);
  reg_tmp =SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_RINGCON);
  SiVoice_WriteReg(pState->currentChanPtr,PROSLIC_REG_RINGCON,reg_tmp&0xF0);

  /* Ensure OFFHOOK Active */
  ProSLIC_SetLinefeedStatus(pState->currentChanPtr,LF_FWD_ACTIVE);
  Delay(pProTimer,500);

  /* 1st Ring Burst */
  ProSLIC_SetLinefeedStatus(pState->currentChanPtr,LF_RINGING);
  Delay(pProTimer,2000);

  /* OHT - the alternative is to have the configuration for ringing set OHT mode automatically... */
  ProSLIC_SetLinefeedStatus(pState->currentChanPtr,LF_FWD_OHT);
  Delay(pProTimer,1400);   /* Delay 250 to 3600ms */
  sendFSKData(pState, cid_msg, 1);

  /* Restore to normal operational mode */
  ProSLIC_SetLinefeedStatus(pState->currentChanPtr,LF_FWD_ACTIVE);
}

/*****************************************************************************************************/
/*
** Sequential (blocking) example of VMWI transmission
*/
void sendVMWIStream(demo_state_t *pState,int enable_flag)
{
  uInt8 *vmwi_msg;

  // VMWI Message
  uInt8 vmwi_msg_enable[] =
    "\x06"           /* SDMF VMWI Type */
    "\x03"           /* Message Length (always 3 for SDMF VMWI) */
    "\x42"           /* VMWI On */
    "\x42"           /* VMWI On */
    "\x42"           /* VMWI On */
    "\x00"           /* Placeholder for Checksum */
    "\x00"           /* Markout */
    ;

  uInt8 vmwi_msg_disable[] =

    "\x06"           /* SDMF VMWI Type */
    "\x03"           /* Message Length (always 3 for SDMF VMWI) */
    "\x6f"           /* VMWI Off */
    "\x6f"           /* VMWI Off */
    "\x6f"           /* VMWI Off */
    "\x00"           /* Placeholder for Checksum */
    "\x00"           /* Markout */
    ;

  if(enable_flag)
  {
    vmwi_msg = vmwi_msg_enable;
  }
  else
  {
    vmwi_msg = vmwi_msg_disable;
  }

  fskSetup(pState);

  /* OHT */
  ProSLIC_SetLinefeedStatus(pState->currentChanPtr,LF_FWD_OHT);
  Delay(pProTimer,1400);   /* Delay 250 to 3600ms (1400) */

  sendFSKData(pState, vmwi_msg, 1);
}
#endif

/*****************************************************************************************************/
void audioMenu(demo_state_t *pState)
{
  int presetNum;
  int32 rxgain, txgain;
  uInt16 rxcount, txcount;
  int user_selection;
  const char *menu_items[] =
  {
    "Change Audio Gains",
    "Load Impedance Preset",
    "Send Caller ID Stream",
    "Send VMWI Enable",
    "Send VMWI Disable",
    "Load PCM Preset",
    "Set PCM RX/TX timeslots",
    "Enable PCM Bus",
    "Disable PCM Bus",
    "Print FSK settings",
    NULL
  };

  do
  {
    user_selection = get_menu_selection( display_menu("Audio Menu", menu_items),
                                         pState->currentChannel);
    printf("\n\n");

    switch( user_selection )
    {
      case 0:
        presetNum = demo_get_preset( DEMO_IMPEDANCE_PRESET );
        ProSLIC_ZsynthSetup(pState->currentChanPtr,presetNum);

#ifdef ENABLE_HIRES_GAIN
        do
        {
          printf("Enter Desired RX Gain (%d - %d(0.1 dB) %s", PROSLIC_GAIN_MIN*10,
                 PROSLIC_GAIN_MAX*10, PROSLIC_PROMPT);
          rxgain = get_int(PROSLIC_GAIN_MIN*10, PROSLIC_EXTENDED_GAIN_MAX*10);
        }
        while(rxgain > PROSLIC_EXTENDED_GAIN_MAX*10);

        do
        {
          printf("Enter Desired TX Gain (%d - %d(0.1 dB) %s", PROSLIC_GAIN_MIN*10,
                 PROSLIC_GAIN_MAX*10, PROSLIC_PROMPT);
          txgain = get_int(PROSLIC_GAIN_MIN*10, PROSLIC_EXTENDED_GAIN_MAX*10);
        }
        while(txgain > PROSLIC_EXTENDED_GAIN_MAX*10);

        ProSLIC_AudioGainSetup(pState->currentChanPtr,rxgain,txgain,presetNum);
        printf("\nRX Gain = %d (0.1 dB)\n", rxgain);
        printf("TX Gain = %d (0.1 dB)\n", txgain);
#else
        do
        {
          printf("Enter Desired RX Gain (%d - %d(dB) %s", PROSLIC_GAIN_MIN,
                 PROSLIC_GAIN_MAX, PROSLIC_PROMPT);
          rxgain = get_int(PROSLIC_GAIN_MIN, PROSLIC_EXTENDED_GAIN_MAX);
        }
        while(rxgain > PROSLIC_EXTENDED_GAIN_MAX);

        do
        {
          printf("Enter Desired TX Gain (%d - %d(dB) %s", PROSLIC_GAIN_MIN,
                 PROSLIC_GAIN_MAX, PROSLIC_PROMPT);
          txgain = get_int(PROSLIC_GAIN_MIN, PROSLIC_EXTENDED_GAIN_MAX);
        }
        while(txgain > PROSLIC_EXTENDED_GAIN_MAX);

        ProSLIC_AudioGainSetup(pState->currentChanPtr,rxgain,txgain,presetNum);
        printf("\nRX Gain = %d dB\n", (int)rxgain);
        printf("TX Gain = %d dB\n", (int)txgain);
#endif

        printf("Preset = %d\n", presetNum);
        break;

      case 1:
        presetNum = demo_get_preset( DEMO_IMPEDANCE_PRESET );
        ProSLIC_ZsynthSetup(pState->currentChanPtr, presetNum);
        break;

      case 2:
#ifdef DISABLE_FSK_SETUP
        printf("%sFSK_SETUP Disabled in proslic_api_config.h- aborting request\n",
               LOGPRINT_PREFIX);
#else
        printf("%sSending CID Stream...\n", LOGPRINT_PREFIX);
        sendCIDStream(pState);
#endif
        break;

      case 3:
#ifdef DISABLE_FSK_SETUP
        printf("%sFSK_SETUP Disabled in proslic_api_config.h- aborting request\n",
               LOGPRINT_PREFIX);
#else
        printf("%sSending VMWI Enable...\n", LOGPRINT_PREFIX);
        sendVMWIStream(pState,1);
#endif
        break;

      case 4:
#ifdef DISABLE_FSK_SETUP
        printf("%sFSK_SETUP Disabled in proslic_api_config.h- aborting request\n",
               LOGPRINT_PREFIX);
#else
        printf("%sSending VMWI Disable...\n", LOGPRINT_PREFIX);
        sendVMWIStream(pState,0);
#endif
        break;

      case 5:
#ifdef DISABLE_PCM_SETUP
        printf("%sFSK_SETUP Disabled in proslic_api_config.h- aborting request\n",
               LOGPRINT_PREFIX);
#else
        presetNum = demo_get_preset( DEMO_PCM_PRESET );
        ProSLIC_PCMSetup(pState->currentChanPtr,presetNum);
#endif
        break;

      case 6:
        do
        {
          printf("Enter RX Count %s ",PROSLIC_PROMPT);
          rxcount = get_int(0,0x3FF);
        }
        while(rxcount > 0x3FF);

        do
        {
          printf("Enter TX Count %s ",PROSLIC_PROMPT);
          txcount = get_int(0,0x3FF);
        }
        while(txcount > 0x3FF);

        ProSLIC_PCMTimeSlotSetup(pState->currentChanPtr,rxcount,txcount);
        break;

      case 7:
        printf("Starting PCM Bus\n");
        ProSLIC_PCMStart(pState->currentChanPtr);
        break;

      case 8:
        printf("%sStopping PCM Bus\n", LOGPRINT_PREFIX);
        ProSLIC_PCMStop(pState->currentChanPtr);
        break;

      case 9:
#ifndef DISABLE_FSK_SETUP
        printf("%sBytes sent per interrupt: %d\n", LOGPRINT_PREFIX,
          FSK_DEPTH_TRIG);
        printf("%sFSK buf depth: %u\n", LOGPRINT_PREFIX,
          ProSLIC_ReadReg(pState->currentChanPtr, PROSLIC_REG_FSKDEPTH));
#else
        printf("%sFSK not enabled\n", LOGPRINT_PREFIX);
#endif
        break;

      default:
        break;

    }
  }
  while(user_selection != QUIT_MENU);
}

