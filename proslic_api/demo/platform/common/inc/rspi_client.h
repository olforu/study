/******************************************************************************
 * Copyright (c) 2017-2018 by Silicon Laboratories
 *
 * $Id: rspi_client.h 6106 2017-06-02 23:56:16Z ketian $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Labs, Inc.
 *
 */

#ifndef RSPI_H
#define RSPI_H

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

#include "rspi_common.h"

#define RSPI_CLIENT_VER        "1.0"
/* TCP UDP CONNECTION */
#define RSPI_BUF_ALMOST_FULL   1000 /* should always be less than MAX_BUFFER_SIZE */
#define RSPI_WRITE_BUF_TIMEOUT 1000 /* time in microseconds */
#define RSPI_WIN_WRITE_TIMEOUT 1   /* time in milliseconds */
#define RSPI_WIN_TIMER_ID      101
#define RSPI_UDP_TIMEOUT       100000 /* time in microseconds */
#define RSPI_MAX_READ_TRIES    5

/* CLIENT FLAGS */
/* flags 0 - 3 are reserved to determine bit shfting */
#define RSPI_COPY_STR_FLAG     4 /* flag that copies string to passed in array
                                    mainly used for reading COM PORT_NUM */
#define RSPI_FLUSH_BUF_FLAG    5 /* flag that flushes write register buffer */
#define RSPI_OPEN_SOCK_FLAG    6 /* flag tells client to open socket */
#define RSPI_CLOSE_SOCK_FLAG   7 /* flag tells client to close socket */

#define RSPI_USER_EXISTS       '\n'

/*
** Function: RspiSignalHandler/RspiTimerProc
**
** Description:
** triggers when it is time to send data in write buffer to client
*/
#ifndef __MINGW32__
void RspiSignalHandler(int x);
#else
VOID CALLBACK RspiTimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
#endif

/*
** Function: ConnectServer
**
** Description:
** Sends data to server using TCP/IP connection
*/
uInt32 RspiServerInterface(uInt8 function[], uInt8 size, uInt8 flag, char* buf);

/*
** Function: FlushWriteBuffer
**
** Description:
** Flushes buffer that contanis register writes to the server
*/
uInt8 RspiFlushWriteBuffer(uInt32* cilentTCPSocket, sigset_t* set,
                           uInt32* timer_handle_, uInt8* writeRegBuffer,
                           uInt16* writeRegCounter);

/*
** Function: Connect_Server
**
** Description:
** Connects to the server using TCP/IP
*/
void RspiInitializeConnection(uInt32* clientTCPSocket, uInt32* clientUDPSocket,
                          struct sockaddr_in* serverAddr, socklen_t* addr_size,
                          char* IP_ADDRESS, uInt16 PORT_NUM);

/*
** Function: GetDataFromTXT
**
** Description:
** Gets TCP/UDP configuration data from .txt file instead of .h
*/
#ifdef RSPI_TXT_CFG
void RspiGetDataFromTXT(char* IP_ADDRESS, uInt16* PORT_NUM,
                        char* username, char* password);
#endif

/*
** Function: get_evb_id
**
** Description:
** Determines which EVB is connected to the server
*/
void RspiGetEvbId(char* buffer, uInt8 size);

#endif

