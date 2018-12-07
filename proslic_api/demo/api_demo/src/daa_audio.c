/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: daa_audio.c 5659 2016-05-16 16:15:59Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the DAA audio menu & support functions.
**
*/

#include "api_demo.h"
#include "user_intf.h"
#include "vdaa.h"

/*****************************************************************************************************/
static void daa_mute_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Disable All",
    "Disable RX",
    "Disable TX",
    "Enable RX",
    "Enable TX",
    "Enable ALL",
    NULL
  };

  int user_selection;
  const int mute_map[6] = { MUTE_DISABLE_ALL, MUTE_DISABLE_RX, MUTE_DISABLE_TX,
                            MUTE_ENABLE_RX, MUTE_ENABLE_TX, MUTE_ENABLE_ALL
                          };
  user_selection = get_menu_selection( display_menu("Audio Mute mode",
                                       menu_items), state->currentChannel);

  Vdaa_SetAudioMute(state->currentChanPtr, mute_map[user_selection]);
}

/*****************************************************************************************************/
static void daa_loopback_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Disable All",
    "Isolation Digital Enabled",
    "Digital Enabled",
    "PCM Analog Enabled",
    "Isolation Digital Disabled",
    "Digital Disabled",
    "PCM Analog Disabled",
    NULL
  };

  int user_selection, isEnabled;
  const int loopback_map[] = { LPBK_NONE, LPBK_IDL, LPBK_DDL, LPBK_PCML };

  user_selection = get_menu_selection( display_menu("Loopback mode", menu_items),
                                       state->currentChannel);

  /* Did the user select enabled or disabled? If disable, shift the selection to match the map and set
     the isEnabled accordingly...
  */
  if(user_selection < 4)
  {
    isEnabled = LPBK_ENABLED;
  }
  else
  {
    user_selection -= 3;
    isEnabled = LPBK_DISABLED;
  }

  Vdaa_SetLoopbackMode(state->currentChanPtr, loopback_map[user_selection],
                       isEnabled);
}

/*****************************************************************************************************/
void daa_audio_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Change TX Audio Gain Preset",
    "Change RX Audio Gain Preset",
    "Change PCM Preset",
    "Modify PCM timeslots",
    "Enable PCM Bus",
    "Disable PCM Bus",
    "Mute Audio Menu",
    "Loopback Menu",
    "Change Hybrid Preset",
    NULL
  };

  int user_selection, aux_selection,rxcount, txcount;
  do
  {
    user_selection = get_menu_selection( display_menu("DAA Audio", menu_items),
                                         state->currentChannel);

    switch(user_selection)
    {

      case 0:
        aux_selection = demo_get_preset(DEMO_VDAA_AUDIO_GAIN_PRESET);
        Vdaa_TXAudioGainSetup(state->currentChanPtr, aux_selection);
        break;

      case 1:
        aux_selection = demo_get_preset(DEMO_VDAA_AUDIO_GAIN_PRESET);
        Vdaa_RXAudioGainSetup(state->currentChanPtr, aux_selection);
        break;

      case 2:
        aux_selection = demo_get_preset(DEMO_VDAA_PCM_PRESET);
        Vdaa_PCMSetup(state->currentChanPtr, aux_selection);
        break;

      case 3:
        do
        {
          printf("Enter RX Count %s ",PROSLIC_PROMPT);
          rxcount = get_int(0,0x3FF);
        }
        while(rxcount > 0x3FF);

        do
        {
          printf("Enter TX Count %s ",PROSLIC_PROMPT);
          txcount = get_int(0,0x3FF);
        }
        while(txcount > 0x3FF);

        Vdaa_PCMTimeSlotSetup(state->currentChanPtr, rxcount, txcount);
        break;

      case 4:
        Vdaa_PCMStart(state->currentChanPtr);
        printf("%sPCM bus enabled\n", LOGPRINT_PREFIX);
        break;

      case 5:
        Vdaa_PCMStop(state->currentChanPtr);
        printf("%sPCM bus disabled\n", LOGPRINT_PREFIX);
        break;

      case 6:
        daa_mute_menu(state);
        break;

      case 7:
        daa_loopback_menu(state);
        break;

      case 8:
        aux_selection = demo_get_preset(DEMO_VDAA_HYBRID_PRESET);
        Vdaa_HybridSetup(state->currentChanPtr, aux_selection);
        break;

      default:
        break;
    }

  }
  while(user_selection != QUIT_MENU);
}

