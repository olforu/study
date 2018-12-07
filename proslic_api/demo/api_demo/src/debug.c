/*
** Copyright (c) 2013-2016 by Silicon Laboratories"", Inc.
**
** $Id: debug.c 6526 2017-05-08 19:08:33Z elgeorge $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** debugging resources
**
*/

#include <stdio.h>
#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"
#include "si_voice.h"
#include "spi_main.h"
#include "proslic.h"

#ifdef LUA
#include "lua_util.h"
#endif

#ifdef SI_ENABLE_LOGGING
extern int is_stdout;
extern FILE *std_out;
#endif

static const char *partNumStrings[] = 
{
  "SI32171", 
  "SI32172", 
  "SI32174",
  "SI32175",
  "SI32176",
  "SI32177",
  "SI32178",
  "SI32179",
  "SI32260",
  "SI32261",
  "SI32262",
  "SI32263",
  "SI32264",
  "SI32265",
  "SI32266",
  "SI32267",
  "SI32268",
  "SI32269",
  "SI32360",
  "SI32361",
  "SI32180",
  "SI32181",
  "SI32182",
  "SI32183",
  "SI32184",
  "SI32185",
  "SI32186",
  "SI32187",
  "SI32188",
  "SI32189", 
  "SI32280",
  "SI32281",
  "SI32282",
  "SI32283",
  "SI32284",
  "SI32285",
  "SI32286",
  "SI32287",
  "SI32289"
};

/*****************************************************************************************************/
#ifndef NOFXS_SUPPORT
static void softResetMenu(demo_state_t *state)
{
  const char *menu_items[] = 
  {
    "Soft reset",
    "Soft reset 2nd channel (dual channel devices only)",
    "Hard reset (dual channel devices only)",
    "Broadcast soft reset (1st channel)",
    "Broadcast hard reset",
    NULL
  };

  int user_selection;
  uInt16 resetOption[5] = {PROSLIC_SOFT_RESET, PROSLIC_SOFT_RESET_SECOND_CHAN, 
    PROSLIC_HARD_RESET, PROSLIC_BCAST_RESET|PROSLIC_SOFT_RESET,
    PROSLIC_BCAST_RESET|PROSLIC_HARD_RESET};

  user_selection = get_menu_selection(display_menu("Soft reset Menu", menu_items), state->currentChannel);

  if( (user_selection >= 0) && (user_selection <= 4) )
  {
    LOGPRINT( "%s soft reset return code: %d\n", LOGPRINT_PREFIX,
      ProSLIC_SoftReset(state->currentChanPtr, resetOption[user_selection]) );
  }
}
#endif

/*****************************************************************************************************/
static void print_device_info(SiVoiceChanType_ptr chanPtr)
{
  const char *chipString;

  if( chanPtr->deviceId->chipType == SI3050)
  {
    chipString = "SI3050";
  }
  else if (chanPtr->deviceId->chipType <= SI32289)
  {
    chipString = partNumStrings[chanPtr->deviceId->chipType];
  }
  else
  {
    chipString = "UNKNOWN";
  }

  LOGPRINT("%sClass = %s\n", LOGPRINT_PREFIX,
           (chanPtr->channelType== PROSLIC)?"ProSLIC":"DAA");

  LOGPRINT("%sType = %d Rev = %d chipType = %s(%d)\n", LOGPRINT_PREFIX, 
    chanPtr->channelType, chanPtr->deviceId->chipRev, chipString, 
    chanPtr->deviceId->chipType);

  if( chanPtr->channelType == DAA)
  {
    LOGPRINT("%slsRev = 0x%02X lsType = 0x%02X\n", LOGPRINT_PREFIX,
      chanPtr->deviceId->lsRev, chanPtr->deviceId->lsType);
  }
}

/*****************************************************************************************************/
static void dump_memory(SiVoiceChanType_ptr chanPtr, const char *filename)
{
  FILE *fp;
  int i;
  int max_reg;

  if(chanPtr->channelType == DAA)
  {
    max_reg = 60;
  }
  else
  {
    max_reg = 128;
  }

  if((fp = fopen(filename,"w"))==NULL)
  {
    printf("\nERROR OPENING %s!!\n", filename);
    return;
  }
  fprintf(fp,"#############\n");
  fprintf(fp,"Registers\n");
  fprintf(fp,"#############\n");
  for(i=0; i<max_reg; i++)
  {
    fprintf(fp,"REG %d = 0x%02X\n",i,SiVoice_ReadReg(chanPtr,i));
  }
#ifndef NOFXS_SUPPORT
  if(chanPtr->channelType == PROSLIC)
  {
    fprintf(fp,"#############\n");
    fprintf(fp,"RAM\n");
    fprintf(fp,"#############\n");
    for(i=0; i<1023; i++)
    {
      fprintf(fp,"RAM %d = 0x%08X\n", i, (int)ProSLIC_ReadRAM(chanPtr,i));
    }

    fprintf(fp,"#############\n");
    fprintf(fp,"MMREG\n");
    fprintf(fp,"#############\n");
    for(i=1024; i<1648; i++)
    {
      fprintf(fp,"RAM %d = 0x%08X\n", i, (int)ProSLIC_ReadRAM(chanPtr,(uInt16)i));
    }
  }
#endif
  fclose(fp);
}
/*****************************************************************************************************/
#ifndef NOFXS_SUPPORT
static void initMenu(demo_state_t *state)
{
  const char *menu_items[] =
  {
    "No options",
    "REINIT*",
    "NO CAL*",
    "NO PATCH LOAD*",
    "SOFTRESET*",
    NULL
  };
  int user_selection, num_items;

  do
  {
    num_items = display_menu("Debug Menu", menu_items);

    LOGPRINT("%s* = ProSLIC only\n", LOGPRINT_PREFIX);
    user_selection = get_menu_selection( num_items, state->currentChannel);

    switch(user_selection)
    {
      case 0:
        if(state->currentChanPtr->channelType == DAA)
        {
#ifdef VDAA_SUPPORT
          Vdaa_Init( &(state->currentChanPtr), 1);
#endif
        }
        else
        {
          ProSLIC_Init_with_Options( &(state->currentChanPtr), 1, INIT_NO_OPT);
        }
        break;

      case 1: /* Reinit */
        if(state->currentChanPtr->channelType == PROSLIC)
        {
          ProSLIC_Init_with_Options( &(state->currentChanPtr), 1, INIT_REINIT);
        }
        else
        {
          LOGPRINT("%s Unsupported option\n", LOGPRINT_PREFIX);
        }
        break;

      case 2: /* no Calibration */
        if(state->currentChanPtr->channelType == PROSLIC)
        {
          ProSLIC_Init_with_Options( &(state->currentChanPtr), 1, INIT_NO_CAL);
        }
        else
        {
          LOGPRINT("%s Unsupported option\n", LOGPRINT_PREFIX);
        }
        break;

      case 3: /* no patch load*/
        if(state->currentChanPtr->channelType == PROSLIC)
        {
          ProSLIC_Init_with_Options( &(state->currentChanPtr), 1, INIT_NO_PATCH_LOAD);
        }
        else
        {
          LOGPRINT("%s Unsupported option\n", LOGPRINT_PREFIX);
        }
        break;

      case 4: /* soft reset */
        if(state->currentChanPtr->channelType == PROSLIC)
        {
          ProSLIC_Init_with_Options( &(state->currentChanPtr), 1, INIT_SOFTRESET);
        }
        else
        {
          LOGPRINT("%s Unsupported option\n", LOGPRINT_PREFIX);
        }
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}
#endif

/*****************************************************************************************************/
void debugMenu(demo_state_t *state)
{
  const char *menu_items[] =
  {
#ifdef ENABLE_DEBUG
    "Toggle Debug Mode",
#else /* usually set in proslic_api_config.h */
    "Toggle Debug Mode (disabled due to compiler settings)",
#endif
    "Read Device Information",
    "Register/RAM Dump to file",
    "Read Register",
    "Write Register",
    "Read RAM",
    "Write RAM",
    "Place part into/out of free run mode (if supported)",
    "Reset EVB",
    "Init Channel",
#ifdef ENABLE_TRACES
    "Toggle Trace Mode",
#else /* usually set in proslic_api_conifg.h */
    "Toggle Trace Mode (disabled due to compiler settings)",
#endif
#if defined(VMB1) || defined(VMB2)
    "Change PCM/SPI settings",
#elif defined(LINUX_SPIDEV)
    "Change SPI settings",
#else
    "SPI settings disabled",
#endif
#ifdef SI_ENABLE_LOGGING
    "Toggle - Redirect DEBUGPRINT and TRACE_PRINT to a file",
#else
    "Logging disabled to a file",
#endif

#ifdef LUA
     "Execute LUA script (experimental)",
#else
    "Lua (disabled due to compiler settings)",
#endif
#ifdef NOFXS_SUPPORT
    "Soft reset (disabled due to compiler settings)",
#else
    "Soft reset",
#endif
    NULL
  };

  int user_selection, num_items;

  do
  {
    num_items = display_menu("Debug Menu", menu_items);
    LOGPRINT("%sDebug mode is %s\n", LOGPRINT_PREFIX,
             (DEBUG_ENABLED(state->currentChanPtr)?"Enabled": "Disabled"));
    LOGPRINT("%sTrace mode is %s\n", LOGPRINT_PREFIX,
             (TRACE_ENABLED(state->currentChanPtr)?"Enabled": "Disabled"));
#ifdef SI_ENABLE_LOGGING
    LOGPRINT("%sDebug & Trace redirected to a file is %s\n", LOGPRINT_PREFIX,
             (is_stdout ? "Disabled": "Enabled"));
#endif
    LOGPRINT("\n");
    user_selection = get_menu_selection( num_items, state->currentChannel);

    switch(user_selection)
    {
      case 0:
        SiVoice_setSWDebugMode(state->currentChanPtr,
                               DEBUG_ENABLED(state->currentChanPtr) == 0 );
        break;

      case 1:
        print_device_info(state->currentChanPtr);
        break;

      case 2:
        {
          char fn[BUFSIZ];
          get_fn("Please enter Log file name", fn);

          if(*fn)
          {
            dump_memory(state->currentChanPtr, fn);
          }
        }
        break;

      case 3:
        {
          int address;
          uInt8 data;

          do
          {
            printf("Please enter register address (dec) %s", PROSLIC_PROMPT);
            address = get_int(0,128);
          }
          while(address >128);

          data = SiVoice_ReadReg(state->currentChanPtr, (uInt8)address);
          LOGPRINT("%sReg %d = 0x%02X", LOGPRINT_PREFIX, address, data);
        }
        break;

      case 4:
        {
          int address;
          uInt8 data;

          do
          {
            printf("Please enter register address (dec) %s", PROSLIC_PROMPT);
            address = get_int(0,128);
          }
          while(address >128);

          printf("Please enter register data (hex) %s", PROSLIC_PROMPT);
          data = get_hex(0,255);
          
          SiVoice_WriteReg(state->currentChanPtr, (uInt8)address, (uInt8)data);
        }
        break;

      case 5:
#ifdef NOFXS_SUPPORT
        printf("Not supported for DAA\n");
#else
        {
          int address;
          uInt32 data;
          if(state->currentChanPtr->channelType == DAA)
          {
            printf("Not supported for DAA\n");
          }
          else
          {
            do
            {
              printf("Please enter RAM address (dec) %s", PROSLIC_PROMPT);
              address = get_int(0,0xFFFF);
            }
            while(address > 0XFFFF);

            data = ProSLIC_ReadRAM(state->currentChanPtr, (uInt16)address);
            LOGPRINT("%sRAM %d = 0x%08X", LOGPRINT_PREFIX, address, (int)data);
          }
        }
#endif
        break;

      case 6:
#ifdef NOFXS_SUPPORT
        printf("Not supported for DAA\n");
#else
        {
          int address;
          uInt32 data;

          if(state->currentChanPtr->channelType == DAA)
          {
            printf("Not supported for DAA\n");
          }
          else
          {
            do
            {
              printf("Please enter RAM address (dec) %s", PROSLIC_PROMPT);
              address = get_int(0,0xFFFF);
            }
            while(address >0XFFFF);

            do
            {
              printf("Please enter RAM data (hex) %s", PROSLIC_PROMPT);
              data = get_hex(0,0X1FFFFFFF);
            }
            while(data > 0x1FFFFFFF);
            ProSLIC_WriteRAM(state->currentChanPtr, (uInt16)address, (ramData)data);
          }
        }
#endif
        break;

      case 7: /* Free run */
#ifdef NOFXS_SUPPORT
        printf("Not supported for DAA\n");
#else
        {
          char user_input;
          if(state->currentChanPtr->channelType == DAA)
          {
            printf("Not supported for DAA\n");
          }
          else
          {
            do
            {
              printf("Enable free run mode (y/n) %s", PROSLIC_PROMPT);
            }
            while( !get_char("YyNn", &user_input));

            if( (user_input== 'Y') || (user_input == 'y'))
            {
              ProSLIC_PLLFreeRunStart(state->currentChanPtr);
            }
            else
            {
              ProSLIC_PLLFreeRunStop(state->currentChanPtr);
            }
          }
        }
#endif
        break;

      case 8:
        SiVoice_Reset(state->currentChanPtr);
        break;

      case 9:
          if(state->currentChanPtr->channelType == DAA)
          {
#ifdef VDAA_SUPPORT
            Vdaa_Init(&(state->currentChanPtr), 1);
#endif
          } 
          else
          {
#ifndef NOFXS_SUPPORT
            if(state->currentChanPtr->channelType == PROSLIC)
            {
              /* For ProSLIC, we have several init options, hence the menu */
              initMenu(state);
            }
#endif
          }
        break;

      case 10:
        SiVoice_setTraceMode(state->currentChanPtr,
                             TRACE_ENABLED(state->currentChanPtr) == 0 );
        break;

      case 11:
        vmbSetup(state->currentChanPtr->deviceId->ctrlInterface);
        break;

      case 12:
#ifdef SI_ENABLE_LOGGING
        if(is_stdout == 0)
        {
          fclose(SILABS_LOG_FP);
          SILABS_LOG_FP = std_out;
          is_stdout = 1;
          LOGPRINT("%sClosed log file\n", LOGPRINT_PREFIX);
        }
        else
        {
          char fn[BUFSIZ];
          std_out = SILABS_LOG_FP;

          get_fn("Enter log file name", fn);

          SILABS_LOG_FP = fopen(fn, "a");

          if(SILABS_LOG_FP == NULL)
          {
            perror("Failed to open log");
            SILABS_LOG_FP = std_out;
          }
          else
          {
            is_stdout = 0;
          }
        }
        break;
#endif
      case 13:
#ifdef LUA
      {
          char script_name[129];
          char sep_string[] = "---------------------------------";

          printf("Please enter Lua script name (128 chars max)\n--> ");

          scanf("%s", script_name);

          printf("Lua interpreter is: %s\n", LUA_COPYRIGHT);
          printf("ProSLIC API extensions are Copyright 2012-2016 by Silicon Labs\n\n");

          printf("Running script: %s\n%s\n", script_name,sep_string);
          run_lua_script(script_name);
          printf("\n%s\nScript DONE\n%s\n\n",sep_string, sep_string);
      }
#endif
        break;

      case 14:
        if(state->currentChanPtr->channelType == DAA)
        {
          printf("Not supported for DAA\n");
        }
        else
        {
#ifdef NOFXS_SUPPORT
        printf("Not supported for DAA\n");
#else
          softResetMenu(state); 
#endif
        }
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

