/******************************************************************************
 * Copyright (c) 2017 by Silicon Labs
 *
 * $Id: rspi_server.h 7068 2018-04-12 23:55:12Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Labs, Inc.
 *
 */

#ifndef RSPI_H
#define RSPI_H

#include <stdio.h>
#include "rspi_common.h"

#ifndef RSPI_IS_EMBEDDED
#ifdef __linux__
#include "proslic_vmb2.h"
#include "sivoice_example_os.h"
#else
#include "vmb2_vcp_wrapper.h"
#endif

#include "proslic_eeprom.h"
#endif

#include "rspi_server_cfg.h"
#include "si_voice_ctrl.h"

#define RSPI_SERVER_VER         "1.0"
#define RSPI_FIRMWARE_STR_LEN   6
#define RSPI_FD_SIZE            1024
#define RSPI_NO_USER            '\0'
#define RSPI_VALID_USER         1

/*
** Function: RspiServer
**
** Description:
** Gets function commands from client and sends the value back to the client
*/
uInt8 RspiServer(ctrl_S* interfacePtr, SiVoiceControlInterfaceType* hwIf);

/*
** Function: WriteRegRAM
**
** Description:
** Sends a bulk of write registers and RAMs to VMB2 board
*/
void RspiWriteRegRAM(uInt32* newSocket, uInt8* buffer, ctrl_S* interfacePtr,
                     SiVoiceControlInterfaceType* hwIf);

#endif
