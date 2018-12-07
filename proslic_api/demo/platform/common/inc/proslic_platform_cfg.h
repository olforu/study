/*
** Copyright (c) 2015-2017 by Silicon Laboratories
**
** $Id: proslic_platform_cfg.h 6801 2017-09-14 22:40:11Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** Configuration options for the platform example code.
**
*/

#ifndef __PROSLIC_PLATFORM_CUSTOM_HDR__
#define __PROSLIC_PLATFORM_CUSTOM_HDR__ 1

/******************* TIMER ********************/

#define SILABS_USE_TIMEVAL /* alternative: timespec */

#ifdef __linux__
#define SILABS_CLOCK CLOCK_MONOTONIC_RAW /* see clock_gettime() for options */
#else
#define SILABS_CLOCK CLOCK_MONOTONIC /* see clock_gettime() for options */
#endif

/***************** SPI USERSPACE *****************/
#ifdef __linux__
#ifndef SPIDEV_NO
#define SPIDEV_NO "3.0"
#endif
#define SILABS_SPIDEV "/dev/spidev"SPIDEV_NO
#ifndef RESET_GPIO
#define RESET_GPIO "1020"
#endif
#define LINUX_GPIO   "/sys/class/gpio/gpio"RESET_GPIO"/value" /* Which GPIO pin to use for reset? */
#define SILABS_MAX_SPI_SPEED 9000000 /* This is actually not the maximum physical speed, just a "safe" max. */
#define SILABS_SPI_RATE      1000000 /* For IOC_XFER */
#define PROSLIC_MAX_RAM_WAIT  100


/* #define SPI_TRC printf */ /* Uncomment this line and comment the one below to get SPI debug */
#define SPI_TRC(...)
//#define SILABS_USE_IOC_XFER 1  /* Set this if your SPIDev implementation does not support read/write and just ioctl transfers */

#define SILABS_BITS_PER_WORD 32 /* MUST be either 8, 16 or 32 */
#define SILABS_RAMWRITE_BLOCK_MODE 1 /* If enabled, will send 24 bytes down vs. register access mode, in some systems this is more efficient */

#define SILABS_BYTE_LEN (SILABS_BITS_PER_WORD/8) /* Should be able to set this independent of BITS_PER_WORD */
#endif

#endif

