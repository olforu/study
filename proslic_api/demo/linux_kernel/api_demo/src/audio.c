/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: audio.c 5150 2015-10-05 18:56:06Z nizajerk $
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
** Implements the audio menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"

/*****************************************************************************************************/
static int set_rxtx_setting(int fd, int channel, int rx, int tx)
{
  proslic_chan_if chan_info;

  chan_info.channel = channel;
  chan_info.int_values[0] = (uint16_t)(rx);
  chan_info.int_values[1] = (uint16_t)(tx);

  if( ioctl(fd, PROSLIC_IOCTL_SET_RXTX_TS, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to set timeslots");
    return -1;
  }
  
  return 0;
}

/*****************************************************************************************************/
void audio_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *menu_items[] = 
  {
    "Load Zsynth Preset",
    "Load PCM bus Preset",
    "Enable PCM Bus",
    "Disable PCM Bus",
    "Set PCM RX/TX timeslots",
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
          PROSLIC_IOCTL_GET_ZSYNTH_COUNT, PROSLIC_IOCTL_SET_ZSYNTH);
        break;

      case 1:
        change_preset(devices[state->current_port].fd, state->current_channel,
          PROSLIC_IOCTL_GET_PCM_COUNT, PROSLIC_IOCTL_SET_PCM);
      break;

      case 2:
        /* Fall through */
      case 3:
        set_preset(devices[state->current_port].fd, state->current_channel,
          (user_selection == 2), PROSLIC_IOCTL_SET_PCM_ON_OFF);
        break;

      case 4:
        {
          int rx,tx;
          printf("Please enter RX timeslot (dec) %s", PROSLIC_PROMPT);
          rx = get_int(0, 0XFFFF);

          printf("Please enter TX timeslot (dec) %s", PROSLIC_PROMPT);
          tx = get_int(0, 0XFFFF);

          if( (rx > 0xFFFF) || (tx > 0xFFFF) )
          {
            PROSLIC_API_MSG("Invalid value, operation aborted.");
          }
          else
          {
            set_rxtx_setting(devices[state->current_port].fd, state->current_channel, rx, tx);
          }
        }
        break;  

      default:
        break;
    } /* switch statement */

  } while(user_selection!= QUIT_MENU);
}

