/*
 * Copyright 2015, Silicon Labs
 * $Id: proslic_sys_cfg.h 5150 2015-10-05 18:56:06Z nizajerk $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, you can select the MPL license:
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
 * If a copy of the MPL was not distributed with this file, You can obtain one at 
 * https://mozilla.org/MPL/2.0/.
 *
 * File purpose: provide system services layer to the ProSLIC API kernel module.
 *
 */

#ifndef __PROSLIC_SYS_CFG_HDR__
#define __PROSLIC_SYS_CFG_HDR__ 1
/* 
 * Begin user defined build options 
 *
 * TODO: move this to a .kconfig option menu
 */

#define SILABS_BITS_PER_WORD    32        /* MUST be either 8, 16, or 32 */
#define SILABS_SPI_RATE         4000000  /* In terms of Hz */
#define SILABS_MAX_RAM_WAIT     1000     /* In terms of loop count */
#define SILABS_RESET_GPIO       1020
#define SILABS_MAX_CHANNELS     4
#define SILABS_RAM_BLOCK_ACCESS 1        /* Define this if you wish to use a single block to write to RAM, may not work on all systems */
#define SILABS_DEFAULT_DBG      7
#define SILABS_MIN_MSLEEP_TIME  30       /* threshold to call mdelay vs. msleep() */
#define SILABS_MSLEEP_SLOP      10       /* If the msleep() function is off by a constant value, set this number positive if it's too long or negative number for too short - terms of mSec */
#define PROSLIC_XFER_BUF        4        /* How many bytes to send in 1 transfer.  Either 1,2, or 4.  If setting SILABS_BITS_PER_WORD to 16 or 32, you MUST set this either 2 or 4 */
/* End of user defined section */

#endif /* __PROSLIC_SYS_CFG_HDR__ */

