/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: converter.c 5150 2015-10-05 18:56:06Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** 
** Implements the converter menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"

/*****************************************************************************************************/
void converter_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *converter_menu_items[] = 
  {
    "Power Down Converter",
    "Power Up Converter",
    "Read DCDC_STATUS",
    NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Converter Menu", converter_menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0:
        /* Fall through */
      case 1:
        set_preset(devices[state->current_port].fd, state->current_channel, user_selection, 
          PROSLIC_IOCTL_SET_CONVERTER_STATE);
        break;

      case 2:
        {
          ramData dcdcStatus;
          dcdcStatus = ReadRAM( devices[state->current_port].fd, state->current_channel, PROSLIC_RAM_DCDC_STATUS);
          PROSLIC_API_MSG("DCDC_STATUS = 0x%08X", dcdcStatus);
        }
        break;

      default: 
        break;
    } /* switch statement */

  } while(user_selection!= QUIT_MENU);
  
}

