/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: ringing.c 5150 2015-10-05 18:56:06Z nizajerk $
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
** Implements the ringing menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"

/*****************************************************************************************************/
void ringing_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *menu_items[] = 
  {
    "Start Ringing",
    "Stop Ringing",
    "Load Ringing Preset",
    NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Ringing Menu", menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0:
      case 1:
        set_preset(devices[state->current_port].fd, state->current_channel, (user_selection==0),
          PROSLIC_IOCTL_SET_RINGER_STATE);
        break;

      case 2:
        change_preset(devices[state->current_port].fd, state->current_channel,
          PROSLIC_IOCTL_GET_RINGER_COUNT, PROSLIC_IOCTL_SET_RINGER);
        break;

      default:
        break;
    } /* switch statement */

  } while(user_selection!= QUIT_MENU);
}

