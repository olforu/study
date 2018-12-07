/******************************************************************************
 * Copyright (c) 2017 by Silicon Laboratories
 *
 * $Id: rspi_common.h 6106 2017-06-02 23:56:16Z ketian $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Labs, Inc.
 *
 */

#ifndef RSPI_COMMON_H
#define RSPI_COMMON_H

#ifdef __linux__
#include <termios.h>
#endif

#ifdef __MINGW32__
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <unistd.h>

#include "si_voice_datatypes.h"
#include "si_voice.h"

#ifndef RSPI_IS_EMBEDDED
#include "spi_pcm_vcp.h"
#endif

/* Definitions for array sizes */
#define RSPI_MAX_BUFFER_SIZE    1024
#define RSPI_MAX_STR_SIZE       32

/* Definitions used for checksum */
#define RSPI_CHECKSUM_LOCATION  4
#define RSPI_PACKET_CORRUPTED   5
#define RSPI_READ_PCKT_SIZE     6
#define RSPI_IS_CORRUPTED       1
#define RSPI_TIMED_OUT          2

/* Definitions to get and receive VMB functions */
#define RSPI_FUNCTION           0
#define RSPI_FUNCTION_VALUE     0
#define RSPI_BYTE_SHIFT         4

/* Definitions for VMB functions */
#define RSPI_CLIENT_DISCONNECT            0
#define RSPI_READ_EEEPROM_BYTE            1
#define RSPI_WRITE_EEPROM_BYTE            2
#define RSPI_GET_FIRMWARE_ID              3
#define RSPI_SPI_INIT                     4
#define RSPI_SPI_CLOSE                    5
#define RSPI_SPI_GET_PORT_NUM             6
#define RSPI_SPI_GET_VMB_HANDLE           7
#define RSPI_CTRL_RESET_WRAPPER           8
#define RSPI_CTRL_READ_REGISTER_WRAPPER   9
#define RSPI_CTRL_WRITE_REGISTER_WRAPPER  10
#define RSPI_CTRL_READ_RAM_WRAPPER        11
#define RSPI_CTRL_WRITE_RAM_WRAPPER       12
#define RSPI_SPI_SET_SCLK_FREQ            13
#define RSPI_SPI_SET_PCLK_FREQ            14
#define RSPI_PCM_SET_FSYNC_TYPE           15
#define RSPI_PCM_SET_SOURCE               16
#define RSPI_PCM_ENABLE_XCONNECT          17
#define RSPI_PCM_READ_SOURCE              18
#define RSPI_SPI_SELECT_CS                19
#define RSPI_SET_PCM_SOURCE_EXP           20
#define RSPI_SPI_SELECT_FORMAT            21
#define RSPI_GET_EVB_ID                   22
#define RSPI_GET_BIT_MASK                 23

/*
** Function: RspiPrint
**
** Description:
** Disables printf if there is no standard output
**
*/
void RspiPrint(char* message);

/*
** Function: RspiChecksum
**
** Description:
** Determines the checksum value
**
*/
uInt8 RspiChecksum(char* buf);


#endif
