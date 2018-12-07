/*
** Copyright (c) 2013-2018 by Silicon Laboratories, Inc.
**
** $Id: api_demo.c 7054 2018-04-06 20:57:58Z nizajerk $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
**
*/

#include "stdio.h"
#include "demo_config.h"
#include "api_demo.h"
#include "user_intf.h"
#include "spi_main.h"

#ifdef MLT_ENABLED
#include "proslic_mlt.h"
#endif

/*
** Device Specific Includes
*/
#ifdef SI3217X
#include "si3217x.h"
#include "vdaa.h"
extern Si3217x_General_Cfg Si3217x_General_Configuration;
#endif

#ifdef SI_ENABLE_LOGGING
#include <stdio.h>
FILE *SILABS_LOG_FP;
FILE *std_out;
int is_stdout = 1;
#endif

#ifdef LUA
#include <lua_util.h>
#endif

/*****************************************************************************************************/
void change_channel(demo_state_t *pState)
{
  int i,m;
  demo_port_t *cPort;

  if(pState->totalChannelCount == 1)
  {
    printf("Only 1 channel supported, aborting request.\n");
    return;
  }

  do
  {
    printf("\nPlease Enter Channel:  (0->%d) %s", (pState->totalChannelCount-1),
           PROSLIC_PROMPT);
    fflush(stdin);
    m = get_int(0, (pState->totalChannelCount-1));
  }
  while( m >= pState->totalChannelCount);

  pState->currentChannel = m;

  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    cPort = &(pState->ports[i]);
    if(m < ((cPort->numberOfChan) + (cPort->channelBaseIndex) ) )
    {
      pState->currentPort = cPort;
      pState->currentChanPtr = cPort->channelPtrs[(m - (cPort->channelBaseIndex) ) ];
      return;
    }
  }
}

/*****************************************************************************************************/
int main(void)
{
  ctrl_S                spiGciObj;  /* Link to host spi obj (os specific) */
  systemTimer_S         timerObj;   /* Link to host timer obj (os specific)*/
  controlInterfaceType  ProHWIntf;  /* proslic hardware interface object */
  demo_port_t           *ports;     /* declare channel state structures */
  demo_state_t          demo_state;
  int                   i=0;

  SETUP_STDOUT_FLUSH;

#ifdef SI_ENABLE_LOGGING
  std_out = stdout;

#ifdef ENABLE_INITIAL_LOGGING
  SILABS_LOG_FP = fopen("api_demo.log", "w");
  if(SILABS_LOG_FP == NULL)
  {
    perror("Failed to open log file - aborting");
    exit -2;
  }
  is_stdout = 0;
#else
  SILABS_LOG_FP = stdout;
#endif

#endif

  ports = SIVOICE_CALLOC(sizeof(demo_port_t), DEMO_PORT_COUNT);
  if(ports == NULL)
  {
    printf("%s Failed to allocate memory\n", LOGPRINT_PREFIX);
    return -1;
  }

  demo_state.currentChannel = 0;
  demo_state.totalChannelCount = 0;
  demo_state.ports = ports;
  demo_state.currentPort = ports;

  PROSLIC_CLS;
  print_banner("ProSLIC API Implementation Demo");
  printf("\nCopyright 2013-2018, Silicon Labs\n");
  printf("API Version %s\n", ProSLIC_Version());
#ifdef MLT_ENABLED
  printf("MLT Version: %s\n", ProSLIC_mlt_version());
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
  ** ----------------------------------------------------------------------
  ** Initialize host TIMER interface object (optional - platform dependent)
  ** ----------------------------------------------------------------------
  */
  printf("%sInitializing system timer...\n", LOGPRINT_PREFIX);
  TimerInit(&timerObj);

  LOGPRINT("%sLinking function pointers...\n", LOGPRINT_PREFIX);
  initControlInterfaces(&ProHWIntf, &spiGciObj, &timerObj);

  /*
  ** In theory, you could have multiple DEMO_PORTs aka evb boards/chipset combos.
  ** This feature has not been tested well enough at this point...
  */
  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_init_port_info(&ports[i], i);

    /* We assume in our sivoice_init_port_info that we have a 178/179 - which
       is a SLIC + VDAA device, 2 ports, if the VDAA support is disabled,
       stop here and not allocate/init the 2nd port.
    */
#if defined(SI3217X) && !defined(VDAA_SUPPORT)
    /* Subtract from #chan the device count since we doubled it */
    ports[i].numberOfChan -= ports[i].numberOfDevice;
    ports[i].chanPerDevice--;
#endif

    if(demo_alloc(&ports[i], &(demo_state.totalChannelCount),
                  &ProHWIntf) != RC_NONE)
    {
      LOGPRINT("%sFailed to allocate for port %d - exiting program.\n",
               LOGPRINT_PREFIX, i);
      exit(-2);
    }
#if defined(SI3217X) && defined(VDAA_SUPPORT)
    /* Check if we do have a 178/179 - we assume the 1st device is just like the rest... */
    if(Si3217x_General_Configuration.daa_cntl!= 0)
    {
      uInt8 reg_data;
      reg_data = SiVoice_ReadReg(*(ports->channelPtrs), PROSLIC_REG_ID);
      if( (( (reg_data>>3) & 7) != 6)
          && ( (reg_data & 0xF) == 1 ) )
      {
        ports->numberOfChan -= SI3217XB_NUMBER_OF_DEVICE;
        ports->chanPerDevice--;
        demo_state.totalChannelCount -= SI3217XB_NUMBER_OF_DEVICE;
      }
    }
    else
    {
      ports->numberOfChan -= SI3217XB_NUMBER_OF_DEVICE;
      ports->chanPerDevice--;
      demo_state.totalChannelCount -= SI3217XB_NUMBER_OF_DEVICE;
    }
#endif
    printf("\tNumber of devices: %d Number of channels: %d\n",
           ports[i].numberOfDevice,
           ports[i].numberOfChan);


    if( (demo_init_devices(&ports[i]) != 0)
        || ( demo_load_presets(&ports[i]) != 0)
        || (demo_set_chan_state(&ports[i]) ) )
    {
      exit(-3);
    }
  }
  demo_state.currentChanPtr = ports->channels;

  printf("%s Initialization Complete\n", LOGPRINT_PREFIX);
#ifdef LUA
    init_lua(&demo_state);
#endif
  if( demo_state.currentChanPtr->channelType != DAA)
  {
#ifdef NOFXS_SUPPORT
    printf("Detected a FXS in a FXO only build!\n");
    exit(-4);
#else
    proslic_main_menu(&demo_state);
#endif
  }
#ifdef VDAA_SUPPORT
  else
  {
    daa_main_menu(&demo_state);
  }

#endif
  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_shutdown(&ports[i]);
  }

  SiVoice_Reset(demo_state.currentChanPtr);

  for(i = 0; i < DEMO_PORT_COUNT; i++)
  {
    demo_free(&ports[i]);
  }
  SIVOICE_FREE(ports);
  return 0;
}

/* 26x_3050 example */
#ifdef SIVOICE_USE_CUSTOM_PORT_INFO 
#include "si3226x.h" 
#include "vdaa.h" 

void demo_init_port_info(demo_port_t *port, unsigned int port_id)
{
  switch(port_id)
  {
    case 0:
      printf("Setting up Si3226x\n");
      port->deviceType = SI3226X_TYPE;
      port->numberOfDevice = SI3226X_NUMBER_OF_DEVICE;
      port->numberOfChan = SI3226X_NUMBER_OF_CHAN;
      port->chanPerDevice =SI3226X_CHAN_PER_DEVICE;
      break;

    case 1:
      printf("Setting up Si3050\n");
      port->deviceType     = SI3050_TYPE;
      port->numberOfDevice = SI3050_NUMBER_OF_DEVICE;
      port->numberOfChan   = SI3050_NUMBER_OF_CHAN;
      port->chanPerDevice  = SI3050_CHAN_PER_DEVICE;
      break;

    default:
      printf("Unknown port: %d\n", port_id);
      break;
  }
}
#endif
