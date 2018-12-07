/*
** Copyright (c) 2015-2017 by Silicon Laboratories
**
** $Id: proslic_linux_core.h 6483 2017-05-03 03:33:42Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
**
*/

#ifndef __PROSLIC_LINUX_CORE_HDR__
#define __PROSLIC_LINUX_CORE_HDR__ 1

#include "proslic.h"
#include "demo_config.h"

#define MAX_NUMBER_OF_DEVICE 16
#define MAX_NUMBER_OF_CHAN 32

#define SI3217XB_NUMBER_OF_CHAN (SI3217XB_NUMBER_OF_DEVICE*SI3217X_CHAN_PER_DEVICE)
#define SI3217XC_NUMBER_OF_CHAN (SI3217XC_NUMBER_OF_DEVICE*SI3217X_CHAN_PER_DEVICE)
#define SI3218X_NUMBER_OF_CHAN (SI3218X_NUMBER_OF_DEVICE*SI3218X_CHAN_PER_DEVICE)
#define SI3219X_NUMBER_OF_CHAN (SI3219X_NUMBER_OF_DEVICE*SI3219X_CHAN_PER_DEVICE)
#define SI3226X_NUMBER_OF_CHAN (SI3226X_NUMBER_OF_DEVICE*SI3226X_CHAN_PER_DEVICE)
#define SI3228X_NUMBER_OF_CHAN (SI3228X_NUMBER_OF_DEVICE*SI3228X_CHAN_PER_DEVICE)

#define PROSLIC_NUM_PORTS          1
#define PROSLIC_CORE_PREFIX "ProSLIC_Core: "

/*****************************************************************************************************/
/* This data structure is to keep track a string of the same devices in 1 container.  If the system
 * has 2 different devices - say  3 Si3226x's and 1 Si3217x, you would need 2 instances of this structure - 
 * 1 to contain the 3 Si3226x's and 1 to contain the Si3217x.  
 *
 * The initialization code at present assumes everything is on the same daisy chain.  Efforts are made 
 * to note where a code change would be needed if this is not true.
 */
typedef struct
{
  int                 deviceType;
  int                 numberOfDevice;
  int                 numberOfChan;
  int                 chanPerDevice;
  int                 channelBaseIndex;

  SiVoiceChanType_ptr   channels;
  SiVoiceChanType_ptr   *channelPtrs;
  SiVoiceDeviceType_ptr devices;

#ifdef TSTIN_SUPPORT
  proslicTestInObjType_ptr pTstin;
#endif

} proslic_core_t;

/*****************************************************************************************************/
/* Global symbols within the module */

int proslic_api_char_dev_init(void);
void proslic_api_char_dev_quit(void);

extern proslic_core_t *proslic_ports;
extern uint8_t proslic_chan_init_count;

#endif /* __PROSLIC_LINUX_CORE_HDR__ */
