/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: daa_linestate.c 5659 2016-05-16 16:15:59Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the DAA main menu & support functions.
**
*/

#include "api_demo.h"
#include "user_intf.h"
#include "vdaa.h"

/*****************************************************************************************************/
void daa_ToggleHookState(demo_state_t *state)
{
  uInt8 regData;

  regData = Vdaa_GetHookStatus(state->currentChanPtr);
  switch(regData)
  {
    case VDAA_ONHOOK:
    case VDAA_ONHOOK_MONITOR:
      Vdaa_SetHookStatus(state->currentChanPtr, VDAA_OFFHOOK);
      break;

    case VDAA_OFFHOOK:
      Vdaa_SetHookStatus(state->currentChanPtr, VDAA_ONHOOK);
      break;
  }
}

/*****************************************************************************************************/
static char *ReadHookStatus(SiVoiceChanType_ptr chanPtr)
{

  switch( Vdaa_GetHookStatus(chanPtr) )
  {
    case VDAA_ONHOOK:
      return "ONHOOK";

    case VDAA_ONHOOK_MONITOR:
      return "ONHOOK_MONITOR";

    case VDAA_OFFHOOK:
      return "OFFHOOK";

    default:
      return "UNKNOWN";
  }
}

/*****************************************************************************************************/
void daa_linestate_menu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "Toggle Hook State",
    "Get Hook State",
    "Line Monitor",
    "Change Line in Use (LIU) settings",
    "Display Line in Use (LIU) settings",
    "Check for Line in Use (LIU)",
    NULL
  };

  int user_selection, aux_selection;
  vdaa_LIU_Config liuCfg;
  Vdaa_InitLineInUseCheck(&liuCfg,6,35,14);

  do
  {
    user_selection = get_menu_selection( display_menu("DAA Line State Menu",
                                         menu_items), state->currentChannel);

    switch(user_selection)
    {
      case 0:
        daa_ToggleHookState(state);
        break;

      case 1:
        LOGPRINT("%sHook state = %s\n", LOGPRINT_PREFIX,
                 ReadHookStatus(state->currentChanPtr));
        break;

      case 2:
        {
          int8 vloop;
          int16 iloop;
          int ovl = 0;
          ovl = Vdaa_ReadLinefeedStatus (state->currentChanPtr,&vloop, &iloop);

          LOGPRINT("\n%sVTR\t=\t%d v\n", LOGPRINT_PREFIX, vloop);
          LOGPRINT("%sILOOP\t=\t%d mA\n", LOGPRINT_PREFIX, iloop);

          if(ovl == RC_VDAA_ILOOP_OVLD)
          {
            LOGPRINT("%s### OVL ###\n", LOGPRINT_PREFIX);
          }
        }
        break;

      case 3:
        {
          int minOnV, minOffV, minOffI;
          do
          {
            printf("Enter minimum acceptable onhook voltage (below indicates parallel handset) (V) %s ",
                   PROSLIC_PROMPT);
            minOnV = get_int(0,0xFF);
          }
          while(minOnV> 0xFF);

          do
          {
            printf("Enter minimum acceptable offhook voltage (below indicates parallel handset) (V) %s ",
                   PROSLIC_PROMPT);
            minOffV = get_int(0,0xFF);
          }
          while(minOffV> 0xFF);

          do
          {
            printf("Enter minimum acceptable offhook loop current (below indicates parallel handset) (mA) %s ",
                   PROSLIC_PROMPT);
            minOffI = get_int(0,0xFF);
          }
          while(minOffI> 0xFF);
          Vdaa_InitLineInUseCheck(&liuCfg, minOnV, minOffV, minOffI);
        }
        break;

      case 4:
        printf("%s Minimum acceptable onhook voltage (below indicates parallel handset) (V) : %d\n",
               LOGPRINT_PREFIX, liuCfg.min_onhook_vloop);
        printf("%s Minimum acceptable offhook voltage (below indicates parallel handset) (V) : %d\n",
               LOGPRINT_PREFIX, liuCfg.min_offhook_vloop);
        printf("%s Minimum acceptable offhook current (below indicates parallel handset) (mA) : %d\n",
               LOGPRINT_PREFIX, liuCfg.min_offhook_iloop);
        break;

      case 5:
        aux_selection = Vdaa_CheckForLineInUse(state->currentChanPtr, &liuCfg);
        printf("%s %s, handset %sdetected\n", LOGPRINT_PREFIX,
               (aux_selection == VDAA_OFFHOOK)?"OFFHOOK":"ONHOOK",
               (liuCfg.status == PAR_HANDSET_NOT_DETECTED)?"not ":" ");
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

