/******************************************************************************
 * Copyright (c) 2014-2018 by Silicon Laboratories
 *
 * $Id: spi_pcm_vcp.h 7065 2018-04-12 20:24:50Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 */

#ifndef SPI_TYPE_H
#define SPI_TYPE_H

#ifdef __linux__
#include <termios.h>
#endif

/* General */
#define EXT_PCM_SRC       0
#define INT_PCM_SRC       1
#define FSYNC_SAME_AS_PCM 0
#define FSYNC_EXT_ONLY    1

/* VMB PCLK */
#define VMB_PCLK_8192     128
#define VMB_PCLK_4096     64
#define VMB_PCLK_2048     32
#define VMB_PCLK_1024     16
#define VMB_PCLK_512      8
#define VMB_PCLK_256      4
#define VMB_PCLK_768      12
#define VMB_PCLK_1536     24

/* VMB2 PCLK */
#define VMB2_PCLK_8192    0
#define VMB2_PCLK_4096    1
#define VMB2_PCLK_2048    2
#define VMB2_PCLK_1024    3
#define VMB2_PCLK_512     4
#define VMB2_PCLK_1536    5
#define VMB2_PCLK_768     6
#define VMB2_PCLK_1544    7

/* VMB2 SCLK */
#define VMB2_SCLK_12000   1
#define VMB2_SCLK_8000    2
#define VMB2_SCLK_4000    5
#define VMB2_SCLK_2000    11
#define VMB2_SCLK_1000    23

/* VMB2 FSYNC */
#define VMB2_FSYNC_SHORT  0
#define VMB2_FSYNC_LONG   1

/* VMB2 PCM XCONNECT ENABLE */
#define VMB2_XCONN_DISABLE 0x00
#define VMB2_XCONN_ENABLE 0x01


/*
** EEPROM Read
*/
unsigned char  ReadEEPROMByte(unsigned short eAddr);

/*
** EEPROM Write
*/
void WriteEEPROMByte(unsigned short eAddr, unsigned char eData);

/*
** Firmware ID
*/
unsigned int GetFirmwareID();

/*
** SPI/GCI structure
*/
typedef struct
{
#ifdef __linux__
  int fd;
  struct termios old_termios;
#else /* cygwin */ /* TODO: figure out if we need the items below... */
  int portNum;
  unsigned long handle;
#endif
} ctrl_S;

/*
** Function: SPI_Init
**
** Description:
** Initializes the SPI interface and toggles reset
*/
int SPI_Init (ctrl_S *hSpi);

/*
** Function: SPI_Close
**
** Description:
** Shuts down the SPI interface
*/

int SPI_Close (ctrl_S *hSpi);

char *SPI_GetPortNum (void *hSpi);

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

int SPI_setSCLKFreq(unsigned char sclk_freq_select);

int PCM_setPCLKFreq(unsigned char pclk_freq_select);

int PCM_setFsyncType(unsigned char fsync_select);

int PCM_setSource(unsigned char pcm_internal_select);

int PCM_enableXConnect(unsigned char enable);

unsigned char PCM_readSource();

void SPI_SelectCS(unsigned char cs);

unsigned short setPcmSourceExp(unsigned short internal,int freq,int extFsync);

void SPI_SelectFormat(unsigned char fmt);

#endif
