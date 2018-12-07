/******************************************************************************
 * Copyright (c) 2014-2016 by Silicon Laboratories
 *
 * $Id: proslic_spidev.h 5663 2016-05-16 18:31:33Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 */

#ifndef SPI_TYPE_H
#define SPI_TYPE_H

#include "sivoice_example_os.h"

typedef SiVoice_CtrlIf_t ctrl_S;

/*
** Function: SPI_Init
**
** Description:
** Initializes the SPI interface
*/
int SPI_Init (ctrl_S *hSpi);


/*
** Function: SPI_Close
**
** Description:
** Shuts down the SPI interface
*/

int SPI_Close (ctrl_S *hSpi);

char *SPI_GetPortNum (ctrl_S *hSpi);

unsigned long SPI_GetVmbHandle(ctrl_S *hSpi);

/*
** Function: ctrl_ResetWrapper
**
** Description:
** Sets the reset pin of the ProSLIC
*/

int ctrl_ResetWrapper (void *hCtrl, int status);

/*
** register read
**
** Description:
** Reads ProSLIC registers
*/

unsigned char ctrl_ReadRegisterWrapper (void *hCtrl, uInt8 channel,
                                        uInt8 regAddr);

/*
** Function: ctrl_WriteRegisterWrapper
**
** Description:
** Writes ProSLIC registers
*/

int ctrl_WriteRegisterWrapper (void *hSpiGci, uInt8 channel,
                                uInt8 regAddr, uInt8 data);

/*
** Function: ctrl_WriteRAMWrapper
**
** Description:
** Writes ProSLIC RAM
*/

int ctrl_WriteRAMWrapper (void *hSpiGci, uInt8 channel,
                          uInt16 ramAddr, ramData data);

/*
** Function: ctrl_ReadRAMWrapper
**
** Description:
** Reads ProSLIC RAM
*/

ramData ctrl_ReadRAMWrapper  (void *hSpiGci, uInt8 channel,
                              uInt16 ramAddr);

#endif

