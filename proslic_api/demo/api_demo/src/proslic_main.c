/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: proslic_main.c 5659 2016-05-16 16:15:59Z nizajerk $
**
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the ProSLIC main menu
**
*/

#include "api_demo.h"
#include "user_intf.h"

#ifdef MLT_ENABLED
#include "proslic_mlt_demo.h"
#endif

/*****************************************************************************************************/
void proslic_main_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Select Channel",
    "Debug Menu",
    "Test & Monitor Menu",
    "Linefeed Menu",
    "DC/DC Converter Menu",
    "Ringing Menu",
    "Audio Menu",
    "Tone Generator Menu",
    "MWI/Pulse Metering Menu",
    "Interrupt Menu",
#ifdef MLT_ENABLED
    "MLT Menu",
#endif
    NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("ProSLIC Menu", menu_items),
                                         state->currentChannel);

    switch(user_selection)
    {
      case 0:
        change_channel(state);
#ifdef VDAA_SUPPORT
        if( state->currentChanPtr->channelType == DAA)
        {
          daa_main_menu(state);
        }
#endif
        break;

      case 1:
        debugMenu(state);
        break;

      case 2:
        testMonitorMenu(state);
        break;

      case 3:
        linefeedMenu(state);
        break;

      case 4:
        converterMenu(state);
        break;

      case 5:
        ringingMenu(state);
        break;

      case 6:
        audioMenu(state);
        break;

      case 7:
        toneGenMenu(state);
        break;

      case 8:
        pmMwiMenu(state);
        break;

      case 9:
        interruptMenu(state);
        break;

      case 10:
#ifdef MLT_ENABLED
        mltMenu(state);
#endif
      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);


}

