/*
** Copyright (c) 2013-2016 by Silicon Laboratories, Inc.
**
** $Id: converter.c 5733 2016-06-20 23:29:59Z nizajerk $
**
** linefeed.c
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** linefeed resources
**
*/

#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"
#include "proslic.h"

/*****************************************************************************************************/
void converterMenu(demo_state_t *pState)
{
  const char *menu_items[] =
  {
    "Power Up Converter",
    "Power Down Converter",
    "Read DCDC_STATUS",
    NULL
  };
  int user_selection, status;
  ramData data;

  do
  {
    user_selection = get_menu_selection( display_menu("Converter Menu", menu_items),
                                         pState->currentChannel);

    switch(user_selection)
    {
      case 0:
        if( (status = ProSLIC_PowerUpConverter(pState->currentChanPtr) ) != RC_NONE)
        {
          printf("\nDCDC Powerup Failed (status = %d)\n", status);
        }
        break;

      case 1:
        ProSLIC_PowerDownConverter(pState->currentChanPtr);
        break;

      case 2:
        data = ProSLIC_ReadRAM(pState->currentChanPtr, PROSLIC_RAM_DCDC_STATUS);
        printf("DCDC_STATUS = 0x%08X\n", (int)data);
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);

}

