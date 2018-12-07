/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: proslic_linux_api.h 5150 2015-10-05 18:56:06Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** char device interface file for the ProSLIC API core module.
** This defines the bridge between "userspace" and kernel space.
**
**
*/

#ifndef __PROSLIC_LINUX_API__
#define __PROSLIC_LINUX_API__ 1

#include <linux/ioctl.h>

typedef struct {
  uint8_t channel;

  /* the union below is not used for ALL IOCTL's and is ignored in those cases..*/
  union {
    uint8_t   reg_address;
    uint16_t  ram_address;
    uint8_t   index;
  };

  union {
    uint8_t   byte_value;
    uint32_t  word_value;
    int16_t   int_value;
    uint8_t   byte_values[4];
    uint16_t  int_values[2];
  };
} proslic_chan_if;

#define PROSLIC_MAGIC_IOCTL_NUM 123 /* MUST be a 8 bit number */

/*
 * Get back the number of total channels. 
 */
#define PROSLIC_IOCTL_GET_CHAN_COUNT _IOR(PROSLIC_MAGIC_IOCTL_NUM, 0, uint8_t *)
#define PROSLIC_IOCTL_GET_PORT_COUNT _IOR(PROSLIC_MAGIC_IOCTL_NUM, 1, uint8_t *)
#define PROSLIC_IOCTL_GET_PORT_CHAN _IOR(PROSLIC_MAGIC_IOCTL_NUM, 2, uint8_t *)

/*
 * Get the device type for a specific channel. enum in byte_value
 */
#define PROSLIC_IOCTL_GET_DEV_TYPE _IOWR(PROSLIC_MAGIC_IOCTL_NUM, 3, proslic_chan_if *)


#define PROSLIC_IOCTL_READ_REG         _IOWR(PROSLIC_MAGIC_IOCTL_NUM, 4, proslic_chan_if *)
#define PROSLIC_IOCTL_WRITE_REG        _IOW(PROSLIC_MAGIC_IOCTL_NUM, 5, proslic_chan_if *)

#define PROSLIC_IOCTL_READ_RAM         _IOWR(PROSLIC_MAGIC_IOCTL_NUM, 6, proslic_chan_if *)
#define PROSLIC_IOCTL_WRITE_RAM        _IOW(PROSLIC_MAGIC_IOCTL_NUM, 7, proslic_chan_if *)

#define PROSLIC_IOCTL_SET_LINE_STATE   _IOW(PROSLIC_MAGIC_IOCTL_NUM, 8, proslic_chan_if *)

#define PROSLIC_IOCTL_GET_DCFEED_COUNT _IOR(PROSLIC_MAGIC_IOCTL_NUM, 9, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_DCFEED       _IOW(PROSLIC_MAGIC_IOCTL_NUM, 10, proslic_chan_if *)

#define PROSLIC_IOCTL_SET_CONVERTER_STATE _IOW(PROSLIC_MAGIC_IOCTL_NUM, 11, proslic_chan_if *)

#define PROSLIC_IOCTL_GET_RINGER_COUNT _IOR(PROSLIC_MAGIC_IOCTL_NUM, 12, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_RINGER       _IOW(PROSLIC_MAGIC_IOCTL_NUM, 13, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_RINGER_STATE _IOW(PROSLIC_MAGIC_IOCTL_NUM, 14, proslic_chan_if *)

#define PROSLIC_IOCTL_GET_TONE_COUNT   _IOR(PROSLIC_MAGIC_IOCTL_NUM, 15, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_TONE         _IOW(PROSLIC_MAGIC_IOCTL_NUM, 16, proslic_chan_if *)
#define PROSLIC_IOCTL_TONE_ON_OFF      _IOW(PROSLIC_MAGIC_IOCTL_NUM, 17, proslic_chan_if *)

#define PROSLIC_IOCTL_GET_ZSYNTH_COUNT _IOR(PROSLIC_MAGIC_IOCTL_NUM, 18, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_ZSYNTH       _IOW(PROSLIC_MAGIC_IOCTL_NUM, 19, proslic_chan_if *)

#define PROSLIC_IOCTL_SET_RXTX_TS      _IOW(PROSLIC_MAGIC_IOCTL_NUM, 20, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_PCM_ON_OFF   _IOW(PROSLIC_MAGIC_IOCTL_NUM, 21, proslic_chan_if *)
#define PROSLIC_IOCTL_GET_PCM_COUNT    _IOR(PROSLIC_MAGIC_IOCTL_NUM, 22, proslic_chan_if *)
#define PROSLIC_IOCTL_SET_PCM          _IOR(PROSLIC_MAGIC_IOCTL_NUM, 23, proslic_chan_if *)

#define PROSLIC_IOCTL_COUNT 23 

#define PROSLIC_CHARDEV_BASE_NAME "proslic"

#endif

