/*
** Copyright (c) 2012 - 2018 by Silicon Laboratories
**
** $Id: spi_main.h 7065 2018-04-12 20:24:50Z nizajerk $
**
** ProSLIC API Demonstration Code
**
** Author(s):
** cdp
**
** Distributed by:
** Silicon Laboratories, Inc
**
** File Description:
** This file provides provides access to SPI driver for
** all proslic motherboards (VMB, VMB2), SPIDev and Linux kernel.
**
**
*/
#include "proslic.h"

#ifndef __SPI_MAIN_HDR__
#define __SPI_MAIN_HDR__ 1

#if defined(VMB2)
#include "spi_pcm_vcp.h"
#define VMB_INFO "VMB2"

#elif defined(RSPI)
#include "spi_pcm_vcp.h"
#include "rspi_client.h"
#define VMB_INFO "RSPI"

#elif defined(VMB1)
#include "proslic_spiGci_usb.h"
#define VMB_INFO "VMB1"

#elif defined(LINUX_SPIDEV)
#define VMB_INFO "Linux_SPIDEV"
#include "proslic_spidev.h"

#elif defined(PROSLIC_LINUX_KERNEL)
#include "proslic_sys.h"

#else
#error "Platform type unknown"
#endif

#if defined(VMB2) || defined(RSPI) || defined(VMB1) || defined (LINUX_SPIDEV)
/*
 * Purpose: interactively ask user for any parameter changes from the default.  Can be empty
 * for customer platforms, but for the VMB1/VMB2, we can change the SCLK & PCLK settings.
 */
void vmbSetup(controlInterfaceType *);
#endif

/*
 * Purpose: Allocate/open any system resources needed to run SPI and timers.  Associates
 * the customer/platform implemented SPI, Timer and Semaphore functions with the API.
 */
void initControlInterfaces(controlInterfaceType *ProHWIntf, void *spiIf,
                           void *timerObj);

/*
 *  Purpose: free any system resources used in the application (timers, SPI)  - should be called prior to
 *  SiVoice_destroyControlInterface()
 */
void destroyControlInterfaces(controlInterfaceType *ProHWIntf);

#endif
