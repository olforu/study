/*
** Copyright 2017-2018, Silicon Labs
** rspi_server.c
**
** $Id: rspi_server.c 7123 2018-04-20 22:51:22Z nizajerk $
** Rspi server implementation file
**
** Author(s):
** ketian
**
** Distributed by:
** Silicon Labs, Inc
**
** File Description:
** This is the implementation file for the SPI/PCM driver used
** in the ProSLIC demonstration code. It calls the library
** that initializes and talks to the voice motherboard.
**
**
*/

#include "rspi_server.h"

#ifdef LINUX_SPIDEV
extern void set_spi_speed(uint32_t speed);
#endif

/*
** Function: WriteRegRAM
**
** Description:
** Takes in an array of bulk of write registers and write RAMs and
** executes them in the order they were called from the client
**
** Input Parameters:
** Pointer to the open TCP socket, buffer values from the client that contains
** the reg and RAM writes, reference to interfacePtr for linux
**
** Return:
** Return 0 for success (No error checking for writes yet)
*/

void RspiWriteRegRAM(uInt32* clientTCPSocket, uInt8* buffer, ctrl_S* interfacePtr,
                     SiVoiceControlInterfaceType* hwIf)
{
  uInt16 i = 0;
  uInt8 writeCheck;
  /** While there is still data to be written continue through the buffer **/
  while(buffer[i] == RSPI_CTRL_WRITE_REGISTER_WRAPPER ||
        buffer[i] == RSPI_CTRL_WRITE_RAM_WRAPPER)
  {
    if(buffer[i] == RSPI_CTRL_WRITE_REGISTER_WRAPPER)
    {
      /*writeCheck = ctrl_WriteRegisterWrapper(interfacePtr, buffer[i + 1],
                                             buffer[i + 2], buffer[i + 3]);*/
      writeCheck = (hwIf->WriteRegister_fptr)(interfacePtr, buffer[i + 1],
                                              buffer[i + 2], buffer[i + 3]);
      /* Increment through the buffer to the next set of writes */
      i += 4;
    }
    else
    {
      uInt16 ramAddr = ((uInt16) buffer[i + 3]) << 8 | buffer[i + 2];
      uInt32 data = 0;
      uInt8 j = 0;

      /* Recombine the 32 bit integer value to be written to RAM */
      for (j = 0; j < RSPI_BYTE_SHIFT; j++)
      {
        data |= ((uInt32) buffer[i + j + 4]) << (8 * j);
      }
      data = ntohl(data);
      ramAddr = ntohs(ramAddr);

      /*writeCheck = ctrl_WriteRAMWrapper(interfacePtr, buffer[i + 1], ramAddr,
                                        data);*/
      writeCheck = (hwIf->WriteRAM_fptr)(interfacePtr, buffer[i + 1], ramAddr,
                                         data);
      i += 8;
    }
    if(writeCheck != 0)
    {
      /* There is an error in a register or RAM write */
      buffer[RSPI_FUNCTION_VALUE] = writeCheck;
      send(*clientTCPSocket, (char*) buffer, 1, 0);
    }
  }
  buffer[RSPI_FUNCTION_VALUE] = 0;
  send(*clientTCPSocket, (char*) buffer, 1, 0);
}

/*
** Function: RspiServer
**
** Description:
** Takes in a command from the client and processes the command by communicating
** directly with the VMB2 board and sends the return value back to the client
**
** Input Parameters:
** Control structure and control interface
**
** Return:
** Returns if a disconnect has occurred
*/

uInt8 RspiServer(ctrl_S* interfacePtr, SiVoiceControlInterfaceType* hwIf)
{
  /*
  ** Bitmask for default board setups
  ** Bits 0 - 1: indicate connected board
  ** 00 = CUSTOM, 01 = VMB1 10 = VMB2 11 = LINUX_SPIDEV
  ** Bit 2: 1 = Has ISI capabilities
  ** Bit 3: 1 = Can change SCLK frequency
  ** Bit 4: 1 = Can change PCM source/PCLK frequency
  ** Bit 5: 1 = Can change Framesync type
  ** Bit 6: 1 = Can change PCM Cross-connect configuration
  ** Bit 7: 1 = Can change SPI clock rate
  */
  #ifdef VMB2
  const uInt32 functionsBitmask = 0x00FE;
  #elif defined(VMB1)
  const uInt32 functionsBitmask = 0x0011;
  #elif defined(LINUX_SPIDEV)
  const uInt32 functionsBitmask = 0x0083;
  #elif defined(RSPI)
  const uInt32 functionsBitmask = 0x0000;
  #else
  const uInt32 functionsBitmask = 0x00FE;
  #endif

  static char user[RSPI_MAX_STR_SIZE];
  user[0] = RSPI_NO_USER; /* indicates there is no current user */
  uInt32 serverTCPSocket, serverUDPSocket, clientTCPSocket, tempSocket;
  uInt32 sockets;
  fd_set active_fd_set, read_fd_set;
  uInt8 buffer[RSPI_MAX_BUFFER_SIZE];
  uInt8 i = 0;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /* Open socket on Windows */
#ifdef __MINGW32__
    WSADATA wData;

    /* MAKEWORD determines which version of winsock to use (2.2 in this case) */
    if(WSAStartup(MAKEWORD(2,2), &wData) != 0)
    {
      RspiPrint("Could not open Windows connection.\n");
      return RSPI_CONNECTION_FAILED;
    }
#endif

  /* Initialize sockets */
  serverTCPSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverUDPSocket = socket(PF_INET, SOCK_DGRAM, 0);
  /* Configure settings of the server address struct */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(RSPI_PORT_NUM);
  serverAddr.sin_addr.s_addr = inet_addr(RSPI_IP_ADDRESS);
  /* Set all bits of the padding field to 0 */
  SIVOICE_MEMSET(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  if(bind(serverTCPSocket, (struct sockaddr *) &serverAddr,
          sizeof(serverAddr)) != 0)
  {
    RspiPrint("Could not bind server TCP socket\n");
    return RSPI_CONNECTION_FAILED;
  }
  if(bind(serverUDPSocket, (struct sockaddr *) &serverAddr,
          sizeof(serverAddr)) != 0)
  {
    RspiPrint("Could not bind server UDP socket\n");
    return RSPI_CONNECTION_FAILED;
  }

  /* Set maximum number of TCP connections to establish */
  if(listen(serverTCPSocket, RSPI_MAX_NUM_CLIENTS) == 0)
  {
    RspiPrint("Listening for client\n");
  }
  else
  {
    RspiPrint("Exceeded number of queued TCP sockets\n");
  }

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  while(1)
  {
    FD_ZERO(&active_fd_set);
    FD_SET(serverTCPSocket, &active_fd_set);
    FD_SET(serverUDPSocket, &active_fd_set);

    while(1)
    {
      struct timeval tv;
      #ifdef __linux__
      uInt8 invalidData = 0;
      #else
      static uInt8 invalidData = 0;
      #endif
      uInt8 selectVal = 1;
      tv.tv_sec = RSPI_CLIENT_TIME_OUT;
      tv.tv_usec = 0;

      /* Clear the buffer on initialization */
      SIVOICE_MEMSET(buffer, 0, sizeof(buffer));

      /* Block until input arrives on one or more active sockets. */
      read_fd_set = active_fd_set;

      if(RSPI_CLIENT_TIME_OUT == -1)
      {
        selectVal = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        if(selectVal < 0)
        {
          RspiPrint("Error on select function\n");
        }
      }
      else
      {
        selectVal = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &tv);
        if(selectVal < 0)
        {
          RspiPrint("Error on select function\n");
        }
      }

      /* Service all the sockets with input pending by going through all of the sockets */
      for(sockets = 0; sockets < RSPI_FD_SIZE; sockets++)
      {
        if (FD_ISSET(sockets, &read_fd_set))
        {
          if (sockets == serverTCPSocket)
          {
            /* Connection request on original socket. */
            invalidData = 1;
            addr_size = sizeof serverStorage;
            tempSocket = accept(serverTCPSocket,
                               (struct sockaddr *) &serverStorage, &addr_size);
            if (tempSocket < 0)
            {
              RspiPrint("Could not establish TCP connection\n");
            }

            #ifndef RSPI_NO_STD_OUT
            printf("Server: connect from host %s, port %hd.\n",
                   inet_ntoa(serverAddr.sin_addr),
                   ntohs(serverAddr.sin_port));
            #endif

            recv(tempSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
            /* Checks if there is a current user */
            if(user[0] != RSPI_NO_USER)
            {
              /* There is a current user */
              snprintf((char*) buffer, RSPI_MAX_BUFFER_SIZE,
                        "\nCurrent User: %s", user);
              RspiPrint(strcat((char*) &buffer[1], "\n"));
              send(tempSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
              break;
            }
            else
            {
              /* New current user, verify username, password, and version */
              char clientVersion[RSPI_MAX_STR_SIZE];
              char clientUsername[RSPI_MAX_STR_SIZE];
              char clientPassword[RSPI_MAX_STR_SIZE];
              char* token = strtok((char*) buffer, " ");

              /* Compares information about client to server */
              for(i = 0; i < 3; i++)
              {
                if(i == 0)
                {
                  SIVOICE_STRNCPY(clientVersion, token, RSPI_MAX_STR_SIZE);
                }
                else if(i == 1)
                {
                  SIVOICE_STRNCPY(clientUsername, token, RSPI_MAX_STR_SIZE);
                }
                else
                {
                  SIVOICE_STRNCPY(clientPassword, token, RSPI_MAX_STR_SIZE);
                }

                /* Get the next token: */
                token = strtok(NULL, " ");
              }

              /* Go through and compare all users registered on the server */
              for(i = 0; i < RSPI_NUM_OF_USERS; i++)
              {
                if(strcmp(clientUsername, rspi_usernames[i]) == 0)
                {
                  if(strcmp(clientPassword, rspi_passwords[i]) == 0)
                  {
                    SIVOICE_STRNCPY((char*) user, clientUsername, RSPI_MAX_STR_SIZE);
                    buffer[RSPI_FUNCTION_VALUE] = RSPI_VALID_USER;
                    break;
                  }
                }
                if(i == RSPI_NUM_OF_USERS - 1)
                {
                  strncpy((char*) buffer, "\nInvalid username or password",
                          RSPI_MAX_BUFFER_SIZE);
                }
              }

              if(strcmp(clientVersion, RSPI_SERVER_VER) != 0)
              {
                snprintf((char*) buffer, RSPI_MAX_BUFFER_SIZE,
                "\nClient and server version are not compatible\nServer Version:%s"
                 ,RSPI_SERVER_VER);
                RspiPrint("Client and server versions are not compatible\n");
              }
            }

            if(buffer[RSPI_FUNCTION_VALUE] == RSPI_VALID_USER)
            {
              /* Make new TCP connection the main TCP connection */
              clientTCPSocket = tempSocket;
              FD_SET(clientTCPSocket, &active_fd_set);
            }
            send(tempSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
            break;
          }
          else if(sockets == serverUDPSocket)
          {
            /* Register or RAM read from client over UDP socket */
            recvfrom(serverUDPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0,
                    (struct sockaddr *)&serverStorage, &addr_size);
            if(user[0] == RSPI_NO_USER)
            {
              /* If client has timed out */
              buffer[RSPI_PACKET_CORRUPTED] = RSPI_TIMED_OUT;
              sendto(serverUDPSocket, (char*) buffer, RSPI_READ_PCKT_SIZE, 0,
                    (struct sockaddr *)&serverStorage, addr_size);
              invalidData = 1;
              break;
            }

            /* Verify checksum */
            if(buffer[RSPI_CHECKSUM_LOCATION] != RspiChecksum((char*) buffer))
            {
              /* If the checksum fails then have client resend read request */
              buffer[RSPI_PACKET_CORRUPTED] = RSPI_IS_CORRUPTED;
              sendto(serverUDPSocket, (char*) buffer, RSPI_READ_PCKT_SIZE, 0,
                    (struct sockaddr *)&serverStorage,addr_size);
              invalidData = 1;
            }
            break;
          }
          else
          {
            /* Data coming from main TCP connection */
            recv(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
            break;
          }
        }
        else if(selectVal <= 0)
        {
          /* Client timed out */
          buffer[RSPI_FUNCTION_VALUE] = 0; /* trigger timeout */
          break;
        }
      }

      /*****************************/
      /* determine which function to call */
      /*printf("Server Function Receive: %d %d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);*/
      if(invalidData == 1)
      {
        invalidData = 0;
        continue;
      }
      uInt8 callFunction = buffer[RSPI_FUNCTION];
      switch (callFunction)
      {
        case RSPI_CLIENT_DISCONNECT:
        {
          if(user[0] != RSPI_NO_USER)
          {
            RspiPrint("Client disconnected\n");
            user[0] = RSPI_NO_USER; /* indicates there is no current user */
          }
          close(clientTCPSocket);
          #if !(defined(VMB1) || defined(RSPI_IS_EMBEDDED))
          SPI_Close(interfacePtr);
          #endif

          /* Clear out fd set to wait for new connections*/
          FD_ZERO(&active_fd_set);
          FD_SET(serverTCPSocket, &active_fd_set);
          FD_SET(serverUDPSocket, &active_fd_set);
          break;
        }
        case RSPI_READ_EEEPROM_BYTE:
        {
#if defined(VMB1) || defined(VMB2)
          /*Combine the split eAddr parameter back together*/
          unsigned short eAddr = ((unsigned short) buffer[2]) << 8 | buffer[1];
          eAddr = htons(eAddr);
          buffer[RSPI_FUNCTION_VALUE] = ReadEEPROMByte(eAddr);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_WRITE_EEPROM_BYTE:
        {
          #if !(defined(VMB1) || defined(RSPI_IS_EMBEDDED) || defined(__linux__))
            unsigned short eAddr=((unsigned short) buffer[2]) << 8 | buffer[1];
            eAddr = htons(eAddr);
            WriteEEPROMByte(eAddr, buffer[3]);
          #endif
          break;
        }
        case RSPI_GET_FIRMWARE_ID:
        {
#if defined(VMB2)
          unsigned int returnValue = GetFirmwareID();
          returnValue = htons(returnValue);
          buffer[RSPI_FUNCTION_VALUE] = (uInt8) returnValue;
          buffer[RSPI_FUNCTION_VALUE + 1] = (uInt8) (returnValue >> 8);
          send(clientTCPSocket, (char*) buffer, 2, 0);
#endif
          break;
        }
        case RSPI_SPI_INIT:
        {
          buffer[RSPI_FUNCTION_VALUE] = SPI_Init(interfacePtr);
          send(clientTCPSocket, (char*) buffer, 1, 0);
          break;
        }
        case RSPI_SPI_CLOSE:
        {
          #if !(defined(VMB1) || defined(RSPI_IS_EMBEDDED))
          #ifndef RSPI_IS_EMBEDDED
          buffer[RSPI_FUNCTION_VALUE] = SPI_Close(interfacePtr);
          RspiPrint("Client disconnected\n");
          user[0] = RSPI_NO_USER;
          send(clientTCPSocket, (char*) buffer, 1, 0);
          #endif
          #endif
          break;
        }
        case RSPI_SPI_GET_PORT_NUM:
        {
#if defined(VMB2)
          SIVOICE_STRNCPY((char*) buffer, SPI_GetPortNum(interfacePtr),
                          RSPI_MAX_STR_SIZE);
          send(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
#endif
          break;
        }
        case RSPI_SPI_GET_VMB_HANDLE:
        {
          #if !(defined(VMB1) || defined(RSPI_IS_EMBEDDED) || defined(__linux__))
            unsigned long handleValue = SPI_GetVmbHandle(interfacePtr);
            handleValue = htonl(handleValue);

            for (i = 0; i < RSPI_BYTE_SHIFT; i++)
            {
          /*store handleValue with most significant values in the highest index*/
              buffer[i] = (uInt8) (handleValue >> (8 * i));
            }
            send(clientTCPSocket, (char*) buffer, 4, 0);
          #endif
          break;
        }
        case RSPI_CTRL_RESET_WRAPPER:
        {
          /*buffer[RSPI_FUNCTION_VALUE] = ctrl_ResetWrapper(interfacePtr,
                                                          buffer[1]);*/
          buffer[RSPI_FUNCTION_VALUE] = (hwIf->Reset_fptr)(interfacePtr,
                                                           buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
          break;
        }
        case RSPI_CTRL_READ_REGISTER_WRAPPER:
        {
          /*buffer[RSPI_FUNCTION_VALUE] = ctrl_ReadRegisterWrapper(interfacePtr,
                                                                 buffer[1],
                                                                 buffer[2]);*/
          buffer[RSPI_FUNCTION_VALUE] = (hwIf->ReadRegister_fptr)(interfacePtr,
                                                                  buffer[1],
                                                                  buffer[2]);
          /* Create a checksum */
          buffer[RSPI_CHECKSUM_LOCATION] = RspiChecksum((char*) buffer);
          sendto(serverUDPSocket, (char*) buffer, RSPI_READ_PCKT_SIZE, 0,
                (struct sockaddr *)&serverStorage,addr_size);
          break;
        }
        case RSPI_CTRL_WRITE_REGISTER_WRAPPER:
        {
          RspiWriteRegRAM(&clientTCPSocket, buffer, interfacePtr, hwIf);
          break;
        }
        case RSPI_CTRL_READ_RAM_WRAPPER:
        {
          uInt8 sum = 0;
          uInt16 ramAddr = ((uInt16) buffer[3]) << 8 | buffer[2];
          ramAddr = ntohs(ramAddr);
          /*ramData RAMValue = ctrl_ReadRAMWrapper(interfacePtr, buffer[1],
                                                 ramAddr);*/
          ramData RAMValue = (hwIf->ReadRAM_fptr)(interfacePtr, buffer[1],
                                                  ramAddr);
          RAMValue = htonl(RAMValue);
          for (i = 0; i < RSPI_BYTE_SHIFT; i++)
          {
            /* store RAMValue with most significant values in the highest index */
            buffer[i] = (uInt8) (RAMValue >> (8 * i));
            sum += buffer[i];
          }
          buffer[RSPI_CHECKSUM_LOCATION] = sum;
          sendto(serverUDPSocket, (char*) buffer, RSPI_READ_PCKT_SIZE, 0,
                (struct sockaddr *)&serverStorage,addr_size);
          break;
        }
        case RSPI_CTRL_WRITE_RAM_WRAPPER:
        {
          RspiWriteRegRAM(&clientTCPSocket, buffer, interfacePtr, hwIf);
          break;
        }
        case RSPI_SPI_SET_SCLK_FREQ:
        {
#if defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = SPI_setSCLKFreq(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#elif defined(LINUX_SPIDEV)
          buffer[RSPI_FUNCTION_VALUE] = 1; 
          set_spi_speed(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_SPI_SET_PCLK_FREQ:
        {
#if defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = PCM_setPCLKFreq(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_PCM_SET_FSYNC_TYPE:
        {
#if defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = PCM_setFsyncType(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_PCM_SET_SOURCE:
        {
#if defined(VMB2) || defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = PCM_setFsyncType(buffer[1]);
          buffer[RSPI_FUNCTION_VALUE] = PCM_setSource(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_PCM_ENABLE_XCONNECT:
        {
#if defined(VMB2) || defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = PCM_enableXConnect(buffer[1]);
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_PCM_READ_SOURCE:
        {
#if defined(VMB2)
          buffer[RSPI_FUNCTION_VALUE] = PCM_readSource();
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_SPI_SELECT_CS:
        {
#if defined(VMB2)
          SPI_SelectCS(buffer[1]);
          buffer[RSPI_FUNCTION_VALUE] = 0;
          send(clientTCPSocket, (char*) buffer, 1, 0);
#endif
          break;
        }
        case RSPI_SET_PCM_SOURCE_EXP:
        {
          buffer[RSPI_FUNCTION_VALUE] = 0;
          send(clientTCPSocket, (char*) buffer, 1, 0);
          break;
        }
        case RSPI_SPI_SELECT_FORMAT:
        {
#if defined(VMB2)
          SPI_SelectFormat(buffer[1]);
          buffer[RSPI_FUNCTION_VALUE] = 0;
          send(clientTCPSocket, (char*) buffer, 1, 0);
          #endif
          break;
        }
        case RSPI_GET_EVB_ID:
        {
#if defined(VMB1) || defined(VMB2)
          ProSLIC_Eeprom_t eeprom_data;
          /*ctrl_ResetWrapper(interfacePtr, 1); /* Place the board in reset */
          (hwIf->Reset_fptr)(interfacePtr, 1);
          getEEPROM_Info(&eeprom_data);
          /*ctrl_ResetWrapper(interfacePtr, 0); /* deassert reset */
          (hwIf->Reset_fptr)(interfacePtr, 0);
          SIVOICE_STRNCPY((char*) buffer, eeprom_data.board_type_str,
                          RSPI_MAX_STR_SIZE);
          send(clientTCPSocket, (char*) buffer, RSPI_MAX_BUFFER_SIZE, 0);
#endif
          break;
        }
        case RSPI_GET_BIT_MASK:
        {
          /* Split the bitmask into 8 bit pieces and send to client */
          for(i = 0; i < RSPI_BYTE_SHIFT; i++)
          {
            buffer[i] = (uInt8) (functionsBitmask >> (8 * i));
          }
          send(clientTCPSocket, (char*) buffer, sizeof(functionsBitmask), 0);
          break;
        }
        default:
        {
          buffer[RSPI_FUNCTION_VALUE] = 0;
          send(clientTCPSocket, (char*) buffer, 1, 0);
          break;
        }
      }

      /* Clear buffer at the end of every function call */
      SIVOICE_MEMSET(buffer, 0, sizeof(buffer));
    }
  }
  return 0;
}

#ifndef RSPI_IS_EMBEDDED
int main(int argc, char const *argv[])
{
  ctrl_S interfacePtr; /* Link to host spi obj (os specific) */
  SiVoiceControlInterfaceType  hwIf;      /* No dynamic memory allocation */
  SiVoice_setControlInterfaceReset (&hwIf, ctrl_ResetWrapper);
  SiVoice_setControlInterfaceWriteRegister (&hwIf, ctrl_WriteRegisterWrapper);
  SiVoice_setControlInterfaceReadRegister (&hwIf, ctrl_ReadRegisterWrapper);
  SiVoice_setControlInterfaceWriteRAM (&hwIf, ctrl_WriteRAMWrapper);
  SiVoice_setControlInterfaceReadRAM (&hwIf, ctrl_ReadRAMWrapper);
  return RspiServer(&interfacePtr, &hwIf);
}
#endif
