/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: debug.c 5150 2015-10-05 18:56:06Z nizajerk $
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
** Implements the debug menu and associated functions.
**
**
*/

#include <sys/ioctl.h>
#include "api_demo.h"
#include "user_intf.h"
#include "proslic_linux_api.h"

/*****************************************************************************************************/
uInt8 ReadReg(int fd, int channel, uInt8 address)
{
  proslic_chan_if chan_info;

  chan_info.reg_address = address;
  chan_info.channel = channel;
  if( ioctl(fd, PROSLIC_IOCTL_READ_REG, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to read register %u for channel %u", address, channel);
  }
  
  return chan_info.byte_value;
}

/*****************************************************************************************************/
int WriteReg(int fd, int channel, uInt8 address, uInt8 data)
{
  proslic_chan_if chan_info;

  chan_info.reg_address = address;
  chan_info.channel = channel;
  chan_info.byte_value = data;

  if( ioctl(fd, PROSLIC_IOCTL_WRITE_REG, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to write register %u for channel %u", address, channel);
    return -1;
  }
  
  return 0;
}

/*****************************************************************************************************/
int WriteRAM(int fd, int channel, uInt16 address, ramData data)
{
  proslic_chan_if chan_info;

  chan_info.ram_address = address;
  chan_info.channel = channel;
  chan_info.word_value = data;

  if( ioctl(fd, PROSLIC_IOCTL_WRITE_RAM, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to write RAM %ul for channel %u", address, channel);
    return -1;
  }
  
  return 0;
}

/*****************************************************************************************************/
ramData ReadRAM(int fd, int channel, uInt16 address)
{
  proslic_chan_if chan_info;

  chan_info.ram_address = address;
  chan_info.channel = channel;
  if( ioctl(fd, PROSLIC_IOCTL_READ_RAM, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to read RAM %ul for channel %u", address, channel);
  }
  
  return chan_info.word_value;
}

/*****************************************************************************************************/
/* Produces same output as the VMB2 API demo */
static void dump_memory(int fd, int channel, const char* filename)
{
  FILE *fp;
  int i;

	if((fp = fopen(filename,"w"))==NULL)
	{
		printf("\nERROR OPENING %s!!\n", filename);
		return;
	}

	fprintf(fp,"#############\n");
	fprintf(fp,"Registers\n");
	fprintf(fp,"#############\n");
	for(i=0; i<128; i++)
	{
		fprintf(fp,"REG %d = 0x%02X\n",(uInt8)i,ReadReg(fd, channel ,(uInt8)i));
	}
	fprintf(fp,"#############\n");
	fprintf(fp,"RAM\n");
	fprintf(fp,"#############\n");
	for(i=0; i<1023; i++)
	{
		fprintf(fp,"RAM %d = 0x%08X\n", (uInt16)i,ReadRAM(fd, channel,(uInt16)i));
	}
	fprintf(fp,"#############\n");
	fprintf(fp,"MMREG\n");
	fprintf(fp,"#############\n");
	for(i=1024; i<1648; i++)
	{
		fprintf(fp,"RAM %d = 0x%08X\n", (uInt16)i,ReadRAM(fd,channel,(uInt16)i));
	}

	fclose(fp);
}

/*****************************************************************************************************/
void debug_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *debug_menu_items[] =
  {
    "Register/RAM Dump",
    "Read Register",
    "Write Register",
    "Read RAM",
    "Write RAM",
    //"Change Freerun mode (if supported)",
    NULL
  };

  int user_selection;
  char *fn;
  
  do
  {
    user_selection = get_menu_selection( display_menu("Debug Menu", debug_menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0: 
        {
          char fn[BUFSIZ];
          get_fn("Please enter Log file name", fn);

          if(*fn)
          {
            dump_memory(devices[state->current_port].fd, state->current_channel, fn);
          }
        }
        break;

      case 1:
        { 
          int address;
          uInt8 data;

          do{
              printf("Please enter register address(dec) %s", PROSLIC_PROMPT);
              address = get_int(0,128);
            } while(address >128);

            data = ReadReg(devices[state->current_port].fd, state->current_channel, (uInt8)address);
            PROSLIC_API_MSG("Reg %d = 0x%02X", address, data);
        }
        break;

      case 2:
        { 
          int address;
          uInt8 data;

          do{
              printf("Please enter register address(dec) %s", PROSLIC_PROMPT);
              address = get_int(0,128);
            } while(address >128);

          do{
              printf("Please enter register data(hex) %s", PROSLIC_PROMPT);
              data = get_hex(0,255);
            } while(data > 255);
          WriteReg(devices[state->current_port].fd, state->current_channel, (uInt8)address, (uInt8)data);
        }
        break;

      case 3:
        { 
          int address;
          uInt32 data;

          do{
              printf("Please enter RAM address(dec) %s", PROSLIC_PROMPT);
              address = get_int(0,0xFFFF);
            } while(address > 0XFFFF);

            data = ReadRAM(devices[state->current_port].fd, state->current_channel, (uInt16)address);
            PROSLIC_API_MSG("RAM %d = 0x%08X", address, data);
        }
        break;

      case 4:
        { 
          int address;
          uInt32 data;

          do{
              printf("Please enter RAM address(dec) %s", PROSLIC_PROMPT);
              address = get_int(0,0xFFFF);
            } while(address >0XFFFF);

          do{
              printf("Please enter RAM data(hex) %s", PROSLIC_PROMPT);
              data = get_hex(0,0X1FFFFFF);
            } while(data > 0x1FFFFFFF);
          WriteRAM(devices[state->current_port].fd, state->current_channel, (uInt16)address, (ramData)data);
        }
        break;

      default:
        break;

    }  /* switch statement */
    fflush(stdin);
  } while(user_selection!= QUIT_MENU);
}

