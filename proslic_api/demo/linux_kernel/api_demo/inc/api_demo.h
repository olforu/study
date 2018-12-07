/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: api_demo.h 5150 2015-10-05 18:56:06Z nizajerk $
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
** main header file for the Linux kernel based userspace demo application.
**
**
*/

#ifndef __PROSLIC_API_DEMO_HDR__
#define __PROSLIC_API_DEMO_HDR__ 1

#include "user_intf.h"
#include "si_voice_datatypes.h"

/* User defined settings */
#define PROSLIC_DEV_NAME "/dev/proslic%u"
#define PROSLIC_MAX_DEVS 1
#define PROSLIC_MAX_DEV_PATHNAME 20
/* end of user defined settings */

#define PROSLIC_API_DEMO_VERSION "0.0.1"
#define PROSLIC_API_TITLE        "ProSLIC API Demo"
#define PROSLIC_API_PREFIX       "PROSLIC_DEMO"
#define PROSLIC_API_MSG(FMT, arg...)     printf( "%s: " FMT "%c", PROSLIC_API_PREFIX,##arg, PROSLIC_EOL);

#define PROSLIC_REG_LINEFEED 30
#define PROSLIC_RAM_DCDC_STATUS 1551
/*****************************************************************************************************/

typedef struct
{
  uint8_t port_count;          /* sort of redundant... since this is a global to all ports */
  uint8_t total_channel_count; /* sort of redundant... since this is a global to all ports */
  uint8_t device_type;
  uint8_t channel_count;
  uint8_t base_channel_index; 
  int     fd;
} proslic_api_info_t;

typedef struct
{
  uint8_t current_port;
  uint8_t current_channel;
} proslic_api_demo_state_t;

/*****************************************************************************************************/
uint8_t get_preset_count(int fd, int channel, int ioctl_num);
int set_preset(int fd, int channel, int preset_value, int ioctl_num);
void change_preset(int fd, int channel, int ioctl_get_count, int ioctl_set);

uInt8 ReadReg(int fd, int channel, uInt8 address);
int WriteReg(int fd, int channel, uInt8 address, uInt8 data);
int WriteRAM(int fd, int channel, uInt16 address, ramData data);
ramData ReadRAM(int fd, int channel, uInt16 address);

void debug_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state);
void linefeed_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state);
void converter_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state);
void audio_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state);
void tonegen_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state);

#endif /* __PROSLIC_API_DEMO_HDR__ */

