/*
** Copyright (c) 2016-2018 by Silicon Laboratories
**
**
** ProSLIC/VDAA EEPROM decoder
**
** $Id: proslic_eeprom.c 7065 2018-04-12 20:24:50Z nizajerk $
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
** Dependencies:
**
** A SPI driver.
*/


#include "si_voice_datatypes.h"
#include "spi_main.h"

#include "proslic_eeprom.h"

extern unsigned char ReadEEPROMByte(unsigned short addr);

/*****************************************************************************/

void getEEPROM_Info(ProSLIC_Eeprom_t *eeprom)
{
  uInt8 tmp;
  unsigned int i;
  unsigned int addr_offset;
  int done = 0;
  uInt8 id_type;

#ifdef ISI_ENABLED
  SPI_SelectCS(3);
#endif
  /*
   ** Read EEPROM Header
   */
  eeprom->checksum = ReadEEPROMByte(EEPROM_CHECKSUM);
  eeprom->map_rev  = ReadEEPROMByte(EEPROM_MAPREV);
  tmp = ReadEEPROMByte(EEPROM_DATA_START_H);
  eeprom->data_start = (tmp << 8) | ReadEEPROMByte(EEPROM_DATA_START_L);
  tmp = ReadEEPROMByte(EEPROM_DATA_END_H);
  eeprom->data_end = (tmp << 8) | ReadEEPROMByte(EEPROM_DATA_END_L);

  /* Token method - read ID bytes and process accordingly until address >= data_end is required */
  addr_offset = eeprom->data_start;

  while(!done)
  {
    id_type = ReadEEPROMByte(addr_offset);

    switch(id_type)
    {
      case EEPROM_ID_BOARD_TYPE:
        if( (eeprom->board_type_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)) )
        {
          for(i=0; i<eeprom->board_type_size; i++)
          {
            eeprom->board_type_str[i] = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS+i);
          }
          eeprom->board_type_str[i] = '\0';
        }
        addr_offset += eeprom->board_type_size + EEPROM_ID_START_OS;
        break;

      case EEPROM_ID_BOARD_REV:
        if( (eeprom->board_rev_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)) )
        {
          for(i=0; i<eeprom->board_rev_size; i++)
          {
            eeprom->board_rev_str[i] = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS+i);
          }
          eeprom->board_rev_str[i] = '\0';
        }
        addr_offset += eeprom->board_rev_size + EEPROM_ID_START_OS;
        break;

      case EEPROM_ID_SSID:
        if((eeprom->ssid_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)) )
        {
          for(i=0; i<eeprom->ssid_size; i++)
          {
            eeprom->ssid_str[i] = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS+i);
          }
          eeprom->ssid_str[i] = '\0';
        }
        addr_offset += eeprom->ssid_size + EEPROM_ID_START_OS;
        break;

      case EEPROM_ID_LF_DEVICE:
        if((eeprom->lf_dev_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)))
        {
          for(i=0; i<eeprom->lf_dev_size; i++)
          {
            eeprom->lf_dev_str[i] = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS+i);
          }
          eeprom->lf_dev_str[i] = '\0';
        }
        addr_offset += eeprom->lf_dev_size + EEPROM_ID_START_OS;
        break;

      case EEPROM_ID_LF_REV:
        if((eeprom->lf_dev_rev_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)))
        {
          for(i=0; i<eeprom->lf_dev_rev_size; i++)
          {
            eeprom->lf_dev_rev_str[i] = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS+i);
          }
          eeprom->lf_dev_rev_str[i] = '\0';
        }
        addr_offset += eeprom->lf_dev_rev_size + EEPROM_ID_START_OS;
        break;

      case EEPROM_ID_MAX_VBAT:
        if((eeprom->max_vbat_size = ReadEEPROMByte(addr_offset + EEPROM_ID_SIZE_OS)))
        {
          eeprom->max_vbat = ReadEEPROMByte(addr_offset+EEPROM_ID_START_OS);
        }
        addr_offset += eeprom->max_vbat_size + EEPROM_ID_START_OS;
        break;

      default:
        done = 1;
        break;
    }
  }
#ifdef ISI_ENABLED
  SPI_SelectCS(2);
#endif
}
