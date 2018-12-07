/*
** Copyright (c) 2015-2017 by Silicon Laboratories
**
** $Id: demo_common.c 7106 2018-04-20 00:18:02Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** Various utility functions that are common for the ProSLIC API demos...
**
**
*/

#include "demo_common.h"
#include "user_intf.h"
#include "si_voice_datatypes.h"

#include DEMO_INCLUDE

/* If the header files are NOT defined, this is the fall back limit */
#define SI_DEMO_MAX_ENUM_DEFAULT 32

#ifdef FXS_CONSTANTS_HDR
#include FXS_CONSTANTS_HDR /* See makefile for how this is determined... */
#endif

#ifdef DAA_CONSTANTS_HDR
#include DAA_CONSTANTS_HDR /* See makefile for how this is determined... */
#endif

#ifdef SI3217X
#include "si3217x.h"
#include "vdaa.h"
extern Si3217x_General_Cfg Si3217x_General_Configuration;
#endif

#ifdef SI3226X
#include "si3226x.h"
#endif

#ifdef SI3218X
#include "si3218x.h"
#endif

#ifdef SI3219X
#include "si3219x.h"
#endif

#ifdef SI3228X
#include "si3228x.h"
#endif

#ifdef VDAA_SUPPORT
#include "vdaa.h"
#endif

#ifdef TSTIN_SUPPORT
#include "proslic_tstin_limits.h"
#endif

/*****************************************************************************************************/
#ifdef PERL

#ifndef NOFXS_SUPPORT
extern const char *Ring_preset_options[];
extern const char *DcFeed_preset_options[];
extern const char *Impedance_preset_options[];
extern const char *FSK_preset_options[];
extern const char *PulseMeter_preset_options[];
extern const char *Tone_preset_options[];
extern const char *PCM_preset_options[];
#endif

#ifdef VDAA_SUPPORT
extern const char *Vdaa_country_preset_options[];
extern const char *Vdaa_audioGain_preset_options[];
extern const char *Vdaa_ringDetect_preset_options[];
extern const char *Vdaa_PCM_preset_options[];
extern const char *Vdaa_hybrid_preset_options[];
#endif

#endif /* PERL */
/*****************************************************************************************************/
#ifndef LOGPRINT_PREFIX
#define LOGPRINT_PREFIX "ProSLIC_DEMO: "
#endif

/*****************************************************************************************************/
/* This is a simple initialization - all ports have the same device type.  For a more complex
 * system, a customer could key off of port_id and depending on value, fill in the "correct" setting
 * for their system. No memory allocation is done here.
 */
#ifndef SIVOICE_USE_CUSTOM_PORT_INFO
void demo_init_port_info(demo_port_t *port, unsigned int port_id)
{
  SILABS_UNREFERENCED_PARAMETER(port_id);
#ifdef SI3217X
  port->deviceType = SI3217X_TYPE;
  port->numberOfDevice = SI3217XB_NUMBER_OF_DEVICE;
  port->numberOfChan = SI3217XB_NUMBER_OF_CHAN;
  port->chanPerDevice = SI3217X_CHAN_PER_DEVICE;
#endif

#ifdef SI3226X
  port->deviceType = SI3226X_TYPE;
  port->numberOfDevice = SI3226X_NUMBER_OF_DEVICE;
  port->numberOfChan = SI3226X_NUMBER_OF_CHAN;
  port->chanPerDevice =SI3226X_CHAN_PER_DEVICE;
#endif

#ifdef SI3218X
  port->deviceType     = SI3218X_TYPE;
  port->numberOfDevice = SI3218X_NUMBER_OF_DEVICE;
  port->numberOfChan   = SI3218X_NUMBER_OF_CHAN;
  port->chanPerDevice  = SI3218X_CHAN_PER_DEVICE;
#endif

#ifdef SI3219X
  port->deviceType     = SI3219X_TYPE;
  port->numberOfDevice = SI3219X_NUMBER_OF_DEVICE;
  port->numberOfChan   = SI3219X_NUMBER_OF_CHAN;
  port->chanPerDevice  = SI3219X_CHAN_PER_DEVICE;
#endif

#ifdef SI3228X
  port->deviceType     = SI3228X_TYPE;
  port->numberOfDevice = SI3228X_NUMBER_OF_DEVICE;
  port->numberOfChan   = SI3228X_NUMBER_OF_CHAN;
  port->chanPerDevice  = SI3228X_CHAN_PER_DEVICE;
#endif

#if defined(SI3050_CHIPSET) && !defined(SI3217X)
  port->deviceType     = SI3050_TYPE;
  port->numberOfDevice = SI3050_NUMBER_OF_DEVICE;
  port->numberOfChan   = SI3050_NUMBER_OF_CHAN;
  port->chanPerDevice  = SI3050_CHAN_PER_DEVICE;
#endif

}
#endif /* SIVOICE_USE_CUSTOM_PORT_INFO */

/*****************************************************************************************************/

int demo_alloc(demo_port_t *port, int *base_channel_index,
               controlInterfaceType *proslic_api_hwIf)
{
  int i;
  static unsigned int chan_ndx=0;

  LOGPRINT("%sAllocating memory\n", LOGPRINT_PREFIX);

  /* First allocate all the needed data structures for ProSLIC API */
  if( SiVoice_createDevices(&(port->devices),
                            (uInt32)(port->numberOfDevice)) != RC_NONE )
  {
    return RC_NO_MEM;
  }

  if( SiVoice_createChannels(&(port->channels),
                             (uInt32)port->numberOfChan) != RC_NONE )
  {
    goto free_mem;
  }

  /* Demo specific code */
  port->channelPtrs = SIVOICE_CALLOC(sizeof(SiVoiceChanType_ptr),
                                     port->numberOfChan);

  port->demo_info = SIVOICE_CALLOC(sizeof(demo_chan_info_t), 
                                     port->numberOfChan); 

  if( (port->channelPtrs == NULL) || (port->demo_info == NULL) )
  {
    goto free_mem;
  }

  /* Back to API init code */
#ifdef TSTIN_SUPPORT
  LOGPRINT("%sCreating Inward Test Objs...\n", LOGPRINT_PREFIX);
  if( ProSLIC_createTestInObjs(&(port->pTstin),
                               (uInt32)(port->numberOfChan) != RC_NONE) )
  {
    goto free_mem;
  }
#endif

  /* Demo specific code */ 
  port->channelBaseIndex = *base_channel_index;
  (*base_channel_index) += port->numberOfChan;

  /* For 3050 based devices we need to ensure the  channel number is even */
  if( port->deviceType == SI3050_TYPE)
  {
    chan_ndx += ((port->channelBaseIndex)&1);
  }

  for(i = 0; i < port->numberOfChan; i++)
  {
    /* If supporting more than 1 SPI interface, a code change is needed here - channel count may not match
     * and the pointer to the api_hwIf may change for the SPI object... */
    if(SiVoice_SWInitChan(&(port->channels[i]),
                          chan_ndx, port->deviceType,
                          &(port->devices[i/port->chanPerDevice]), proslic_api_hwIf ) != RC_NONE)
    {
      goto free_mem;
    }
    /* For Si350 AND a SPI designed for PROSLIC, the channel index needs to be
     * in steps of 2 vs. 1 
     */
    if( port->deviceType == SI3050_TYPE)
    {
      chan_ndx += ((i+1)<<1);
    }
    else
    {
      chan_ndx++;
    }
    port->channelPtrs[i] = &(port->channels[i]); /* Demo specific */
#ifdef TSTIN_SUPPORT
    LOGPRINT("%sConfiguring Inward Tests...\n", LOGPRINT_PREFIX);
    ProSLIC_testInPcmLpbkSetup(port->pTstin, &ProSLIC_testIn_PcmLpbk_Test);
    ProSLIC_testInDcFeedSetup(port->pTstin, &ProSLIC_testIn_DcFeed_Test);
    ProSLIC_testInRingingSetup(port->pTstin, &ProSLIC_testIn_Ringing_Test);
    ProSLIC_testInBatterySetup(port->pTstin, &ProSLIC_testIn_Battery_Test);
    ProSLIC_testInAudioSetup(port->pTstin, &ProSLIC_testIn_Audio_Test);
#endif

    SiVoice_setSWDebugMode(&(port->channels[i]),
                           TRUE); /* Enable debug mode for all channels */
#ifdef ENABLE_INITIAL_LOGGING
    SiVoice_setTraceMode(&(port->channels[i]),
                         TRUE); /* Enable trace mode for all channels */
#endif
  }


  return RC_NONE;

free_mem:
  demo_free(port);
  return RC_NO_MEM;
}

/*****************************************************************************************************/
int demo_init_devices(demo_port_t *port)
{
  int rc=0;


#ifndef NOFXS_SUPPORT
  if( port->deviceType != SI3050_TYPE )
  {
  LOGPRINT("%sInitializing ProSLIC...\n", LOGPRINT_PREFIX);
  if(ProSLIC_Init(port->channelPtrs, port->numberOfChan) != RC_NONE)
  {
    LOGPRINT("%s ERROR: ProSLIC_Init() failed\n", LOGPRINT_PREFIX);
    return(-1);
  }
  }
#endif
#ifdef VDAA_SUPPORT
  if( (port->deviceType == SI3050_TYPE)
#if defined(SI3217X)
      /* Si32178/9 Rev B only supports VDAA */
      ||( (Si3217x_General_Configuration.daa_cntl != 0)
          &&  (port->deviceType == SI3217X_TYPE)
          && (port->channels->deviceId->chipRev == 1) )

#endif /* 17x */
    )
  {
    LOGPRINT("%sInitializing FXO...\n", LOGPRINT_PREFIX);
    /* Initialize any DAA channels that are present */
    if (Vdaa_Init(port->channelPtrs, port->numberOfChan))
    {
      LOGPRINT("\n%s ERROR : DAA Initialization Failed\n", LOGPRINT_PREFIX);
      return(-1);
    }
  }
#endif

#ifndef SKIP_LBCAL
  /*
   ** Longitudinal Balance Calibration
   **
  */
  LOGPRINT("%sStarting Longitudinal Balance Calibration...\n", LOGPRINT_PREFIX);
  rc = ProSLIC_LBCal(port->channelPtrs, port->numberOfChan);
  if(rc != RC_NONE)
  {
    LOGPRINT("\n%sLB CAL ERROR : %d\n", LOGPRINT_PREFIX, rc);
  }
#endif

  return rc;
}

/*****************************************************************************************************/
int demo_load_presets(demo_port_t *port)
{
  int i;
  SiVoiceChanType_ptr chanPtr;

  for(i = 0; i < port->numberOfChan; i++)
  {
    chanPtr = port->channelPtrs[i];
#ifndef NOFXS_SUPPORT
    /* We assume the 1st constants setting is the default one... */
    ProSLIC_DCFeedSetup(chanPtr, 0);
    ProSLIC_RingSetup(chanPtr, 0);
    ProSLIC_ZsynthSetup(chanPtr, 0);
    ProSLIC_PCMSetup(chanPtr, 0);
#endif

#ifdef VDAA_SUPPORT
    Vdaa_CountrySetup(chanPtr, 0);
    Vdaa_HybridSetup(chanPtr, 0);
    Vdaa_PCMSetup(chanPtr, 0);
#endif
  }
  return 0;
}

/*****************************************************************************************************/
int demo_set_chan_state(demo_port_t *port)
{
  int i;
#ifndef NOFXS_SUPPORT
  int j;
#endif
  SiVoiceChanType_ptr chanPtr;

  for(i = 0; i < port->numberOfChan; i++)
  {
    chanPtr = port->channelPtrs[i];
#ifndef NOFXS_SUPPORT
    ProSLIC_SetLinefeedStatus(chanPtr, LF_FWD_ACTIVE);
    ProSLIC_EnableInterrupts(chanPtr);

    /* Cache IRQEN settings for the demo */
    for(j = PROSLIC_REG_IRQEN1; j < PROSLIC_REG_IRQEN4; j++)
    {
      port->demo_info[i].irq_save[j-PROSLIC_REG_IRQEN1] =
        SiVoice_ReadReg(chanPtr, j);
    }
#endif

#ifdef VDAA_SUPPORT
    Vdaa_SetHookStatus(chanPtr, VDAA_ONHOOK);
#endif
  }
  return 0;
}

/*****************************************************************************************************/
void demo_shutdown(demo_port_t *port)
{
#ifdef NOFXS_SUPPORT
  SILABS_UNREFERENCED_PARAMETER(port);
#else
  int i;
  SiVoiceChanType_ptr chanPtr;

  for(i = 0; i < port->numberOfChan; i++)
  {
    chanPtr = port->channelPtrs[i];
    ProSLIC_ShutdownChannel(chanPtr);
  }
#endif
}

/*****************************************************************************************************/
void demo_free(demo_port_t *port)
{
#ifdef TSTIN_SUPPORT
  ProSLIC_destroyTestInObjs(&(port->pTstin));
#endif
  if(port->demo_info != NULL)
  {
    SIVOICE_FREE(port->demo_info);
  }

  SiVoice_destroyChannels( &(port->channels) );
  SiVoice_destroyDevices( &(port->devices) );

  if(port->channelPtrs != NULL)
  {
    SIVOICE_FREE(port->channelPtrs);
  }
}

/*****************************************************************************************************/
int demo_get_preset(demo_preset_t preset_enum)
{
  int max_value, user_input;
  const char *preset_string[] =
  {
    "Ringing",
    "DC Feed",
    "Zsynth",
    "FSK",
    "Pulse Metering",
    "Tone Generation",
    "PCM",
    "Country",
    "Audio Gain",
    "Ring Validation",
    "PCM",
    "Hybrid"
  };
#ifdef PERL
  const char **menu_items;
#endif
  switch(preset_enum)
  {
#ifndef NOFXS_SUPPORT
    case DEMO_RING_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = RINGING_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif
#ifdef PERL
      menu_items = Ring_preset_options;
#endif
      break;

    case DEMO_DCFEED_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value =  DC_FEED_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = DcFeed_preset_options;
#endif
      break;

    case DEMO_IMPEDANCE_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = IMPEDANCE_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Impedance_preset_options;
#endif
      break;

    case DEMO_FSK_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = FSK_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = FSK_preset_options;
#endif
      break;

    case DEMO_PM_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = PULSE_METERING_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = PulseMeter_preset_options;
#endif
      break;

    case DEMO_TONEGEN_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = TONE_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Tone_preset_options;
#endif
      break;

    case DEMO_PCM_PRESET:
#ifdef FXS_CONSTANTS_HDR
      max_value = PCM_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = PCM_preset_options;
#endif
      break;
#endif /* FXS selection */

#ifdef VDAA_SUPPORT
    case DEMO_VDAA_COUNTRY_PRESET:
#ifdef DAA_CONSTANTS_HDR
      max_value = VDAA_COUNTRY_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Vdaa_country_preset_options;
#endif
      break;

    case DEMO_VDAA_AUDIO_GAIN_PRESET:
#ifdef DAA_CONSTANTS_HDR
      max_value = VDAA_AUDIO_GAIN_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Vdaa_audioGain_preset_options;
#endif
      break;

    case DEMO_VDAA_RING_VALIDATION_PRESET:
#ifdef DAA_CONSTANTS_HDR
      max_value = VDAA_RING_VALIDATION_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Vdaa_ringDetect_preset_options;
#endif
      break;

    case DEMO_VDAA_PCM_PRESET:
#ifdef DAA_CONSTANTS_HDR
      max_value = VDAA_PCM_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Vdaa_PCM_preset_options;
#endif
      break;

    case DEMO_VDAA_HYBRID_PRESET:
#ifdef DAA_CONSTANTS_HDR
      max_value = VDAA_HYBRID_LAST_ENUM-1;
#else
      max_value = SI_DEMO_MAX_ENUM_DEFAULT;
#endif

#ifdef PERL
      menu_items = Vdaa_hybrid_preset_options;
#endif
      break;
#endif /* VDAA */
    default:
      return -1; /* We shouldn't hit this line... but just in case... */
  } /* switch(preset_enum) */

#ifndef PERL
  do
  {
    printf("Enter %s preset (0-%d) %s", preset_string[preset_enum], max_value,
           PROSLIC_PROMPT);
    user_input = get_int(0, max_value);
  }
  while( user_input >= max_value);
#else
  do
  {
    display_menu(preset_string[preset_enum], menu_items);
    printf("Select Menu item (0-%d) %s ", max_value, PROSLIC_PROMPT);
    user_input = get_int( 0, max_value) ;
  }
  while(user_input > max_value);
#endif
  return user_input;
}

/*****************************************************************************************************/
SiVoiceChanType_ptr demo_get_cptr(demo_state_t *pState, int channel_number)
{
  int i;

  if(channel_number >= pState->totalChannelCount)
  {
    return NULL;
  }

  /* Determine which port we're on */
  for(i = 0; channel_number < pState->ports[i].channelBaseIndex; i++)
  {
  }
  return pState->ports[i].channelPtrs[(channel_number - pState->ports[i].channelBaseIndex)];
}

/*****************************************************************************************************/
void demo_save_slic_irqens(demo_state_t *pState)
{
  int portIndex = pState->currentChannel - pState->currentPort->channelBaseIndex;
  int i;

  for(i = PROSLIC_REG_IRQEN1; i < PROSLIC_REG_IRQEN4; i++)
  {
    pState->currentPort->demo_info[portIndex].irq_save[i-PROSLIC_REG_IRQEN1] = 
      SiVoice_ReadReg(pState->currentChanPtr, i);
  }
}

/*****************************************************************************************************/
void demo_restore_slic_irqens(demo_state_t *pState)
{
  int portIndex = pState->currentChannel - pState->currentPort->channelBaseIndex;
  int i;

  for(i = PROSLIC_REG_IRQEN1; i < PROSLIC_REG_IRQEN4; i++)
  {
    SiVoice_WriteReg(pState->currentChanPtr, i, 
      pState->currentPort->demo_info[portIndex].irq_save[i-PROSLIC_REG_IRQEN1] );
  }
}

