/*
** Copyright (c) 2010-2017 by Silicon Laboratories
**
** $Id: pbx_demo_dial_plan.c 6238 2017-01-20 21:51:17Z nizajerk $
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
** This file implements the dial plan.
**
*/

#ifdef CID_ONLY_DEMO
#include "si_cid.h"
#include "si_cid_fsm.h"
#include "si_cid_int.h"
#include "si_cid_sys_srvc.h"
#endif

#include "si_voice_datatypes.h"
#include "si_voice.h"
#include "proslic.h"
#include "pbx_demo.h"

/****************************************************************************/
/* Decode DTMF digit array read from the ProSLIC and return back the digits as
   an ASCII C string.
*/
static char *decode_digits(char *buf, unsigned int len)
{
  static char decoded[SI_DEMO_MAX_PH_NUM+1];
  const char decoder_ring[] = "D1234567890*#ABC";
  unsigned int i;

  if(len > SI_DEMO_MAX_PH_NUM)
  {
    len = SI_DEMO_MAX_PH_NUM;
  }

  for(i = 0; i < len; i++)
  {
    decoded[i] = decoder_ring[buf[i]];
  }

  decoded[i] = 0; /*NULL terminate */
  return(decoded);
}

/****************************************************************************/
int si_pbx_digit_map(pbx_demo_t *main, unsigned int chan)
{

  int i;
  int match = SI_PBX_DEMO_DIGCOL_NOMATCH;
  char *decoded_digits;

  decoded_digits = decode_digits(main->cid_states[chan].digits,
                                 main->cid_states[chan].digitCount);
#if 1
  printf("DBG: digits dialed for chan %d = %s\n", chan, decoded_digits);
#endif

  for(i = 0; i< main->demo_state.totalChannelCount; i++)
  {
#if 0
    printf("comparing %s to %s\n",
           main->cid_states[i].cid_data.number,
           decoded_digits);
#endif
    if(strncmp(main->cid_states[i].cid_data.number, decoded_digits,
               strlen(main->cid_states[i].cid_data.number)) == 0)
    {
      match = i;
      break;
    }
  }

  if(match >= 0)
  {
    if(main->chan_info[match].state != SI_PBX_DEMO_FXS_STATE_IDLE)
    {
      return SI_PBX_DEMO_DIGCOL_BUSY;
    }
  }

  return(match);
}

