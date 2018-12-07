/*
** Copyright 2017, Silicon Labs
** rspi_client.c
**
** $Id: rspi_server.c 6106 2017-08-01 23:56:16Z ketian $
** Rspi client implementation file
**
** Author(s):
** ketian
**
** Distributed by:
** Silicon Labs, Inc
**
** File Description:
** This is the implementation of Rspi Client to be used with Rspi Server
**
**
*/

#include "rspi_client.h"

#ifndef RSPI_TXT_CFG
#include "rspi_client_cfg.h"
#endif

#define DRVR_PREFIX "RSPI:"

#ifdef SI_SPI_DEBUG
#ifdef SI_ENABLE_LOGGING
/* NOTE: can loosely convert into a GUI script with grep VMB2 <logfn> |cut -b6- */
#define DRVR_PRINT(...) fprintf(SILABS_LOG_FP,__VA_ARGS__);fflush(SILABS_LOG_FP)
#else
#define DRVR_PRINT LOGPRINT
#endif /* LOGGING */
#else
#define DRVR_PRINT(...)
#endif

/*
** Function: RspiSignalHandler/RspiTimerProc
**
** Description:
** Triggers with alarm expires and causes write reg buffer to flush
**
** Input Parameters:
** Signal Number (Alarm is 14)
**
** Return:
** none
**
*/

#ifndef __MINGW32__
void RspiSignalHandler(int x)
{
  /* Set a flag to flush the current write register buffer */
  SILABS_UNREFERENCED_PARAMETER(x);
  (void) RspiServerInterface(NULL, 0, RSPI_FLUSH_BUF_FLAG, NULL);
}
#else
VOID CALLBACK RspiTimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
  SILABS_UNREFERENCED_PARAMETER(lpParameter);
  SILABS_UNREFERENCED_PARAMETER(TimerOrWaitFired);
  (void) RspiServerInterface(NULL, 0, RSPI_FLUSH_BUF_FLAG, NULL);
}
#endif

/*
** Function: RspiServerInterface
**
** Description:
** Opens a TCP/IP connection to the server with function for server to return
** Also takes in function calls from higher level programs and passes them to
** the server. Function then returns the value of the function call back to the
** higher level program.
**
** Input Parameters:
** IP Address, Port Number, Function, Flag
** Function includes the integer value of which functin to call
** Flags 0-3 determines how many indexes of the buffer to return
** Other flags determine whether to open/closer socket and if to copy
** the received buffer as a string
**
** Return:
** Value from the function called by the server
**
*/

uInt32 RspiServerInterface(uInt8 function[], uInt8 size, uInt8 flag, char* buf)
{
  static uInt32 clientTCPSocket, clientUDPSocket;
  static struct sockaddr_in serverAddr;
  static socklen_t addr_size;
  static uInt32 timer_handle_;
  sigset_t set;
  uInt32 returnValue = 0;
  static uInt8 writeRegBuffer[RSPI_MAX_BUFFER_SIZE] = {0};
  static uInt16 writeRegCounter = 0;
  uInt8 buffer[RSPI_MAX_BUFFER_SIZE];
  uInt8 i = 0;
  fd_set readfds, masterfds;
  struct timeval tv;

  #ifndef __MINGW32__
  struct sigaction act;
  #endif
  #ifdef RSPI_TXT_CFG
    char rspi_ip_address[RSPI_MAX_STR_SIZE];
    char rspi_username[RSPI_MAX_STR_SIZE];
    char rspi_password[RSPI_MAX_STR_SIZE];
    uInt16 port_num;
    RspiGetDataFromTXT(rspi_ip_address, &rspi_port_num, rspi_username,
                       rspi_password);
  #endif


  tv.tv_sec = 0;
  tv.tv_usec = RSPI_UDP_TIMEOUT;
  SIVOICE_MEMSET(buffer, 0, sizeof(buffer)); /* Clear buffer */

  /* Flag opens client socket and determines if client meets server conditions */
  if(flag == RSPI_OPEN_SOCK_FLAG)
  {
    RspiInitializeConnection(&clientTCPSocket, &clientUDPSocket, &serverAddr,
                             &addr_size, rspi_ip_address, rspi_port_num);

    /* Send server username, password, version */
    snprintf((char*) buffer, RSPI_MAX_BUFFER_SIZE, "%s %s %s",
             RSPI_CLIENT_VER, rspi_username, rspi_password);
    if(send(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0) < 0)
    {
      RspiPrint("Server disconnected\n");
      return RSPI_CONNECTION_FAILED;
    }
    if(recv(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0) <= 0)
    {
      RspiPrint("Server disconnected\n");
      return RSPI_CONNECTION_FAILED;
    }
    if(buffer[RSPI_FUNCTION] == RSPI_USER_EXISTS)
    {
      RspiPrint(strcat((char*) &buffer[1], "\n"));
      return RSPI_CONNECTION_FAILED;
    }
    SIVOICE_MEMSET(buffer, 0, sizeof(buffer));
    flag = 0; /* So we can get the SPI init return value back */
  }
  #ifndef __MINGW32__

    /* Set up signal for when alarm triggers */
    act.sa_handler = RspiSignalHandler;
    act.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &act, 0);
    if (sigaddset(&set, SIGALRM) == -1)
    {
      RspiPrint("Could not set up signal blocking\n");
    }
  #endif

  /* Sends mutliple writes to the server */
  if(flag == RSPI_FLUSH_BUF_FLAG)
  {
    /* Flush buffer on time out */
    return RspiFlushWriteBuffer(&clientTCPSocket, &set, &timer_handle_,
                                writeRegBuffer, &writeRegCounter);
  }

  /* If the function call to server is a write add it to the write buffer
     If the buffer is full then send it to the server and sets up timer*/

  if(function[RSPI_FUNCTION] == RSPI_CTRL_WRITE_REGISTER_WRAPPER ||
     function[RSPI_FUNCTION] == RSPI_CTRL_WRITE_RAM_WRAPPER)
  {
    /* If there are successive writes then store them in writeRegBuffer */
    for(i = 0; i < size; i++)
    {
      writeRegBuffer[writeRegCounter] = function[i];
      writeRegCounter++;
    }

    if(writeRegCounter > RSPI_BUF_ALMOST_FULL)
    {
      /* If the buffer is almost full then send it to the server */
      return RspiFlushWriteBuffer(&clientTCPSocket, &set, &timer_handle_,
                                  writeRegBuffer, &writeRegCounter);
    }
    else
    {
      #ifdef __MINGW32__
        /* Alarm system for Windows */
        if(timer_handle_ != 0)
        {
          DeleteTimerQueueTimer(NULL, (HANDLE)timer_handle_, NULL);
          timer_handle_ = 0;
        }
        CreateTimerQueueTimer((HANDLE)&timer_handle_, NULL, RspiTimerProc, NULL,
                              RSPI_WIN_WRITE_TIMEOUT, 0, WT_EXECUTEDEFAULT);
      #else
        ualarm(RSPI_WRITE_BUF_TIMEOUT, 0); /* alarm for 1ms */
      #endif
    }
    return 0;
  }
  else
  {
    /* Send data in write buffer before any other function calls */
    if(writeRegBuffer[RSPI_FUNCTION] == RSPI_CTRL_WRITE_REGISTER_WRAPPER ||
       writeRegBuffer[RSPI_FUNCTION] == RSPI_CTRL_WRITE_RAM_WRAPPER)
    {
      returnValue = RspiFlushWriteBuffer(&clientTCPSocket, &set, &timer_handle_,
                                         writeRegBuffer, &writeRegCounter);
      if (returnValue == RSPI_CONNECTION_FAILED)
      {
        return returnValue;
      }
    }
  }
  if(function[RSPI_FUNCTION] == RSPI_CTRL_READ_REGISTER_WRAPPER ||
     function[RSPI_FUNCTION] == RSPI_CTRL_READ_RAM_WRAPPER)
  {
    /** If RAM or reg read then send data over UDP **/
    SIVOICE_MEMSET(buffer, 0, sizeof(buffer));
    SIVOICE_MEMCPY(buffer, function, size * sizeof(uInt8));

    /* Create a checksum */
    uInt8 checksum = RspiChecksum((char*) buffer);

    /* Tries to read from server multiple times in case of poor connection */
    for(i = 0; i <= RSPI_MAX_READ_TRIES; i++)
    {
      FD_ZERO(&masterfds);
      FD_SET(clientUDPSocket, &masterfds);
      SIVOICE_MEMCPY(&readfds, &masterfds, sizeof(fd_set));
      SIVOICE_MEMCPY(buffer, function, size * sizeof(uInt8));
      buffer[RSPI_CHECKSUM_LOCATION] = checksum;
      /*printf("UDP Function Call: %d %d %d\n", buffer[0], buffer[1], buffer[2]);*/
      #ifndef __MINGW32__
        sigprocmask(SIG_BLOCK, &set, NULL);
      #endif

      if(sendto(clientUDPSocket, (char*) buffer, RSPI_READ_PCKT_SIZE, 0,
         (struct sockaddr*)&serverAddr, addr_size) <= 0)
      {
        RspiPrint("Server disconnected\n");
        return RSPI_CONNECTION_FAILED;
      }

      /* Listens on socket to receive data and sets up timeout */
      if(select(clientUDPSocket + 1, &readfds, NULL, NULL, &tv) < 0)
      {
        RspiPrint("Error on select\n");
      }
      if (FD_ISSET(clientUDPSocket, &readfds))
      {
        /* read from the socket */
        recvfrom(clientUDPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0,
                 (struct sockaddr*)&serverAddr, &addr_size);
      }
      else
      {
        /* the socket timed out */
        if(i == RSPI_MAX_READ_TRIES)
        {
          RspiPrint("Server disconnected\n");
          return RSPI_CONNECTION_FAILED;
        }
        continue;
      }

      #ifndef __MINGW32__
        sigprocmask(SIG_UNBLOCK, &set, NULL);
      #endif

      /* Check checksum */
      /*printf("UDP Function Receive: %d %d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);*/
      if(buffer[RSPI_PACKET_CORRUPTED] == RSPI_TIMED_OUT)
      {
        RspiPrint("Timed out\n");
        return RSPI_CONNECTION_FAILED;
      }
      if(buffer[RSPI_PACKET_CORRUPTED] == RSPI_IS_CORRUPTED)
      {
        if(i == RSPI_MAX_READ_TRIES)
        {
          RspiPrint("Failed to read value correctly\n");
        }
        continue;
      }
      else
      {
        /* Verify checksum */
        if(buffer[RSPI_CHECKSUM_LOCATION] != RspiChecksum((char*) buffer))
        {
          /* If the checksum fails then resend read request */
          if(i == RSPI_MAX_READ_TRIES)
          {
            RspiPrint("Failed to read value correctly\n");
          }
          continue;
        }
        else
          break;
      }
    }

    /* Recombine the read values in the buffer and return it */
    for(i = 0; i <= flag; i++)
    {
      returnValue |= ((uInt32) buffer[i]) << (8 * i);
    }
    return returnValue;
  }

  #ifndef __MINGW32__
    sigprocmask(SIG_BLOCK, &set, NULL);
  #endif

  SIVOICE_MEMCPY(buffer, function, size * sizeof(uInt8));
  /* printf("Client Function Call: %d %d %d\n", buffer[0], buffer[1], buffer[2]); */
  if(send(clientTCPSocket, (char*) buffer, size, 0) < 0)
  {
    RspiPrint("Server disconnected\n");
    return RSPI_CONNECTION_FAILED;
  }

  if(recv(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0) <= 0)
  {
    RspiPrint("Server disconnected\n");
    return RSPI_CONNECTION_FAILED;
  }
  /* printf("Client Function Receive: %d %d %d\n", buffer[0], buffer[1], buffer[2]); */
  #ifndef __MINGW32__
    sigprocmask(SIG_UNBLOCK, &set, NULL);
  #endif

  /* Recombine the read values in the buffer and return it */
  if(flag < 4)
  {
    for(i = 0; i <= flag; i++)
    {
      returnValue |= ((uInt32) buffer[i]) << (8 * i);
    }
  }
  else if(flag == RSPI_COPY_STR_FLAG)
  {
    /* Copy recieved buffer values into passed in array */
    SIVOICE_STRNCPY(buf, (char*) buffer, size - 1); /* -1 for null character */
    returnValue = 0;
  }
  else if(flag == RSPI_CLOSE_SOCK_FLAG)
  {
    close(clientTCPSocket);
  }
  return returnValue;
}

/*
** Function: RspiFlushWriteBuffer
**
** Description:
** Flushes the buffer
**
** Input Parameters:
** TCP socket, alarms, write buffer, counter that determines buffer size
**
** Return:
** none
**
*/

uInt8 RspiFlushWriteBuffer(uInt32* clientTCPSocket, sigset_t* set,
                      uInt32* timer_handle_, uInt8* writeRegBuffer,
                      uInt16* writeRegCounter)
{
  #ifndef __MINGW32__
    SILABS_UNREFERENCED_PARAMETER(timer_handle_);
    sigprocmask(SIG_BLOCK, set, NULL); /* block signal alarm from triggering */
    ualarm(0, 0); /* disable the alarm */
  #else
    if(*timer_handle_ != 0)
    {
      DeleteTimerQueueTimer(NULL, (HANDLE)*timer_handle_, NULL);
      *timer_handle_ = 0;
    }
  #endif

  if(send(*clientTCPSocket, (char*) writeRegBuffer, *writeRegCounter, 0) < 0)
  {
    RspiPrint("Server disconnected\n");
    return RSPI_CONNECTION_FAILED;
  }

  if(recv(*clientTCPSocket, (char*)writeRegBuffer, RSPI_MAX_BUFFER_SIZE,0) <= 0)
  {
    RspiPrint("Server disconnected\n");
    return RSPI_CONNECTION_FAILED;
  }

  if(writeRegBuffer[RSPI_FUNCTION_VALUE] != 0)
  {
    RspiPrint("Failed to write to a register or RAM\n");
    return RSPI_CONNECTION_FAILED;
  }

  SIVOICE_MEMSET(writeRegBuffer, 0, RSPI_MAX_BUFFER_SIZE * sizeof(uInt8));
  *writeRegCounter = 0;

  #ifndef __MINGW32__
    sigprocmask(SIG_UNBLOCK, set, NULL);
  #endif

  return 0;
}

/*
** Function: RspiInitializeConnection
**
** Description:
** Created TCP and UDP connections to the server
**
** Input Parameters:
** TCP and UDP sockets and address information
**
** Return:
** none
**
*/

void RspiInitializeConnection(uInt32* clientTCPSocket, uInt32* clientUDPSocket,
                          struct sockaddr_in* serverAddr, socklen_t* addr_size,
                          char* rspi_ip_address, uInt16 rspi_port_num)
{
  /* Open socket on Windows */
  #ifdef __MINGW32__
    WSADATA wData;
    if(WSAStartup(MAKEWORD(2,2), &wData) != 0)
    {
      RspiPrint("Could not open Windows connection.\n");
    }
  #endif

  *clientTCPSocket = socket(PF_INET, SOCK_STREAM, 0);
  *clientUDPSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*---- Configure settings of the server address struct ----*/

  serverAddr->sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr->sin_port = htons(rspi_port_num);
  /* Set IP address to localhost */
  serverAddr->sin_addr.s_addr = inet_addr(rspi_ip_address);
  /* Set all bits of the padding field to 0 */
  SIVOICE_MEMSET(serverAddr->sin_zero, '\0', sizeof serverAddr->sin_zero);

  /*---- Connect the socket to the server using the address struct ----*/
  *addr_size = sizeof *serverAddr;

  connect(*clientTCPSocket, (struct sockaddr *) serverAddr, *addr_size);
}

/*
** Function: RspiGetDataFromTXT
**
** Description:
** Gets IP address, port number, username, and password from
** custom/rspi_client_cfg.txt
**
** Input Parameters:
** ip_address, port_num, username, password for
**
** Return:
** none
**
*/

#ifdef RSPI_TXT_CFG
void RspiGetDataFromTXT(char* rspi_ip_address, uInt16* rspi_port_num,
                    char* rspi_username, char* rspi_password)
{
  uInt8 i = 0;
  char firstChar;
  char parameter[RSPI_MAX_STR_SIZE];
  FILE *file = fopen("../custom/rspi_client_cfg.txt", "r");

  if (file != NULL)
  {
    /* Start reading in parameters from file */
    while(fscanf(file,"%s", parameter) > 0)
    {
      firstChar = parameter[0];
      if(firstChar == '#' || isspace(firstChar))
      {
        continue;
      }
      switch(i)
      {
        case 0:
        {
          SIVOICE_STRNCPY(rspi_ip_address, parameter, RSPI_MAX_STR_SIZE);
          break;
        }
        case 1:
        {
          *rspi_port_num = (uInt16) strtoul(parameter, NULL, 0);
          break;
        }
        case 2:
        {
          SIVOICE_STRNCPY(rspi_username, parameter, RSPI_MAX_STR_SIZE);
          break;
        }
        case 3:
        {
          SIVOICE_STRNCPY(rspi_password, parameter, RSPI_MAX_STR_SIZE);
          break;
        }
      }
      i++;
    }

    fclose(file);
  }
  else
  {
    RspiPrint("Failed to open cfg file");
  }
}
#endif

/*
** EEPROM Read
*/

unsigned char  ReadEEPROMByte(unsigned short eAddr)
{
  eAddr = htons(eAddr);
  uInt8 function[3] = {RSPI_READ_EEEPROM_BYTE, (uInt8) eAddr,
                                               (uInt8) (eAddr >> 8)};
  unsigned char returnValue = (unsigned char)RspiServerInterface(function,
                                                    sizeof(function), 0, NULL);
  return returnValue;
}

/*
** EEPROM Write
*/

void WriteEEPROMByte(unsigned short eAddr, unsigned char eData)
{
  eAddr = htons(eAddr);
  uInt8 function[4] = {RSPI_WRITE_EEPROM_BYTE, (uInt8) eAddr,
                       (uInt8) (eAddr >> 8), (uInt8) eData};
  RspiServerInterface(function, sizeof(function), 0, NULL);
}

/*
** Get firmware version
*/

unsigned int GetFirmwareID()
{
  uInt8 function[1] = {RSPI_GET_FIRMWARE_ID};
  unsigned int returnValue = ntohs((unsigned int) RspiServerInterface(function,
                                                  sizeof(function), 1, NULL));
  return returnValue;
}


/*
** Function: get_evb_id
**
** Description:
** Gets the version of the evaluation board attached to the server
**
** Input Parameters:
** Buffer to receive the version and the size of the buffer
**
** Return:
** None
**
*/

void RspiGetEvbId(char* buffer, uInt8 size)
{
  static char boardTypeStr[RSPI_MAX_STR_SIZE];
  uInt8 function[1] = {RSPI_GET_EVB_ID};
  unsigned int data;

  ctrl_ResetWrapper(NULL, 1); /* Place the board in reset */
  RspiServerInterface(function, sizeof(boardTypeStr), RSPI_COPY_STR_FLAG,
                      boardTypeStr);
  ctrl_ResetWrapper(NULL, 0); /* deassert reset */

  RspiPrint("Board connected to the server:\n");

  /* If the first character isn't a S then EEPROM is not programmed or for the case of the Si3050, not installed */
  if(boardTypeStr[0] != 'S')
  {
    RspiPrint("Si3050 or EEPROM not programmed correctly\n");
  }
  else
  {
    RspiPrint(boardTypeStr);
  }

  /* if the eeprom string contains a 0 (Si30...), then it's a 3050, else it's assumed to be ProSLIC */
  if(boardTypeStr[3] != '0')
  {
    data = ctrl_ReadRegisterWrapper(NULL, 0, 0);
    #ifndef RSPI_NO_STD_OUT
    printf(":%c\n", (data &07) + 'A');
    #endif
    snprintf(buffer, size, "%s:%c", boardTypeStr, (data &07) + 'A');
  }
  else
  {
    data = ctrl_ReadRegisterWrapper(NULL, 0, 13);
    #ifndef RSPI_NO_STD_OUT
    printf(":%c\n", ((data >>2 ) &0xF) + 'A');
    #endif
    snprintf(buffer, size, ":%c", ((data >>2 ) &0xF) + 'A');
  }
}

/*
** Function: SPI_Init
**
** Description:
** Initializes the SPI interface by opening the USB VCP
**
** Input Parameters:
** interfacePtr to EVB
**
** Return:
** 1 - Success
** 0 - Failed to open port
**
*/

int SPI_Init(ctrl_S *hSpi)
{
  char BoardID[RSPI_MAX_STR_SIZE];
  uInt8 result;
  SILABS_UNREFERENCED_PARAMETER(hSpi);
  uInt8 function[1] = {RSPI_SPI_INIT};
  result = (uInt8) RspiServerInterface(function, sizeof(function),
                                       RSPI_OPEN_SOCK_FLAG, NULL);
  if(result == RSPI_CONNECTION_FAILED)
  {
    return 0;
  }
  RspiGetEvbId(BoardID, RSPI_MAX_STR_SIZE);
  return result;
}

/*
** Function: SPI_Close
**
** Description:
** Closes VCP
**
** Input Parameters:
** none
**
** Return:
** 1 - Success
** 0 - Failed to close port
**
*/

int SPI_Close(ctrl_S *hSpi)
{
  uInt8 result;
  SILABS_UNREFERENCED_PARAMETER(hSpi);
  uInt8 function[1] = {RSPI_SPI_CLOSE};
  result = (uInt8) RspiServerInterface(function, sizeof(function),
                                       RSPI_CLOSE_SOCK_FLAG, NULL);
  return result;
}

/*
** Function: SPI_GetPortNum
**
** Description:
** Returns VCP port number (for debug purposes)
**
** Input Parameters:
** none
**
** Return:
** port number
**
*/

char *SPI_GetPortNum(void *hSpi)
{
  static char buf[RSPI_MAX_STR_SIZE];
  SILABS_UNREFERENCED_PARAMETER(hSpi);
  uInt8 function[1] = {RSPI_SPI_GET_PORT_NUM};
  RspiServerInterface(function, sizeof(buf), RSPI_COPY_STR_FLAG, buf);
  return buf;
}

/*
** Function: SPI_GetVmbHandle
**
** Description:
** Returns VCP port file handle (for debug purposes)
**
** Input Parameters:
** none
**
** Return:
** handle (void *)
**
*/

unsigned long SPI_GetVmbHandle(ctrl_S *hSpi)
{
  SILABS_UNREFERENCED_PARAMETER(hSpi);
  unsigned long returnValue;
  uInt8 function[1] = {RSPI_SPI_GET_VMB_HANDLE};
  returnValue = htonl((unsigned long) RspiServerInterface(function,
                                                    sizeof(function), 3, NULL));
  return returnValue;
}

/*
** Function: ctrl_ResetWrapper
**
** Description:
** Sets the reset pin of the ProSLIC
*/
int ctrl_ResetWrapper(void *hctrl, int status)
{
  SILABS_UNREFERENCED_PARAMETER(hctrl);
  DRVR_PRINT("%s # Reset: %d\n", DRVR_PREFIX, status);
  uInt8 function[2] = {RSPI_CTRL_RESET_WRAPPER, (uInt8) status};
  RspiServerInterface(function, sizeof(function), 0, NULL);
  return 0;
}

/*
** SPI/GCI register read
**
** Description:
** Reads a single ProSLIC register
**
** Input Parameters:
** channel: ProSLIC channel to read from
** regAddr: Address of register to read
** return data: data to read from register
**
** Return:
** register value
*/

uInt8 ctrl_ReadRegisterWrapper(void *hctrl, uInt8 channel, uInt8 regAddr)
{
  SILABS_UNREFERENCED_PARAMETER(hctrl);
  uInt8 returnValue;
  uInt8 function[3] = {RSPI_CTRL_READ_REGISTER_WRAPPER, channel, regAddr};
  returnValue = (uInt8) RspiServerInterface(function, sizeof(function), 0, NULL);
  return returnValue;
}

/*
** Function: ctrl_WriteRegisterWrapper
**
** Description:
** Writes a single ProSLIC register
**
** Input Parameters:
** channel: ProSLIC channel to write to
** address: Address of register to write
** data: data to write to register
**
** Return:
** none
*/

int ctrl_WriteRegisterWrapper(void *hctrl, uInt8 channel, uInt8 regAddr,
                              uInt8 data)
{
  SILABS_UNREFERENCED_PARAMETER(hctrl);
  DRVR_PRINT("%sREGISTER %d = %02X # CHANNEL = %d\n", DRVR_PREFIX, regAddr, data,
             channel);
  uInt8 function[4] = {RSPI_CTRL_WRITE_REGISTER_WRAPPER, channel, regAddr, data};
  RspiServerInterface(function, sizeof(function), 0, NULL);
  return 0;
}
/*
** Function: SPI_ReadRAMWrapper
**
** Description:
** Reads a single ProSLIC RAM location
**
** Input Parameters:
** channel: ProSLIC channel to read from
** address: Address of RAM location to read
** pData: data to read from RAM location
**
** Return:
** none
*/

ramData ctrl_ReadRAMWrapper(void *hctrl, uInt8 channel, uInt16 ramAddr)
{
  SILABS_UNREFERENCED_PARAMETER(hctrl);
  ramAddr = htons(ramAddr);
  uInt8 function[4] = {RSPI_CTRL_READ_RAM_WRAPPER, channel, (uInt8) ramAddr,
                       (uInt8) (ramAddr >> 8)};
  uInt32 returnValue =  ntohl(RspiServerInterface(function, sizeof(function), 3,
                                                  NULL));
  return returnValue;
}

/*
** Function: SPI_WriteRAMWrapper
**
** Description:
** Writes a single ProSLIC RAM location
**
** Input Parameters:
** channel: ProSLIC channel to write to
** address: Address of RAM location to write
** data: data to write to RAM location
**
** Return:
** none
*/

int ctrl_WriteRAMWrapper(void *hctrl, uInt8 channel, uInt16 ramAddr,
                         ramData data)
{
  SILABS_UNREFERENCED_PARAMETER(hctrl);
  DRVR_PRINT("%sRAM %d = %08X # CHANNEL = %d\n", DRVR_PREFIX, ramAddr, data,
             channel);
  ramAddr = htons(ramAddr);
  data = htonl(data);
  uInt8 function[8] = {RSPI_CTRL_WRITE_RAM_WRAPPER, channel, (uInt8) ramAddr,
                       (uInt8) (ramAddr >> 8), (uInt8) data,
                       (uInt8) (data >> 8), (uInt8) (data >> 16),
                       (uInt8) (data >> 24)};
  RspiServerInterface(function, sizeof(function), 0, NULL);
  return 0;
}

/*
** Function: SPI_setSCLKFreq
**
** Description:
** Select VMB2 SCLK Freq
**
** Input Parameters:
** freq_select
**
** Return:
** none
*/

int SPI_setSCLKFreq(unsigned char sclk_freq_select)
{
  uInt8 function[2] = {RSPI_SPI_SET_SCLK_FREQ, (uInt8) sclk_freq_select};
  uInt8 returnValue =  (uInt8) RspiServerInterface(function, sizeof(function),
                                                   0, NULL);
  return returnValue;
}

int PCM_setPCLKFreq(unsigned char pclk_freq_select)
{
  uInt8 function[2] = {RSPI_SPI_SET_PCLK_FREQ, (uInt8) pclk_freq_select};
  uInt8 returnValue =  (uInt8) RspiServerInterface(function, sizeof(function),
                                                   0, NULL);
  return returnValue;
}

int PCM_setFsyncType(unsigned char fsync_select)
{
  uInt8 function[2] = {RSPI_PCM_SET_FSYNC_TYPE, (uInt8) fsync_select};
  uInt8 returnValue =  (uInt8) RspiServerInterface(function, sizeof(function),
                                                   0, NULL);
  return returnValue;
}

int PCM_setSource(unsigned char pcm_internal_select)
{
  uInt8 function[2] = {RSPI_PCM_SET_SOURCE, (uInt8) pcm_internal_select};
  uInt8 returnValue =  (uInt8) RspiServerInterface(function, sizeof(function),
                                                   0, NULL);
  return returnValue;
}

int PCM_enableXConnect(unsigned char enable)
{
  uInt8 function[2] = {RSPI_PCM_ENABLE_XCONNECT, (uInt8) enable};
  uInt8 returnValue =  (uInt8) RspiServerInterface(function, sizeof(function),
                                                   0, NULL);
  return returnValue;
}

/*
** Wrapper for setPcmSourceExp()
**
*/

unsigned short setPcmSourceExp(unsigned short internal,int freq,int extFsync)
{
  SILABS_UNREFERENCED_PARAMETER(internal);
  SILABS_UNREFERENCED_PARAMETER(freq);
  SILABS_UNREFERENCED_PARAMETER(extFsync);
  return 0;
}

unsigned char PCM_readSource()
{
  uInt8 function[1] = {RSPI_PCM_READ_SOURCE};
  uInt8 returnValue = (uInt8) RspiServerInterface(function, sizeof(function),
                                                  0, NULL);
  return returnValue;
}

void SPI_SelectCS(unsigned char cs)
{
  uInt8 function[2] = {RSPI_SPI_SELECT_CS, (uInt8) cs};
  RspiServerInterface(function, sizeof(function), 0, NULL);
}

void SPI_SelectFormat(unsigned char fmt)
{
  uInt8 function[2] = {RSPI_SPI_SELECT_FORMAT, (uInt8) fmt};
  RspiServerInterface(function, sizeof(function), 0, NULL);
}
