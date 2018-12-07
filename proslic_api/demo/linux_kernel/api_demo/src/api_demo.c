/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: api_demo.c 5365 2015-11-13 23:52:24Z nizajerk $
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
** main entry point for the proslic api demo communicating to a Linux
** kernel driver.
**
**
*/

/* For open() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* for perror */
#include <errno.h>

/* for ioctl */
#include <sys/ioctl.h>

#include "proslic.h"
#include "api_demo.h"
#include "user_intf.h"

#include "proslic_linux_api.h"


/*****************************************************************************************************/

static int proslic_connect(int *fd)
{
  uint8_t i;
  char    fn[PROSLIC_MAX_DEV_PATHNAME];

  for( i = 0; i < PROSLIC_MAX_DEVS; i++)
  {
    sprintf( fn, PROSLIC_DEV_NAME, i );
    PROSLIC_API_MSG( "Opening/initializing %s - this may take several seconds", fn);
    fd[i] = open( fn, O_RDWR);

    if( fd[i] == -1 )
    {
      PROSLIC_API_MSG( "Failed to open %s", fn);
      perror( PROSLIC_API_PREFIX );
      break;
    }
  }

  if(fd[i] == -1)
  {
    for(; i; i--)
    {
      PROSLIC_API_MSG( "Closing device %u", i);
      close(fd[i]);
    }
    return EIO;
  }

  return 0;
}

/*****************************************************************************************************/

/* Disconnect all devices */
static void proslic_disconnect(int *fds)
{
  uint8_t i;

  for(i = 0; i < PROSLIC_MAX_DEVS; i++)
  {
    if(fds[i] != -1)
    {
      close( fds[i] );
    }
  }
}

/*****************************************************************************************************/

/* print out basic info on the device - assumes already open() was called on the file descriptor.*/
static void get_dev_info(proslic_api_info_t *dev_info, uint8_t *base_channel)
{
  proslic_chan_if chan_if;

  if( ioctl(dev_info->fd, PROSLIC_IOCTL_GET_PORT_COUNT, &(dev_info->port_count)) != 0)
  {
    PROSLIC_API_MSG( "Failed: on  PROSLIC_IOCTL_GET_PORT_COUNT" );
    perror( PROSLIC_API_PREFIX );
    return;
  }

  if( ioctl(dev_info->fd, PROSLIC_IOCTL_GET_CHAN_COUNT, &(dev_info->total_channel_count)) != 0)
  {
    PROSLIC_API_MSG( "Failed: on  PROSLIC_IOCTL_GET_CHANNEL_COUNT" );
    perror( PROSLIC_API_PREFIX );
    return;
  }

  if( ioctl(dev_info->fd, PROSLIC_IOCTL_GET_PORT_CHAN, &(dev_info->channel_count)) != 0)
  {
    PROSLIC_API_MSG( "Failed: on  PROSLIC_IOCTL_GET_CHANNEL_COUNT" );
    perror( PROSLIC_API_PREFIX );
    return;
  }

  dev_info->base_channel_index = *base_channel;
  (*base_channel) += dev_info->channel_count;
}

/*****************************************************************************************************/
/* Print out the port info we have... */
static void print_dev_info(proslic_api_info_t *dev_info)
{
  printf( "Number of ports = %u%c", dev_info->port_count, PROSLIC_EOL);
  printf( "Number of total channels = %u%c", dev_info->total_channel_count, PROSLIC_EOL);
  printf( "Number of channels on this port = %u%c", dev_info->channel_count, PROSLIC_EOL);
}

/*****************************************************************************************************/
static void main_menu(proslic_api_info_t *devices, proslic_api_demo_state_t *state)
{
  const char *main_menu_items[] = 
  {
    "Select channel",
    "Debug Menu",
    "Linefeed Menu",
    "DC/DC Converter Menu",
    "Ringing Menu",
    "Audio Menu",
    "Tone Generator Menu",
    //"Interrupt Menu",
    //"Test & Monitor Menu",
    NULL
  };
  int user_selection;

  do
  {
    user_selection = get_menu_selection( display_menu("Main Menu", main_menu_items), state->current_channel);

    switch(user_selection)
    {
      case 0: //Select channel
        {
          uint8_t new_channel;
          do
          {
            uint8_t i;

            printf("Please enter new channel number (0-%d) %s", devices->total_channel_count-1, PROSLIC_PROMPT);
            new_channel = (uint8_t)get_int(0, devices->total_channel_count-1);

            if(new_channel < devices->total_channel_count)
            {
              state->current_channel = new_channel;

              /* See if we need to move to a different port... */
              for(i = 0; i < new_channel; i++)
              {
                if(new_channel < (devices->channel_count + devices->base_channel_index) )
                {
                  state->current_port = i;
                  break;
                }
              }    
            }
          } while(new_channel >= devices->total_channel_count);
        }
        break;

      case 1: 
        debug_menu(devices, state);
        break;

      case 2: 
        linefeed_menu(devices, state);
        break;

      case 3: 
        converter_menu(devices, state);
        break;

      case 4:
        ringing_menu(devices, state);
        break;

      case 5:
        audio_menu(devices, state);
        break;

      case 6:
        tonegen_menu(devices, state);
        break;

      default:
        break;
    }
  } while(user_selection != QUIT_MENU);
}

/*****************************************************************************************************/
uint8_t get_preset_count(int fd, int channel, int ioctl_num)
{
  proslic_chan_if chan_info;

  chan_info.channel = channel;

  if( ioctl(fd, ioctl_num, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to retrieve preset count");
    perror("PROSLIC_DEMO IOCTL error message");
    return 0;
  }
  
  return chan_info.byte_value;
}

/*****************************************************************************************************/
int set_preset(int fd, int channel, int preset_index, int ioctl_num)
{
  proslic_chan_if chan_info;

  chan_info.channel = channel;
  chan_info.byte_value = (uint8_t)(preset_index);

  if( ioctl(fd, ioctl_num, &chan_info) != 0)
  {
    PROSLIC_API_MSG("Failed to set preset");
    perror("PROSLIC_DEMO IOCTL error message");
    return -1;
  }
  
  return 0;
}

/*****************************************************************************************************/
void change_preset(int fd, int channel, int ioctl_get_count, int ioctl_set)
{
  int preset_value;
  uInt8 max_presets;

  max_presets = get_preset_count(fd, channel,ioctl_get_count);
  if( max_presets != 0)
  {
    printf("Please enter preset: 0-%u %s ", max_presets, PROSLIC_PROMPT);
    preset_value = get_int(0, max_presets); 
            
    if(preset_value > max_presets)
    {
      PROSLIC_API_MSG("Invalid value, operation aborted.");
    }
    else
    {
      set_preset(fd, channel, preset_value,ioctl_set);
    }
  }
  else
  {
    PROSLIC_API_MSG("preset count = 0, cancelled change.");
    }
}

/*****************************************************************************************************/

int main(void)
{
  int fd[PROSLIC_MAX_DEVS];
  int rc;
  uint8_t i;
  uint8_t base_channel = 0;
  proslic_api_info_t dev_info[PROSLIC_MAX_DEVS];
  proslic_api_demo_state_t current_state;

  PROSLIC_CLS;
  print_banner(PROSLIC_API_TITLE);
  printf( "Copyright 2015, Silicon Labs%c", PROSLIC_EOL);
  printf( "Demo version: %s%c", PROSLIC_API_DEMO_VERSION, PROSLIC_EOL);

  rc = proslic_connect(&fd);
  if(rc != 0)
  {
    return rc;
  }

  /* Query the kernel module for basic info */
  for(i = 0; i < PROSLIC_MAX_DEVS; i++)
  {
    dev_info[i].fd = fd[i];
    get_dev_info(&(dev_info[i]), &base_channel);
    if(dev_info[i].port_count != PROSLIC_MAX_DEVS)
    {
      PROSLIC_API_MSG("Failed to get correct port count on device: %u", i);
      proslic_disconnect(fd);
      return EIO;
    }

    print_dev_info( &(dev_info[i]) );
  }

  memset(&current_state, 0, sizeof(proslic_api_demo_state_t));
  main_menu(dev_info, &current_state);

  proslic_disconnect(fd);
  return rc;
}
