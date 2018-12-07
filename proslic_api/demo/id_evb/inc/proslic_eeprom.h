/*
** Copyright (c) 2016 by Silicon Laboratories
**
**
** ProSLIC/VDAA EEPROM decoder
**
** $Id: proslic_eeprom.h 5478 2016-01-18 21:21:37Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This source file decodes the EEPROM found on the Silicon Labs ProSLIC/VDAA EVB boards.
** 
** Dependancies:
**
** A SPI driver.  
*/

#ifndef __PROSLIC_EEPROM_HDR__
#define __PROSLIC_EEPROM_HDR__ 1

#define EEPROM_CHECKSUM           0
#define EEPROM_MAPREV             1
#define EEPROM_DATA_START_L       2
#define EEPROM_DATA_START_H       3
#define EEPROM_DATA_END_L         4
#define EEPROM_DATA_END_H         5
#define EEPROM_ID_TYPE_OS         0
#define EEPROM_ID_SIZE_OS         1
#define EEPROM_ID_START_OS        3
#define EEPROM_ID_BOARD_TYPE      0xF1
#define EEPROM_ID_BOARD_REV       0xF2
#define EEPROM_ID_SSID            0xE1
#define EEPROM_ID_LF_DEVICE       0xD1
#define EEPROM_ID_LF_REV          0xD2
#define EEPROM_ID_MAX_VBAT        0xC1

typedef struct
{
    /* Input from EEPROM */
    uInt8 checksum;
    uInt8 map_rev;
    uInt16 data_start;
    uInt16 data_end;
    uInt8  board_type_size;
    char   board_type_str[32];
    uInt8  board_rev_size;
    char   board_rev_str[8];
    uInt8  ssid_size;
    char   ssid_str[32];
    uInt8  lf_dev_size;
    char   lf_dev_str[32];
    uInt8  lf_dev_rev_size;
    char   lf_dev_rev_str[8];
    uInt8  max_vbat_size;
    uInt8  max_vbat;
    //uInt8  device_rev;  /* Not stored on EEPROM - read directly from device */
} ProSLIC_Eeprom_t;

void getEEPROM_Info(ProSLIC_Eeprom_t *eeprom);

#endif /* __PROSLIC_EEPROM_HDR__ */



