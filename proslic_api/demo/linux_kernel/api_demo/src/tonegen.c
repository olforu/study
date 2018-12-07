/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: tonegen.c 5150 2015-10-05 18:56:06Z nizajerk $
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
** Implements the tonegen menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"


/*****************************************************************************************************/
void tonegen_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *menu_items[] = 
  {
    "Load Tone Generator Preset",
    "Enable Tone Generator w/o cadence timers",
    "Enable Tone Generator w/ cadence timers",
    "Disable Tone Generator",
    NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Audio Menu", menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0:
        change_preset(devices[state->current_port].fd, state->current_channel,
          PROSLIC_IOCTL_GET_TONE_COUNT, PROSLIC_IOCTL_SET_TONE);
        break;

      case 1:
        /* Fall through */
      case 2:
        /* Fall through */
      case 3:
        {
          set_preset(devices[state->current_port].fd, state->current_channel,
            user_selection-1, PROSLIC_IOCTL_TONE_ON_OFF);
        }
        break;

      default:
        break;
    } /* switch statement */

  } while(user_selection!= QUIT_MENU);
}

