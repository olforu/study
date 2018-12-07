/*
 * Copyright 2015, Silicon Labs
 * $Id: proslic_sys_timer.c 5353 2015-11-13 20:05:22Z nizajerk $
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

#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "proslic_sys.h"

/*****************************************************************************************************/
static int proslic_sys_delay(void *hTimer, int timeInMsec)
{
	if(likely(timeInMsec < SILABS_MIN_MSLEEP_TIME))
	{
		mdelay(timeInMsec);
	}
	else
	{
		msleep((timeInMsec-SILABS_MSLEEP_SLOP));
	}
	return PROSLIC_SPI_OK;
}

/*****************************************************************************************************/
/* Code assumes time value has been allocated */
static int proslic_sys_getTime(void *hTimer, void *time)
{
	if(time != NULL)
	{
		((proslic_timeStamp *)time)->timerObj = current_kernel_time();
		return PROSLIC_SPI_OK;
	}
	else
	{
		return PROSLIC_TIMER_ERROR;
	}
}
/*****************************************************************************************************/
 
static int proslic_sys_timeElapsed(void *hTimer, void *startTime, int *timeInMsec)
{
	if( (startTime != NULL) && (timeInMsec != NULL) )
	{
		struct timespec now = current_kernel_time();
		struct timespec ts_delta = timespec_sub(now, ((proslic_timeStamp *) startTime)->timerObj);
		*timeInMsec = ( (ts_delta.tv_sec *1000) + (ts_delta.tv_nsec / NSEC_PER_MSEC) );
		return PROSLIC_SPI_OK;
	}
	else
	{
		return PROSLIC_TIMER_ERROR;
	}
}


proslic_timer_fptrs_t proslic_timer_if =
{
	proslic_sys_delay,
	proslic_sys_timeElapsed,
	proslic_sys_getTime
};
