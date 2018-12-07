/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: linefeed.c 5150 2015-10-05 18:56:06Z nizajerk $
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
** Implements the linefeed menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"

/* These states MUST match what is in the linefeed status register definitions */
static const char *linefeed_states[] = 
{
  "OPEN",
  "FWD ACTIVE",
  "FWD OHT",
  "TIP OPEN",
  "RINGING",
  "REV ACTIVE",
  "REV OHT",
  "RING OPEN",
  NULL
};

/*****************************************************************************************************/
static void ChangeLinefeedState(int fd, int channel)
{
  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Linefeed State", linefeed_states), channel);

    if(user_selection < 8)
    {
      proslic_chan_if chan_info;
      chan_info.channel = channel;
      chan_info.byte_value = (uint8_t)user_selection;

      if( ioctl(fd, PROSLIC_IOCTL_SET_LINE_STATE, &chan_info) != 0)
      {
        PROSLIC_API_MSG("Failed to change linestate");
        return -1;
      }
      
    }
  } while(user_selection!= QUIT_MENU);
}

/*****************************************************************************************************/
void linefeed_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *linefeed_menu_items[] = 
  {
    "Read Linefeed State",
    "Change Lineeed State",
    "Load DC Feed Preset",
    //"POLREV setting",
    //"WINK setting",
    //"PWRSAVE Mode",
    NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Linefeed Menu", linefeed_menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0:
        {
          uInt8 data;
          data = ReadReg(devices[state->current_port].fd, state->current_channel, PROSLIC_REG_LINEFEED);
          PROSLIC_API_MSG("LF State = %s (0x%02X)", linefeed_states[data & 0x7], data);
        }
        break;

      case 1:
        ChangeLinefeedState(devices[state->current_port].fd, state->current_channel);
        break;

      case 2:
        change_preset(devices[state->current_port].fd, state->current_channel,
          PROSLIC_IOCTL_GET_DCFEED_COUNT, PROSLIC_IOCTL_SET_DCFEED);
        break;

      default:
        break;
    } /* switch statement */

  } while(user_selection!= QUIT_MENU);
}

