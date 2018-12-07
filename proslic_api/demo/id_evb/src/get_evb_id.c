/*
** Copyright (c) 2016 by Silicon Laboratories
**
**
** ProSLIC API EEPROM decoder ring...
**
** $Id: get_evb_id.c 5665 2016-05-16 22:21:32Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This source file prints out the EEPROM string for the Silicon Labs EVB.
** Dependencies:
**
** A SPI driver.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

/* Standard ProSLIC API header files */
#include "si_voice_datatypes.h"
#include "proslic.h"
#include "spi_main.h"
#include "proslic_timer.h"
#include "proslic_eeprom.h"
#include "user_intf.h"

/*****************************************************************************/

/*****************************************************************************/
int main(void )
{
  ctrl_S                spiGciObj; /* Link to host spi obj (os specific) */
  controlInterfaceType  hwIf;      /* No dynamic memory allocation */
  systemTimer_S         timerObj;  /* Link to host timer obj (os specific)*/
  ProSLIC_Eeprom_t      eeprom_data;

  SETUP_STDOUT_FLUSH;

  /* Initialize platform */
  if(SPI_Init(&spiGciObj) == FALSE)
  {
    printf("Cannot connect to %s\n", VMB_INFO);
    return -1;
  }
  /*
   ** Establish linkage between host objects/functions
   ** and ProSLIC API (required)
   */
  initControlInterfaces(&hwIf, &spiGciObj, &timerObj);
  (hwIf.Reset_fptr)(hwIf.hCtrl, 1); /* Place the board in reset */
  getEEPROM_Info(&eeprom_data);
  (hwIf.Reset_fptr)(hwIf.hCtrl, 0); /* deassert reset */

  /* Was the EEPROM not programmed or for the case of the Si3050, not installed? */
  if( (eeprom_data.checksum == 0xFF)  || (eeprom_data.checksum == 0) )
  {
    printf("0xFF:UNKNOWN:UNKNOWN:UNKNOWN:A\n");
  }
  else
  {
    unsigned int data;

    printf("0x%02X:%s:%s:%s", 
      eeprom_data.checksum, eeprom_data.board_type_str, eeprom_data.board_rev_str, 
      eeprom_data.lf_dev_str);

    /* if the eeprom string contains a 0 (Si30...), then it's a 3050, else it's assumed to be ProSLIC */
    if(eeprom_data.board_type_str[3] != '0')
    {
      data = (hwIf.ReadRegister_fptr)(hwIf.hCtrl, 0, 0);
      printf(":%c\n", (data &07) + 'A');
    }
    else
    {
      data = (hwIf.ReadRegister_fptr)(hwIf.hCtrl, 0, 13);
      printf(":%c\n", ((data >>2 ) &0xF) + 'A');
    }
  }
  FLUSH_STDOUT;
}

