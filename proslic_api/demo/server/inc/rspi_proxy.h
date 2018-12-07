/******************************************************************************
 * Copyright (c) 2017 by Silicon Labs
 *
 * $Id: rspi_proxy.h 7068 2018-04-12 23:55:12Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Labs, Inc.
 *
 */

#ifndef RSPI_H
#define RSPI_H

#include "rspi_common.h"
#include "rspi_proxy_cfg.h"

#define RSPI_FD_SIZE            1024

/*
** Function: RspiProxy
**
** Description:
** Gets commands from client or another proxy and sends to server or another
** proxy and gets the return value back up the chain
*/
uInt8 RspiProxy();

#endif
