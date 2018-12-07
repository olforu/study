/******************************************************************************
 * Copyright (c) 2014-2017 by Silicon Laboratories
 *
 * $Id: proslic_vmb2_posix.c 6757 2017-08-29 18:43:04Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements the VMB2 SPI communication for Linux.  It uses the environment
 * variable SIVOICE_SPI_IF to determine which USB serial port to use.  The default is
 * /dev/ttyUSB0
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "si_voice_datatypes.h"
#include "si_voice.h"
#include "spi_pcm_vcp.h"

#include "proslic_vmb2.h"

#define SI_USB_SPI_DELAY 50 /* in uSec */
#define VMB2_PORT_ENV "SIVOICE_SPI_IF"
/* #define SIVOICE_VMB2_DBG  */ /* Don't turn on unless you want to see a lot of "raw" commands */

#define SIVOICE_DEFAULT_COMPORT "/dev/ttyUSB0"
static char *vcp_port;

/******************************************************************************
 * Initialize the SPI communications port - basically, open the virtual
 * comm port (VCP) and configure for a particular data rate.
 *
 * Returns: 0 = OK
 *
 * NOTE: Not re-entrant, should only be called 1 time per execution of software.
 */

int SiVoiceOpenIF(ctrl_S *interfacePtr)
{
  struct termios new_termios;
  int fd;

  if( (vcp_port = getenv(VMB2_PORT_ENV) ) == NULL )
  {
    vcp_port = SIVOICE_DEFAULT_COMPORT;
  }

  printf("Using %s for VMB2 communications\n", vcp_port);

  fd = open(vcp_port, O_RDWR|O_NOCTTY);

  if( fd < 0 )
  {
    perror(vcp_port);
    return RC_SPI_FAIL;
  }

  if(!isatty(fd) )
  {
    perror(vcp_port);
    return RC_SPI_FAIL;
  }

  /* Save settings */
  tcgetattr(fd, &( (interfacePtr)->old_termios) );

  SIVOICE_MEMSET(&new_termios, 0, sizeof(new_termios));

  cfsetospeed(&new_termios, B115200);
  new_termios.c_cflag =  CS8 | CLOCAL | CREAD;
  new_termios.c_iflag = IGNPAR;
  new_termios.c_cc[VMIN] = 1; /* read() must wait for at least 1 char */

  tcsetattr(fd, TCSANOW, &new_termios);
  tcflush(fd,TCIOFLUSH);

  interfacePtr->fd = fd;

  return RC_NONE;
}

/******************************************************************************
 * Tear down any resources we've allocated for the VMB2...
 */
int SiVoiceCloseIF(ctrl_S *interfacePtr)
{
  int fd = interfacePtr->fd;

  tcsetattr(fd, TCSANOW, &((interfacePtr)->old_termios) );
  ((ctrl_S *)interfacePtr)->fd = -1;
  return (close(fd) == 0);
}

/******************************************************************************
 * SiVoiceVMB2_SendCmd
 */

int SiVoiceVMB2_SendCmd(ctrl_S *interfacePtr, uInt8 *buffer)
{
  int fd = interfacePtr->fd;
  int byteCount;


  buffer[SIVOICE_VMB2_TERM_CID_BYTE] |= SIVOICE_VMB2_CMD_TERMINATOR;
#ifdef SIVOICE_VMB2_DBG
  {
    int myCounter;
    printf("%s(%d): ", __FUNCTION__, __LINE__);
    for(myCounter = 0; myCounter < SIVOICE_VMB2_BUFFER_SZ; myCounter++)
    {
      printf("%02x ", buffer[myCounter]);
    }
    printf("\n");
  }
#endif

  byteCount = write(fd, buffer, SIVOICE_VMB2_BUFFER_SZ);
  usleep(SI_USB_SPI_DELAY);
  read(fd, buffer,
       SIVOICE_VMB2_BUFFER_SZ); /* flush response for now. TODO: need to check response */
  tcdrain(fd);
  tcflush(fd, TCIFLUSH);

  if( byteCount != SIVOICE_VMB2_BUFFER_SZ )
  {
    return RC_SPI_FAIL;
  }
  else
  {
    return RC_NONE;
  }
}

/******************************************************************************
 * SiVoiceVMB2_ReadData - NOTE: no check if we never get all the data,
 *                         in this case, we'll sit here forever...
 */

int SiVoiceVMB2_ReadData(ctrl_S *interfacePtr, uInt8 *buffer)
{
  unsigned int i = 0;
  unsigned int j;
  int byteCount;
  int fd = ((ctrl_S *)interfacePtr)->fd;

  buffer[SIVOICE_VMB2_TERM_CID_BYTE] |= SIVOICE_VMB2_CMD_TERMINATOR;
  byteCount = write(fd, buffer, SIVOICE_VMB2_BUFFER_SZ);
  usleep(SI_USB_SPI_DELAY);

  if( byteCount != SIVOICE_VMB2_BUFFER_SZ )
  {
    return RC_SPI_FAIL;
  }

  do
  {
    j = i;
    i += read(fd, &(buffer[j]), SIVOICE_VMB2_BUFFER_SZ);
  }
  while(i < SIVOICE_VMB2_BUFFER_SZ);

  /* TODO: check response */

#ifdef SIVOICE_VMB2_DBG
  {
    int myCounter;
    printf("%s(%d): ", __FUNCTION__, __LINE__);
    for(myCounter = 0; myCounter < SIVOICE_VMB2_BUFFER_SZ; myCounter++)
    {
      printf("%02x ", buffer[myCounter]);
    }
    printf("\n");
  }
#endif

  return RC_NONE;
}

char *SPI_GetPortNum(void *interfacePtr)
{
  SILABS_UNREFERENCED_PARAMETER(interfacePtr);
  return vcp_port;
}

