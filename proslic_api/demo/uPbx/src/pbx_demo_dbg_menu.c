/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo_dbg_menu.c 6256 2017-01-31 01:24:33Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file contains the debug menu enabled via hookflash.
**
**
*/

#include "proslic.h"

#include "pbx_demo.h"

#ifdef CID_ONLY_DEMO
#include "si_cid.h"
#include "si_cid_example.h"
#include "si_cid_en_na.h"
#include "si_cid_sys_srvc.h"
#include "cid_demo_util.h"
#else
#include <ctype.h>
#endif

#include "user_intf.h"
#include "api_demo.h"

/* external function prototypes */
void debugMenu(demo_state_t *state);

#ifndef CID_ONLY_DEMO
/* This code came from the CID Framework... some code changes made here...*/

/****************************************************************************/
/* Check if the given phone number only contains digits and is within specified length
   returns TRUE if OK
*/

BOOLEAN cid_demo_is_valid_phone_num(uInt8 *new_number, unsigned int length)
{
  unsigned int i;
  BOOLEAN is_valid = TRUE;

  if( (strlen( (char *)new_number) > length) || (strlen( (char *)new_number) < 2) )
  {
    printf("error: string length\n");
    is_valid = FALSE;
  }
  else
  {
    /* Verify character validity - 0-9 */
    for(i = 0; i < strlen( (char *)new_number); i++)
    {
      if(isdigit(new_number[i]) == 0)
      {
        printf("%c is not legal",new_number[i]);
        is_valid = FALSE;
        break;
      }
    }
  }

  return(is_valid);
}

/****************************************************************************/
/* Return back a valid phone number or private/unavailable encoding.  If failed, returns FALSE.
 * NOTE: took out CID specific info...
*/
BOOLEAN cid_demo_get_valid_phone_number(char *prompt, uInt8 *destination, unsigned int length)
{
  char new_number[255];
  BOOLEAN is_valid;

  do
  {
    printf("enter new %s (up to %u characters) (no dashes allowed) --> ",prompt,length);
    FLUSH_STDOUT;
    scanf("%s",new_number);

    is_valid = cid_demo_is_valid_phone_num( (uInt8 *)new_number,length);

  }while(is_valid == FALSE);

  strcpy( (char *)destination,new_number);
  return TRUE;
}
#endif

/****************************************************************************/
/*
 * Change channel being operated on...
 */
static void change_channel(demo_state_t *pState)
{
  int i,m;
  demo_port_t *cPort;

  if(pState->totalChannelCount == 1)
  {
    printf("Only 1 channel configured, aborting request.\n");
    return;
  }

  do
  {
     printf("\nPlease Enter Channel:  (0->%d) %s", (pState->totalChannelCount-1),
            PROSLIC_PROMPT);
     FLUSH_STDOUT;
     m = get_int(0, (pState->totalChannelCount-1));
   }
   while( m >= pState->totalChannelCount);

   pState->currentChannel = m;

   for(i = 0; i < DEMO_PORT_COUNT; i++)
   {
     cPort = &(pState->ports[i]);
     if(m < ((cPort->numberOfChan) + (cPort->channelBaseIndex) ) )
     {
       pState->currentPort = cPort;
       pState->currentChanPtr = cPort->channelPtrs[(m - (cPort->channelBaseIndex) ) ];
       return;
     }
   }
}

/****************************************************************************/
/*
 * Change basic CID settings...
 */
static void cid_demo_change_settings(chanState_t *port, int channel_number)
{
  const char *menu_items[] =
  {
      "Change phone number",
#ifdef CID_ONLY_DEMO
      "Change name",
#endif
      NULL
  };

  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Call settings Menu", menu_items),
                                             channel_number);
    switch( user_selection )
    {
      case 0:
        cid_demo_get_valid_phone_number( "calling number",
            port->cid_data.number, 15 );
        break;
#ifdef CID_ONLY_DEMO
      case 1:
        cid_demo_get_valid_name("name",  port->cid_data.name, SI_DEMO_MAX_NAME);
        break;
#endif
    }
  }while( user_selection != QUIT_MENU );
}

/****************************************************************************/
/*
 * presents a menu of options and then acts upon the user selection.
 */

void pbx_menu(pbx_demo_t *main)
{
  const char *menu_items[] =
  {
      "Change channel",
      "Change general CID settings",
      "Display CID settings",
      "Low level debug",
      "Quit Demo",
      NULL
  };

  int user_selection;
  int current_channel = main->demo_state.currentChannel;

  chanState_t *port = &(main->cid_states[current_channel]);

  /* Flush out the input buffer prior to reading the console input */
  while(kbhit() )
  {
    (void)getchar();
  }

  do
  {
    user_selection = get_menu_selection( display_menu("uPBX Demo Main Menu", menu_items),
                                            current_channel);
    switch(user_selection)
    {
      case 0:
        change_channel(&(main->demo_state));
        port = &(main->cid_states[main->demo_state.currentChannel]);
        break;

      case 1:
        cid_demo_change_settings(port, current_channel);
        break;

      case 2:
      {
        int i;
        print_banner("Calling plan");
        for(i = 0; i < main->demo_state.totalChannelCount; i++)
        {
#ifdef CID_ONLY_DEMO
          printf("Channel: %d name = %-30s number = %s\n", i,
              main->cid_states[i].cid_data.name,
              main->cid_states[i].cid_data.number
              );
#else
          printf("Channel: %d number = %s\n", i,
              main->cid_states[i].cid_data.number
              );
#endif
        }
        printf("\n");
      }
      break;

      case 3:
        debugMenu( &(main->demo_state ) );
        break;

      case 4:
        printf("Shutting down demo..\n");
        main->is_not_done = TRUE;
        user_selection = QUIT_MENU;
        break;
    }
  }while(user_selection != QUIT_MENU);

}

