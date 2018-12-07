/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: demo_common.h 6594 2017-06-30 22:51:35Z mjmourni $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** Common functions for the various API demo programs.
**
**
*/

#ifndef __PROSLIC_DEMO_COMMON_HDR__
#define __PROSLIC_DEMO_COMMON_HDR__  1

#include "proslic.h"

#ifdef TSTIN_SUPPORT
#include "proslic_tstin.h"
#endif

#define DEMO_NUM_IRQS 4
/*****************************************************************************************************/
typedef struct
{
  uInt8                 irq_save[DEMO_NUM_IRQS]; /* Save/restore IRQ settings for various functions */
  BOOLEAN               isFRS_enabled;          /* Is fast ring start enabled */
} demo_chan_info_t; 

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
  int                   deviceType;
  int                   numberOfDevice;
  int                   numberOfChan;
  int                   chanPerDevice;
  int                   channelBaseIndex;

  SiVoiceChanType_ptr   channels;
  SiVoiceChanType_ptr   *channelPtrs;
  SiVoiceDeviceType_ptr devices;
  demo_chan_info_t      *demo_info;
#ifdef TSTIN_SUPPORT
  proslicTestInObjType_ptr pTstin;
#endif

} demo_port_t;

typedef struct
{
  demo_port_t         *ports;
  demo_port_t         *currentPort;
  int                 currentChannel;
  int                 totalChannelCount;
  SiVoiceChanType_ptr currentChanPtr;
} demo_state_t;

typedef enum
{
  DEMO_RING_PRESET,
  DEMO_DCFEED_PRESET,
  DEMO_IMPEDANCE_PRESET,
  DEMO_FSK_PRESET,
  DEMO_PM_PRESET,
  DEMO_TONEGEN_PRESET,
  DEMO_PCM_PRESET,
  DEMO_VDAA_COUNTRY_PRESET,
  DEMO_VDAA_AUDIO_GAIN_PRESET,
  DEMO_VDAA_RING_VALIDATION_PRESET,
  DEMO_VDAA_PCM_PRESET,
  DEMO_VDAA_HYBRID_PRESET,
} demo_preset_t;

/*****************************************************************************************************/
void demo_init_port_info(demo_port_t *port, unsigned int port_id);
int  demo_alloc(demo_port_t *port, int *base_channel_index,
                controlInterfaceType *proslic_api_hwIf);
int  demo_init_devices(demo_port_t *port);
int  demo_load_presets(demo_port_t *port);
void demo_shutdown(demo_port_t *port);
void demo_free(demo_port_t *port);
int  demo_get_preset(demo_preset_t preset_enum);
/* Return back the channel pointer, given a channel number */
SiVoiceChanType_ptr demo_get_cptr(demo_state_t *demo_state, int channel_number);
void demo_save_slic_irqens(demo_state_t *pState);
void demo_restore_slic_irqens(demo_state_t *pState);
int demo_set_chan_state(demo_port_t *port);

#endif /* __PROSLIC_DEMO_COMMON_HDR__  */

