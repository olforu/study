/*
* Copyright 2017-2018 by Silicon Laboratories
*
* $Id: rspi_server_cfg.h 7068 2018-04-12 23:55:12Z nizajerk $
*
*
* Author(s):
* ketian
*
* Distributed by:
* Silicon Labs, Inc
*
* This file contains proprietary information.
* No dissemination allowed without prior written permission from
* Silicon Labs, Inc.
*
*
*/
#ifndef RSPI_SERVER_CFG
#define RSPI_SERVER_CFG

#define RSPI_IP_ADDRESS      "127.0.0.1"

#define RSPI_PORT_NUM        7890
#define RSPI_MAX_NUM_CLIENTS 5
#define RSPI_NUM_OF_USERS    1

/*
** Bitmask to determine available changes to defaults if custom setup
** Make sure custom flag is set (CUSTOM = 1) and no other flags
** Bitmask defaults for VMB2, VMB1, Linux_SPIDEV are in rspi_server.h
** NOTE: Not yet implemented, just a thought
** Bits 0 - 1: indicate connected board
** 00 = CUSTOM, 01 = VMB1 10 = VMB2 11 = LINUX_SPIDEV
** Bit 2: 1 = Has ISI capabilities
** Bit 3: 1 = Can change SCLK frequency
** Bit 4: 1 = Can change PCM source/PCLK frequency
** Bit 5: 1 = Can change Framesync type
** Bit 6: 1 = Can change PCM Cross-connect configuration
** Bit 7: 1 = Can change SPI clock rate
*/
#ifdef CUSTOM
const uInt32 functionsBitmask = 0x0000;
#endif

/*
** How many seconds before server times client out and needs to re login
** set to -1 for never time_out
*/
#define RSPI_CLIENT_TIME_OUT -1

/*
** User verification
** Usernames and passwords can have at most 19 characters and in order
** Remember to update RSPI_NUM_OF_USERS
*/
const char* rspi_usernames[RSPI_NUM_OF_USERS] = {"admin"};
const char* rspi_passwords[RSPI_NUM_OF_USERS] = {"admin"};

#endif

