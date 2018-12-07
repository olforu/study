/*
** Copyright (c) 2015-2017 by Silicon Laboratories
**
** $Id: proslic_linux.c 6483 2017-05-03 03:33:42Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** Main file for ProSLIC Linux "core" api module.
**
**
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

#include "proslic.h"
#include "si_voice.h"

#ifdef SI3217X
#include "si3217x.h"
#include "vdaa.h"
extern Si3217x_General_Cfg Si3217x_General_Configuration;
#endif
#ifdef SI3218X
#include "si3218x.h"
extern Si3218x_General_Cfg Si3218x_General_Configuration;
#endif
#ifdef SI3219X
#include "si3219x.h"
extern Si3219x_General_Cfg Si3219x_General_Configuration;
#endif
#ifdef SI3226X
#include "si3226x.h"
#endif
#ifdef SI3228X
#include "si3228x.h"
#endif

#include "proslic_sys.h"
#include "proslic_linux_core.h"

#ifdef TSTIN_SUPPORT
#include "proslic_tstin.h"
#endif

#define PROSLIC_CORE_VER     "0.1.1" /* NOTE: Not the same version as the userspace version! */
#define DRIVER_DESCRIPTION   "ProSLIC API core module"
#define DRIVER_AUTHOR        "Silicon Laboratories"

/* If supporting more than 1 SPI interface or more than 1 device on a daisy chain, a code change is needed here */
static SiVoiceControlInterfaceType proslic_api_hwIf;
proslic_core_t *proslic_ports = NULL;
uint8_t proslic_chan_init_count = 0; /* How many channels were initialized, vs. detected */

/*****************************************************************************************************/

static void initControlInterfaces(SiVoiceControlInterfaceType *ProHWIntf, void *spiIf, void *timerObj)
{
  SiVoice_setControlInterfaceCtrlObj (ProHWIntf, spiIf);
	SiVoice_setControlInterfaceReset (ProHWIntf, proslic_spi_if.reset);

	SiVoice_setControlInterfaceWriteRegister (ProHWIntf, proslic_spi_if.write_reg);
	SiVoice_setControlInterfaceReadRegister (ProHWIntf, proslic_spi_if.read_reg);		
	SiVoice_setControlInterfaceWriteRAM (ProHWIntf, proslic_spi_if.write_ram);
	SiVoice_setControlInterfaceReadRAM (ProHWIntf, proslic_spi_if.read_ram);

	SiVoice_setControlInterfaceTimerObj (ProHWIntf, timerObj);
	SiVoice_setControlInterfaceDelay (ProHWIntf, proslic_timer_if.slic_delay);
	SiVoice_setControlInterfaceTimeElapsed (ProHWIntf, proslic_timer_if.elapsed_time);
	SiVoice_setControlInterfaceGetTime (ProHWIntf, proslic_timer_if.get_time);

	SiVoice_setControlInterfaceSemaphore (ProHWIntf, NULL);
}

/*****************************************************************************************************/
 /* This is a simple initialization - all ports have the same device type.  For a more complex 
  * system, a customer could key off of port_id and depending on value, fill in the "correct" setting
  * for their system. No memory allocation is done here.
  */
static void init_port_info(proslic_core_t *port, unsigned int port_id)
{

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

}

/*****************************************************************************************************/

static int proslic_api_init(proslic_core_t *port, int *base_channel_index)
{
  int i;

  printk(KERN_INFO "%s in proslic_api_init()\n", PROSLIC_CORE_PREFIX);

  /* First allocate all the needed data structures */
  if( SiVoice_createDevices(&(port->devices), (uInt32)(port->numberOfDevice)) != RC_NONE )
  {
    return -ENOMEM;
  }

  if( SiVoice_createChannels(&(port->channels), (uInt32)port->numberOfChan) != RC_NONE )
  {
    goto free_mem;
  }

  port->channelPtrs = kzalloc(sizeof(port->channelPtrs)*port->numberOfChan, GFP_KERNEL);

#ifdef TSTIN_SUPPORT
  if( ProSLIC_createTestInObjs(&(port->pTstin), (uInt32)(port->numerOfChan) != RC_NONE) )
  {
    goto free_mem;
  }
#endif

  for(i = 0; i < port->numberOfChan; i++)
  {
    /* If supporting more than 1 SPI interface, a code change is needed here - channel count may not match
     * and the pointer to the api_hwIf may change for the SPI object... */
    if(SiVoice_SWInitChan(&(port->channels[i]), 
      (i + port->channelBaseIndex), port->deviceType, 
      &(port->devices[i/port->chanPerDevice]), &proslic_api_hwIf ) != RC_NONE)
    {
      goto free_mem;
    }
    port->channelPtrs[i] = &(port->channels[i]);
#ifdef TSTIN_SUPPORT
	  ProSLIC_testInPcmLpbkSetup(port->pTstin, &ProSLIC_testIn_PcmLpbk_Test);
		ProSLIC_testInDcFeedSetup(port->pTstin, &ProSLIC_testIn_DcFeed_Test);
		ProSLIC_testInRingingSetup(port->pTstin, &ProSLIC_testIn_Ringing_Test);
		ProSLIC_testInBatterySetup(port->pTstin, &ProSLIC_testIn_Battery_Test);
		ProSLIC_testInAudioSetup(port->pTstin, &ProSLIC_testIn_Audio_Test);
#endif
    SiVoice_setSWDebugMode(&(port->channels[i]), TRUE); /* Enable debug mode for all channels */
  }

  proslic_chan_init_count += port->numberOfChan; /* count the number of channels initialized */
  port->channelBaseIndex = *base_channel_index;
  (*base_channel_index) += port->numberOfChan;

  return 0;

  free_mem:
#ifdef TSTIN_SUPPORT
    ProSLIC_destroyTestInObjs(&(port->pTstin));
#endif
    SiVoice_destroyChannels( &(port->channels) );
    SiVoice_destroyDevices( &(port->devices) );
    return -ENOMEM;
}

/*****************************************************************************************************/
int init_module(void)
{
  void         *hCtrl;
  int          num_channels;
  unsigned int i;
  int rc = 0;

  printk( KERN_INFO "%s ProSLIC API core module loading, version: %s api version: %s\n", 
    PROSLIC_CORE_PREFIX, PROSLIC_CORE_VER, SiVoice_Version() );
  printk( KERN_INFO "%s Copyright 2015-2016, Silicon Laboratories\n", PROSLIC_CORE_PREFIX);
  /* 
   * Since the ProSLIC systems services module, as delivered from Silabs does not need further initialization
   * we skip any allocation of timer and spi resources in this module... we do check if there was a device detected
   * though..
   */

  num_channels = proslic_get_channel_count();

  if(num_channels == 0)
  {
    return -EIO;
  }

  proslic_ports = kzalloc(sizeof(*proslic_ports)*PROSLIC_NUM_PORTS, GFP_KERNEL );

  if(proslic_ports == NULL)
  {
    return -ENOMEM;
  }

  num_channels = 0; /* Now we use num_channels to set the channel base index */

  for(i = 0; i < PROSLIC_NUM_PORTS; i++)
  {
    /* If supporting more than 1 SPI interface, a code change is needed here as well at the system services layer. */
    hCtrl = proslic_get_hCtrl(0);

    if(hCtrl)
    {
      initControlInterfaces(&proslic_api_hwIf, hCtrl, NULL);
      init_port_info(&proslic_ports[i],i);
      proslic_api_init(&(proslic_ports[i]), &num_channels);

    }
    else
    {
      return -EIO;
    }
  }

  rc = proslic_api_char_dev_init();

  if(rc < 0)
  {
    cleanup_module();
  }

  /* 
   * Now put the part into reset, then take us out of reset - we assume all ProSLIC/DAA devices are connected
   * to the same reset. NOTE: The proslic system module in our example takes the device(s) out of reset,
   * this gets undone here for a "clean" start...
   */
  SiVoice_Reset(proslic_ports->channels);

  return rc;
}

/*****************************************************************************************************/

void cleanup_module(void)
{
  printk(KERN_ALERT "%s: unloading module", PROSLIC_CORE_PREFIX);

  proslic_api_char_dev_quit();

  if(proslic_ports)
  { 
    unsigned int i;

    /* 
     * reset the device(s) for safety - for a real application, you may not want to do this for cases 
     * of software updates and have an alarm system connected...
     */
    SiVoice_Reset(proslic_ports->channels);

    for(i = 0; i < PROSLIC_NUM_PORTS; i++)
    {
      /* If supporting more than 1 SPI interface or more than 1 device on a daisy chain, a code change is needed here */
      SiVoice_destroyChannels( &(proslic_ports[i].channels) );
      SiVoice_destroyDevices( &(proslic_ports[i].devices) );
#ifdef TSTIN_SUPPORT
      ProSLIC_destroyTestInObjs(&(proslic_ports[i].pTstin), port->numerOfChan);
#endif
      if(proslic_ports[i].channelPtrs)
      {
        kfree(proslic_ports[i].channelPtrs);
      }
    }

    kfree(proslic_ports);
  }
  proslic_ports = 0;
}

/*****************************************************************************************************/
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("Proprietary");

