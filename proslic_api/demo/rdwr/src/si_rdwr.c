/*
** Copyright (c) 2013-2018 by Silicon Laboratories
**
**
** ProSLIC API system integration file for Linux
**
** $Id: si_rdwr.c 7065 2018-04-12 20:24:50Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This source file allows one to peek/poke register and RAM locations.
** This is a good test program to verfiy the SPI/PCM bus communication
** works prior to trying to bring up the ProSLIC API.
**
** Dependancies:
**
** A SPI & PCM driver.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

/* Standard ProSLIC API header files */
#include "si_voice_datatypes.h"
#include "proslic.h"
#include "spi_main.h"
#include "proslic_timer.h"
#include "user_intf.h"

#define SI_SPI_VALIDATION_LOOPCOUNT 1024 /* How many times to write/read verify per verification command */

#define NUMBER_OF_CHAN 4

enum
{
  SI_DEMO_RD,
  SI_DEMO_WR,
  SI_DEMO_IRD,
  SI_DEMO_IWR,
  SI_DEMO_SPI_VERIFY,
  SI_DEMO_USAGE,
  SI_DEMO_RESET,
  SI_DEMO_QUIT
};

typedef struct
{
  const char *option_name;
  unsigned int enum_value;
} si_string_2_enum;

/*****************************************************************************/

static void display_usage()
{
  printf("\nrd/wr demo application commands:\n");
  printf("---------------------------------\n");
  printf("rd <channel> <register>\t-read a register\n");
  printf("wr <channel> <register> <value>\t-write to a register\n");
  printf("rrd <channel> <RAM ADDRESS>\t-read a RAM/Indirect location\n");
  printf("rwr <channel> <RAM ADDRESS> <value>\t-write to a RAM/Indirect\n");
  printf("wrv <channel> <register>- Verify SPI Bus (typical register is 12 or 14)\n");
  printf("reset <0= out of reset|1 = in reset>\t - reset device(s)\n");
  printf("\nexit - quit program\n");
}

#define SI_DEMO_DELIMITERS " \t\n"

/*****************************************************************************/
/* Simple command parser - 1 = done, 0 = go on */
static int si_demo(controlInterfaceType *ctrl)
{
  char *user_strings[4];
  int i = 0;
  int option_count = 1; /* First one is the command... */
  int channel = 0;
  int reg_address = 0;
  unsigned int ram_address = 0;
  char input[80];
  char *usr_string;

  si_string_2_enum commands[] =
  {
    {"rd",     SI_DEMO_RD},
    {"wr",     SI_DEMO_WR},
    {"ird",    SI_DEMO_IRD},
    {"rrd",    SI_DEMO_IRD},
    {"iwr",    SI_DEMO_IWR},
    {"rwr",    SI_DEMO_IWR},
    {"wrv",    SI_DEMO_SPI_VERIFY},
    {"reset",  SI_DEMO_RESET},
    {"help",   SI_DEMO_USAGE},
    {"?",      SI_DEMO_USAGE},
    {"exit",   SI_DEMO_QUIT},
    {"quit",   SI_DEMO_QUIT},
    {NULL,     0}
  };

  printf("--> ");
  fgets(input,79, stdin);

  user_strings[i++] = strtok(input, SI_DEMO_DELIMITERS);

  while ( (usr_string = strtok(NULL , SI_DEMO_DELIMITERS)) != NULL)
  {
    user_strings[i++] = usr_string;
    option_count++;
  }

  /* Figure out which command was requested */
  for (i = 0; commands[i].option_name != NULL; i++)
  {
    if(strcmp(user_strings[0], commands[i].option_name) == 0)
    {
      break;
    }
  }

  /* Parse common items here */
  if ((commands[i].enum_value != SI_DEMO_QUIT)
      && (commands[i].enum_value != SI_DEMO_USAGE))
  {
    /* Pull in channel address 0 to  NUMBER_OF_CHAN-1 */
    if ( option_count > 1 )
    {
      channel = atoi(user_strings[1]);

      if( (channel <0) || (channel >= NUMBER_OF_CHAN))
      {
        printf("Invalid channel number\n");
        return 0;
      }
    }

    /* Commands that take in register address as the second argument */
    if ((commands[i].enum_value == SI_DEMO_RD)
        || (commands[i].enum_value == SI_DEMO_WR)
        || (commands[i].enum_value == SI_DEMO_SPI_VERIFY) )
    {
      if (option_count > 2)
      {
        reg_address = atoi(user_strings[2]);
      }

      if( (reg_address < 0) || (reg_address > 255))
      {
        printf("Invalid register address\n");
        return 0;
      }
    }

    /* Commands that take in a RAM address as the second argument */
    if ((commands[i].enum_value == SI_DEMO_IRD)
        || (commands[i].enum_value == SI_DEMO_IWR) )
    {
      if (option_count > 2)
      {
        ram_address = atol(user_strings[2]);
      }

      if (ram_address >= 1024)
      {
        printf("WARNING: addresses above 1024 need to have the UAM mode active - please refer to your applications note for your chipset on how to do this\n");
      }
    }

  } /* if not QUIT or USAGE */

  switch (commands[i].enum_value)
  {
    case SI_DEMO_RD:
      {
        if(option_count != 3)
        {
          display_usage();
        }
        else
        {
          reg_address = atoi(user_strings[2]);
          printf("rd result %u = %02x\n",
                 reg_address,
                 (ctrl->ReadRegister_fptr)( ctrl->hCtrl, channel, reg_address));
        }
      }
      break;

    case SI_DEMO_WR:
      {
        int value;

        if(option_count != 4)
        {
          display_usage();
          return 0;
        }

        value = atoi(user_strings[3]);

        if ((value < 0) || (value > 255))
        {
          printf("Invalid value\n");
          return 0;
        }

        (ctrl->WriteRegister_fptr)(ctrl->hCtrl, channel, reg_address, value);

      }
      break;

    case SI_DEMO_IRD:
      {
        if(option_count != 3)
        {
          display_usage();
          return 0;
        }
        printf("ird result %04x = %04x\n",
               (unsigned int)ram_address,
               (unsigned int)(ctrl->ReadRAM_fptr)( ctrl->hCtrl, channel, ram_address));
      }
      break;

    case SI_DEMO_IWR:
      {
        uInt32 value;

        if(option_count != 4)
        {
          display_usage();
          return 0;
        }

        value = atol(user_strings[3]);
        (ctrl->WriteRAM_fptr)( ctrl->hCtrl, channel, ram_address, value);
      }
      break;

    case SI_DEMO_SPI_VERIFY:
      {
        int write_data, read_data;
        int bad_rds = 0;

        if(option_count != 3)
        {
          display_usage();
          return 0;
        }


        for(i = 0; i < SI_SPI_VALIDATION_LOOPCOUNT; i++)
        {
          write_data = rand()%0xFF;
          (ctrl->WriteRegister_fptr)(ctrl->hCtrl,channel, reg_address, write_data);

          read_data = (ctrl->ReadRegister_fptr)(ctrl->hCtrl, channel, reg_address);

          if(write_data != read_data)
          {
            bad_rds++;
          }
        }
        printf("Read: %d, bad reads: %d\n",i,bad_rds);
      }
      break;

    case  SI_DEMO_RESET:
      {
        if(option_count != 2)
        {
          display_usage();
          return 0;
        }
        /* channel is actually the reset value - the parser does an atoi() early on */
        (ctrl->Reset_fptr)(ctrl->hCtrl,channel);
      }
      break;

    case SI_DEMO_USAGE:
      display_usage();
      break;

    case SI_DEMO_QUIT:
      return 1;
  }

  return 0;
}


/*****************************************************************************/
int main(int argc, char *argv[] )
{
  ctrl_S                spiGciObj; /* Link to host spi obj (os specific) */
  controlInterfaceType  hwIf;      /* No dynamic memory allocation */
  systemTimer_S         timerObj;  /* Link to host timer obj (os specific)*/

  SETUP_STDOUT_FLUSH;

  puts("ProSLIC API rdwr version 0.1.2");
  puts("Copyright 2013-2016, Silicon Laboratories, Inc.\n");

  /* Initialize platform */
  if(SPI_Init(&spiGciObj) == FALSE)
  {
    printf("%s: Cannot connect to %s\n", *argv, VMB_INFO);
    return -1;
  }
  /*
   ** Establish linkage between host objects/functions
   ** and ProSLIC API (required)
   */
  printf("Demo: Linking function pointers...\n");
  initControlInterfaces(&hwIf, &spiGciObj, &timerObj);
  display_usage();

  while(si_demo(&hwIf) == 0)
  {
  }

  /* Close any system resources */
  destroyControlInterfaces(&hwIf);

  return 0;
}
