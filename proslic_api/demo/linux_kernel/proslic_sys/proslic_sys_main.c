/*
 * Copyright 2015, Silicon Labs
 * $Id: proslic_sys_main.c 5353 2015-11-13 20:05:22Z nizajerk $
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include "proslic_sys.h"

#define PROSLIC_SYS_SERVICES_VER   "0.0.1"
#define DRIVER_DESCRIPTION         "ProSLIC API compatible system services"
#define DRIVER_AUTHOR              "Silicon Laboratories"

int proslic_debug_setting = SILABS_DEFAULT_DBG;
int proslic_channel_count = SILABS_MAX_CHANNELS;

module_param(proslic_debug_setting, int, S_IRUSR | S_IWUSR |S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(proslic_debug_setting, "debug mode bitmask, 1 = TRC, 2 = DBG, 4 = ERR");
module_param(proslic_channel_count, int, S_IRUSR | S_IRGRP  );
MODULE_PARM_DESC(proslic_channel_count, "Number of actual channels on the platform");

/*****************************************************************************************************/

int init_module(void)
{
  int rc;

  printk( KERN_INFO "ProSLIC API system services module loaded, version: %s\n", PROSLIC_SYS_SERVICES_VER);
  printk( KERN_INFO "Debug = 0x%0x\n", proslic_debug_setting);

  if(proslic_channel_count > SILABS_MAX_CHANNELS)
  {
    printk( KERN_ERR "proslic_channel_count max is: %d, got: %d\n", SILABS_MAX_CHANNELS, proslic_channel_count);
    return -EINVAL;
  }
  
  rc = proslic_spi_setup();

  if(rc == 0)
  {
    /* TODO: any other init we may want tot do here... */
  }
  else
  {
    printk( KERN_ERR "proslic_spi_setup returned: %d\n", rc);
    return rc;
  }

  return 0;

}

void cleanup_module(void)
{
  proslic_spi_shutdown();
  printk( KERN_INFO "ProSLIC API system services module unloaded.\n");
}

MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("Dual MPL/GPL"); 

EXPORT_SYMBOL(proslic_spi_if);
EXPORT_SYMBOL(proslic_timer_if);
EXPORT_SYMBOL(proslic_get_channel_count);
EXPORT_SYMBOL(proslic_get_device_type);
EXPORT_SYMBOL(proslic_get_hCtrl);
