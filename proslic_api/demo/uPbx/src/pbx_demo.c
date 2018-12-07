/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo.c 6476 2017-05-03 01:08:55Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file is the main C file that initializes the ProSLIC, CID state machine,
** & PBX statemachine.  It then starts the PBX demo polling function and returns
** when needed.
**
**
*/
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "si_voice_datatypes.h"

#include "si_voice.h"
#include "proslic.h"

#ifdef CID_ONLY_DEMO
#include "cid_demo.h"
#endif

#include "pbx_demo.h"
#include "pbx_demo_cfg.h"
#include "spi_main.h"
#include "demo_common.h"
#include "user_intf.h"

#ifdef CID_ONLY_DEMO
#include "cid_demo_util.h"
#include "cid_demo_proslic.h"
#include "si_cid.h"
#include "si_cid_en_na.h"
#include "si_cid_fsm.h"

extern const char si_cid_version[];

#ifdef SI_CID_ENC_NA
#include "cid_demo_na.h"

/** Type 1 FSM - Onhook data transfer */
extern const si_cid_fsm_t si_cid_na_type1_fsm;

#endif

#endif

#ifdef LUA
#include <lua_util.h>
#endif


#include FXS_CONSTANTS_HDR

/****************************************************************************/
static void pbx_demo_init_cid_port(chanState_t *port, char *name, char *number)
{
  /* Configure the static members of the call */
  SIVOICE_STRCPY(port->cid_data.number,number);

#ifdef CID_ONLY_DEMO
  SIVOICE_STRCPY(port->cid_data.name,name);

  port->init_data.gen_param.enc_ptr = si_cid_enc_na;
  port->init_data.gen_param.fsm_ptr = &si_cid_na_type1_fsm;
  port->init_data.gen_param.fsk_index = DEFAULT_CID_FSK_NA;
  port->init_data.gen_param.ring_index = DEFAULT_CID_RING_NA;

  /* Set some sane defaults */
  port->init_data.encoder_data.na.is_sdmf      = FALSE;
  port->init_data.encoder_data.na.is_vmwi_msg  = FALSE;
  port->init_data.encoder_data.na.is_toll_call = FALSE;
  port->init_data.encoder_data.na.vmwi_status  = FALSE;

  port->cid_data.country_setting = SI_CID_COU_NA;
#endif

  port->digitCount = 0;
  port->hook_change = 0;
  port->ringCount = 0;
}

/****************************************************************************/
static  errorCodeType pbx_demo_cid_setup(pbx_demo_t *pbx_setup)
{
  int i,j,rc;
  char *names[4] = {"Silicon Labs","Major Customer","ProSLIC Ave","DAA user"};
  SiVoiceChanType_ptr chanPtr;
  char phoneNumber[15];
  uInt8 reg_value;

#if defined(PROSLIC_MANUAL_RING) || defined CID_MANUAL_RING_ENABLE
  ProSLIC_ringCadence_t ringCadenceTiming = {
    PROSLIC_MAX_RING_PERIODS,
    {{SI_PBX_MSEC_TO_TICKS(200), SI_PBX_MSEC_TO_TICKS(400)},
    {SI_PBX_MSEC_TO_TICKS(2010), SI_PBX_MSEC_TO_TICKS(3300)}}
  };
#endif

#ifdef CID_ONLY_DEMO
  const si_cid_gen_enc_params_t general_params
  = /* Default : North America FSK/RING*/
  {
    DEFAULT_CID_FSK_NA,
    DEFAULT_CID_RING_NA,
    NULL, /* The FSM is set in the execute command */
    NULL,
    FALSE,
    FALSE,
    FALSE
  };
#endif
  /* Now setup CID specific info */
  pbx_setup->cid_states =
    SIVOICE_CALLOC(pbx_setup->demo_state.totalChannelCount, sizeof(chanState_t));

  if(pbx_setup->cid_states == NULL)
  {
    printf("%sfailed to allocate for cid_channel\n", LOGPRINT_PREFIX);
    exit (-3);
  }

#ifdef CID_ONLY_DEMO
  if( si_cid_init_fsm(pbx_setup->demo_state.totalChannelCount) != SI_CID_RC_OK)
  {
    return -1;
  }
#endif
  pbx_setup->chan_info = SIVOICE_CALLOC( pbx_setup->demo_state.totalChannelCount,
                                         sizeof(pbx_chan_t) );

  if(pbx_setup->chan_info == NULL)
  {
    printf("%sfailed to allocate for PBX chan info\n", LOGPRINT_PREFIX);
    exit (-3);
  }

  for(i = 0; i < pbx_setup->demo_state.totalChannelCount; i++)
  {
    /* Check if the chipset supports certain capabilities for CID */
    chanPtr = demo_get_cptr(&pbx_setup->demo_state, i);

#ifdef CID_ONLY_DEMO
    rc = cid_demo_chk_chipset(&pbx_setup->cid_states[i], chanPtr);

    if(  (rc != RC_NONE )
         || (pbx_setup->cid_states[i].is_dtmf_decoder_avail == FALSE))
    {
      printf("%sChipset may not support fully the CID demo (DTMF detection)\n", LOGPRINT_PREFIX);
    }

    /* Associate the channel pointer with the FSM code */
        si_cid_assoc_proslic_chan(i, chanPtr);

        if(si_cid_assoc_usr_data(i,&(pbx_setup->cid_states[i])) != SI_CID_RC_OK)
        {
          return RC_CHANNEL_TYPE_ERR;
        }
#ifdef CID_MANUAL_RING_ENABLE
        si_cid_assoc_manualCadence(i, &ringCadenceTiming);
#endif
#else /* CID_ONLY_DEMO not set */
        /* Setup ringer here... 
         * NOTE: for manual ringing, one needs to either have the ring timers off
         * or set the on/off time to the longest configured period.
         */
        ProSLIC_RingSetup(chanPtr, 0); 
#endif

    ProSLIC_PCMSetup(chanPtr, PBX_PCM_PRESET);
    /* Set audio gain to -6, -2 */
    ProSLIC_AudioGainSetup(chanPtr, -6, -2, 0);


    sprintf(phoneNumber, "55512%02d", i);

    pbx_demo_init_cid_port(&(pbx_setup->cid_states[i]), names[i%4],
                           phoneNumber);
    printf("%sChannel %d phone number = %s\n", LOGPRINT_PREFIX,
        i, phoneNumber);

    pbx_setup->chan_info[i].state = SI_PBX_DEMO_FXS_STATE_IDLE;
    ProSLIC_InitializeHookChangeDetect( &(pbx_setup->chan_info[i].hookDetection),
      &(pbx_setup->chan_info[i].hookTimer) );

    ProSLIC_PCMTimeSlotSetup(chanPtr, PBX_TIMESLOTS(i), PBX_TIMESLOTS(i));

    /*
     * For this demo to work correctly, we need a few interrupts enabled.
     * In order to avoid any issues with configuration files that do not support
     * these interrupts, we add the ones we need here.
     */
    reg_value = SiVoice_ReadReg(chanPtr, PROSLIC_REG_IRQEN1 );
    reg_value |= 0x50; /* FSK | Ring timer */
    SiVoice_WriteReg(chanPtr, PROSLIC_REG_IRQEN1, reg_value);

    reg_value = SiVoice_ReadReg(chanPtr, PROSLIC_REG_IRQEN1+1 );
    reg_value |= 0x3; /* LCR | RTP */
    SiVoice_WriteReg(chanPtr, PROSLIC_REG_IRQEN1+1, reg_value);

    /* We need to init the PBX version for the CID code since
     * we do not have access to the CID version outside the CID framework
     * internal state machine...
     */
#if defined(PROSLIC_MANUAL_RING) || defined(CID_MANUAL_RING_ENABLE) 
    ProSLIC_RingCadenceInit( &(pbx_setup->chan_info[i].ringCadence), &ringCadenceTiming);
#endif

#ifdef PBX_DEMO_ENABLE_MWI
    ProSLIC_MWIEnable(chanPtr);
    ProSLIC_SetMWIState(chanPtr, SIL_MWI_FLASH_ON);
#endif
  }
  return RC_NONE;
}

/****************************************************************************/
void pbx_demo_teardown(pbx_demo_t *pbx_demo)
{
}

/****************************************************************************/
/* Main entry point */
int main(void)
{
  ctrl_S                spiGciObj;  /* Link to host spi obj (os specific) */
  systemTimer_S         timerObj;   /* Link to host timer obj (os specific)*/
  controlInterfaceType  ProHWIntf;  /* proslic hardware interface object */

  pbx_demo_t pbx_demo;	    /* User's channel object, which has
                             * a member defined as SiVoiceChanType_ptr VoiceObj
                             */
  unsigned int i;

  SETUP_STDOUT_FLUSH;
  PROSLIC_CLS;

  print_banner("micro-PBX demonstration software. Version 0.1.0");
  printf("Copyright 2010-2017, Silicon Labs, Released under NDA\n");
#ifdef CID_DEMO_ONLY
  printf("CID API framework: %s\nProSLIC API: %s\n", si_cid_version,
         ProSLIC_Version());
#else
  printf("ProSLIC API: %s\n", ProSLIC_Version());
#endif
  /*
  ** --------------------------------------------------------------------
  ** Initialize host SPI interface object (optional - platform dependent)
  ** --------------------------------------------------------------------
  */
  printf("\n%sConnecting to Host -> %s ...\n", LOGPRINT_PREFIX, VMB_INFO);
  if(SPI_Init(&spiGciObj) == FALSE)
  {
    printf("%sCannot connect to %s\n",LOGPRINT_PREFIX, VMB_INFO);
    exit(-1);
  }

  /*
  ** --------------------------------------------------------------------
  ** We need to enable cross connect for VMB2... firmware >= 1.7
  ** If you need to try this out on older VMB2's, you need to isolate RX/TX
  ** PCM pins and short them...
  **
  ** For VMB1 - just place a shunt across the RX/TX pin header.
  ** ----------------------------------------------------------------------
  */
#ifdef VMB2
  {
    uInt16 firmware_version;
    firmware_version = GetFirmwareID();
    if( firmware_version < 0x107 )
    {
      printf("Error: unsupported VMB2 firmware revision: %d.%d\n",
         ((firmware_version >> 8)&0x00FF),
         (firmware_version & 0x00FF));
      exit(-1);
    }

    PCM_enableXConnect(1);
  }
#endif

  /*
  ** ----------------------------------------------------------------------
  ** Initialize host TIMER interface object (optional - platform dependent)
  ** ----------------------------------------------------------------------
  */
  printf("%sInitializing system timer...\n", LOGPRINT_PREFIX);
  TimerInit(&timerObj);

  LOGPRINT("%sLinking function pointers...\n", LOGPRINT_PREFIX);
  initControlInterfaces(&ProHWIntf, &spiGciObj, &timerObj);

  pbx_demo.demo_state.ports = SIVOICE_CALLOC(sizeof(demo_port_t), DEMO_PORT_COUNT);
  if(pbx_demo.demo_state.ports == NULL)
  {
    printf("%s Failed to allocate memory\n", LOGPRINT_PREFIX);
    return -1;
  }

  pbx_demo.demo_state.totalChannelCount = 0;

  /*
   ** This demo supports single device/BOM option only - for now...
   */
  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_init_port_info(&pbx_demo.demo_state.ports[i],i);

    /* We assume in our sivoice_init_port_info that we have a 178/179 - which
       is a SLIC + VDAA device, 2 ports, if the VDAA support is disabled,
       stop here and not allocate/init the 2nd port.

       NOTE: for the CID demo, we never have VDAA support enabled since we can't
       decode it!
    */
#if defined(SI3217X)
    /* Subtract from #chan the device count since we doubled it */
    pbx_demo.demo_state.ports[i].numberOfChan -= pbx_demo.demo_state.ports[i].numberOfDevice;
    pbx_demo.demo_state.ports[i].chanPerDevice--;
#endif
    if(demo_alloc(&pbx_demo.demo_state.ports[i], &(pbx_demo.demo_state.totalChannelCount),
                  &ProHWIntf) != RC_NONE)
    {
      LOGPRINT("%sFailed to allocate for port %u - exiting program.\n",
               LOGPRINT_PREFIX, i);
      exit(-2);
    }

    /* Generic demo framework setup */
    if( (demo_init_devices(&pbx_demo.demo_state.ports[i]) != 0)
        || (demo_load_presets(&pbx_demo.demo_state.ports[i]) != 0)
        || (demo_set_chan_state(&pbx_demo.demo_state.ports[i]) ) )
    {
      return -3;
    }

    printf("\nport: %u\tNumber of devices: %d Number of channels: %d\n", i,
           pbx_demo.demo_state.ports[i].numberOfDevice,
           pbx_demo.demo_state.ports[i].numberOfChan);
  }

  pbx_demo.demo_state.currentPort = pbx_demo.demo_state.ports;
  pbx_demo.demo_state.currentChannel = 0;
  pbx_demo.is_not_done = FALSE;

  /*
   * Now configure the Caller ID and remaining items specific PBX demo
   * functions.
   */
  if(pbx_demo_cid_setup(&pbx_demo) != RC_NONE)
  {
    printf("Failed to setup CID\n");
    return -4;
  }

#ifdef LUA
    init_lua(&(pbx_demo.demo_state));
#endif


  /* We stay in cid_demo_run until we quit */
  printf("Demo is now ready to start - perform a hookflash to change settings\n");
  pbx_demo_exec(&pbx_demo);
#ifdef CID_ONLY_DEMO
  /* Tear down everything */
  si_cid_quit_fsm(pbx_demo.demo_state.totalChannelCount);
#endif
  pbx_demo_teardown(&pbx_demo);

  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_shutdown(&(pbx_demo.demo_state.ports[i]));
  }

  SiVoice_Reset(pbx_demo.demo_state.currentChanPtr);
  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_free(&(pbx_demo.demo_state.ports[i]));
  }

  SIVOICE_FREE(pbx_demo.demo_state.ports);

  return 0;
}

