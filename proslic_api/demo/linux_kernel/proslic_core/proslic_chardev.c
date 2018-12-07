/*
** Copyright (c) 2015 by Silicon Laboratories
**
** $Id: proslic_chardev.c 7012 2018-02-22 20:02:10Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** char device interface file for ProSLIC Linux "core" api module.
**
**
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include <asm/uaccess.h>

#include "proslic_linux_core.h"
#include "proslic_linux_api.h"

#include "proslic_api_config.h"

#include "proslic.h"
#include "si_voice.h"

#ifdef SI3217X
#include "si3217x.h"
#include "vdaa.h"
extern Si3217x_General_Cfg Si3217x_General_Configuration;
#endif
#ifdef SI3218X
#include "si3218x.h"
#endif
#ifdef SI3226X
#include "si3226x.h"
#endif
#ifdef SI3228X
#include "si3228x.h"
#endif

#include "si32xxx_multibom_constants.h" /* Code assumes ALL constants files have the same enum values,
                                           if this incorrect, change this to point to your
                                           system's constants file.
                                        */

/*****************************************************************************************************/

typedef struct 
{
  uint8_t             lock_count;
  struct cdev         cdev;
  struct dev_t        *dev_t;
  proslic_core_t      *port;
} proslic_file_dev_t;

/*****************************************************************************************************/

static int proslic_major_num = -1;
static proslic_file_dev_t *proslic_file_info = NULL;
static dev_t proslic_dev;

/*****************************************************************************************************/

static int proslic_open(struct inode *node, struct file *fp)
{
  uint8_t i;
  proslic_core_t *port;
  proslic_file_dev_t *file_info;

  try_module_get(THIS_MODULE);

  /* Pull the minor number so we can access the correct "port" */
  i = iminor(node);

  
  if(unlikely( i >  PROSLIC_NUM_PORTS) )
  {
    module_put(THIS_MODULE);
    return -ENOENT;
  }

  file_info = &(proslic_file_info[i]);
  port = &(proslic_ports[i]);
  file_info->port =  port;
  fp->private_data = file_info;

  /* Since we do a bunch of initialization here, we have 1 open() per port */
  if(file_info->lock_count)
  {
    module_put(THIS_MODULE);
    return -EBUSY;
  }
  file_info->lock_count++;

  printk(KERN_INFO "%s proslic_api_init() being called\n", PROSLIC_CORE_PREFIX);
  /* We're going to assume if the init fails, it was due to an I/O error */
  if(ProSLIC_Init( port->channelPtrs, port->numberOfChan) != RC_NONE)
  {
    module_put(THIS_MODULE);
    return -EIO;
  }

  /* 
   * LBCal for ProSLIC is normally needed to be done once per hardware instance - meaning
   * it can be done at the factory and then left alone unless some rework is done.  One could 
   * use some non-volatile storage to restore the data once LBCal is done one time.  This can save several
   * seconds per channel at bootime... 
   * 
   * Since this is * system specific, this exercise is left to the user.  Basically:
   *
   *  int32 data[4];
   *
   * if(lbcal_data_not_present())
   * {
   *  ProsSLIC_LBCal(port->channels, ports->numberOfChan)
   *  for(i = 0; i < port->numberOfChan; i++)
   *  {
   *     ProSLIC_GetLBCalResult( &(port->channels[i]), data, &data[1], &data[2], &data[3]);
   *  }
   *  //Call my storage function now...
   *  }
   *  else
   *  {
   *    //Call my retrieval function here to populate the data...
   *    for(i = 0; i < port->numberOfChan; i++)
   *    {
   *       ProSLIC_LoadPreviousLBCal( &(port->channels[i]), data, &data[1], &data[2], &data[3] );
   *    }
   *  }
   */

    ProSLIC_LBCal(&(port->channels), port->numberOfChan);

    /* Load custom presets - default to the first preset... */
    for(i = 0; i < port->numberOfChan; i++)
    {
      SiVoiceChanType_ptr chan = &(port->channels[i]);
      ProSLIC_DCFeedSetup( chan, 0);
      ProSLIC_RingSetup( chan, 0);
      ProSLIC_ZsynthSetup( chan, 0);
    }

  
    for(i = 0; i < port->numberOfChan; i++)
    {
      SiVoiceChanType_ptr chan = &(port->channels[i]);

      ProSLIC_SetLinefeedStatus( chan, LF_FWD_ACTIVE );
      ProSLIC_EnableInterrupts( chan );
    }

  return 0;
}

/*****************************************************************************************************/

static int proslic_close(struct inode *node, struct file *fp)
{
  proslic_file_dev_t *file_info = (proslic_file_dev_t *) fp->private_data;

  file_info->lock_count--;
  SiVoice_Reset( *(file_info->port->channelPtrs) );
  module_put(THIS_MODULE);
  return 0;
}

/*****************************************************************************************************/
/* Make sure the "absolute" channel number is within the given port's range, if it is return back
 * in the channel parameter the relative value, otherwise, return error code.
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int check_chanParam(proslic_file_dev_t *file_info, proslic_chan_if *chan_info, uint8_t *channel)
#else
static long check_chanParam(proslic_file_dev_t *file_info, proslic_chan_if *chan_info, uint8_t *channel)
#endif
{
  *channel = chan_info->channel;

  if(*channel < file_info->port->channelBaseIndex)
  {
    return -ENXIO;
  }

  *channel -= file_info->port->channelBaseIndex;

  if(*channel > file_info->port->numberOfChan)
  {
    return -ENXIO;
  }
  return 0;
}

/*****************************************************************************************************/

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int proslic_ioctl(struct inode *inode, struct file *fp, unsigned int cmd, unsigned long ioctl_params)
#else
static long proslic_ioctl(struct file *fp, unsigned int cmd , unsigned long ioctl_params)
#endif
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
  int rc = 0;
#else
  long rc = 0;
#endif
  proslic_file_dev_t *file_info = (proslic_file_dev_t *) fp->private_data;
  proslic_chan_if chan_if;
  SiVoiceChanType_ptr chanPtr = NULL;

  if( (_IOC_TYPE(cmd) != PROSLIC_MAGIC_IOCTL_NUM)
    || ( _IOC_NR(cmd) > PROSLIC_IOCTL_COUNT))
  {
    return -EFAULT;
  }

  /* Make sure user has permissions */
  if( _IOC_DIR(cmd) && _IOC_READ )
  {
   rc = !access_ok(VERIFY_WRITE, (void __user *)ioctl_params, _IOC_SIZE(cmd));
  } else if(_IOC_DIR(cmd) && _IOC_WRITE)
  {
   rc = !access_ok(VERIFY_READ, (void __user *)ioctl_params, _IOC_SIZE(cmd));
  }

  if(rc)
  {
    return -EACCES;
  }

  /* Most of the IOCTL's use proslic_chan_if as a conduit to send data
   * to from userspace, do the common code here instead of duplicating it...
   */
  if(_IOC_NR(cmd) > 2) 
  {
    uint8_t channel;
    if(copy_from_user(&chan_if, (void *) __user ioctl_params, sizeof(proslic_chan_if)) != 0)
    {
      return -EFAULT;
    }

    rc = check_chanParam(file_info, &chan_if, &channel);
    if(rc != 0)
    {
      return rc;
    }
    chanPtr = file_info->port->channelPtrs[channel];
  }

  switch(cmd)
  {
    case PROSLIC_IOCTL_GET_CHAN_COUNT:
      put_user(proslic_chan_init_count, (uint8_t __user *)ioctl_params);
      return 0;

    case PROSLIC_IOCTL_GET_PORT_COUNT:
      put_user(PROSLIC_NUM_PORTS, (uint8_t __user *)ioctl_params);
      return 0;

    case PROSLIC_IOCTL_GET_PORT_CHAN:
      put_user(file_info->port->numberOfChan, (uint8_t __user *)ioctl_params);
      return 0;

    case PROSLIC_IOCTL_GET_DEV_TYPE:
      chan_if.byte_value = file_info->port->deviceType;
      if(copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT;
      }
      break;
      
    case PROSLIC_IOCTL_READ_REG:
      chan_if.byte_value = ProSLIC_ReadReg(chanPtr, chan_if.reg_address);
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_WRITE_REG:
      if (ProSLIC_WriteReg(chanPtr, chan_if.reg_address, chan_if.byte_value) != RC_NONE)
      {
        rc = -EFAULT;
      }
      break;

    case PROSLIC_IOCTL_READ_RAM:
      chan_if.word_value = ProSLIC_ReadRAM(chanPtr, chan_if.ram_address);
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_WRITE_RAM:
      if (ProSLIC_WriteRAM(chanPtr, chan_if.ram_address, chan_if.word_value) != RC_NONE)
      {
        rc = -EFAULT;
      }
      break;

    case PROSLIC_IOCTL_SET_LINE_STATE:
      if (chan_if.byte_value < 8)
      {
        ProSLIC_SetLinefeedStatus(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_GET_DCFEED_COUNT:
      chan_if.byte_value = DC_FEED_LAST_ENUM;
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_SET_DCFEED:
      if(chan_if.byte_value < DC_FEED_LAST_ENUM)
      {
        ProSLIC_DCFeedSetup(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_SET_CONVERTER_STATE:
      if( (chan_if.byte_value & 0xFE) == 0)
      {
        if(chan_if.byte_value)
        {

          ProSLIC_PowerUpConverter(chanPtr);
        }
        else
        {
          ProSLIC_PowerDownConverter(chanPtr);
        }
      }
      else
      {
        rc = -EINVAL;          
      }
      break;


    case PROSLIC_IOCTL_GET_TONE_COUNT:
      chan_if.byte_value = TONE_LAST_ENUM;
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_SET_TONE:
      if(chan_if.byte_value < TONE_LAST_ENUM)
      {
        ProSLIC_ToneGenSetup(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;


    case PROSLIC_IOCTL_TONE_ON_OFF:
      if( (chan_if.byte_value & 0xFC) == 0)
      {
        if(chan_if.byte_value == 0)
        {
          ProSLIC_ToneGenStart(chanPtr,0);
        }
        else if(chan_if.byte_value == 1)
        {
          ProSLIC_ToneGenStart(chanPtr,1);
        }
        else
        {
          ProSLIC_ToneGenStop(chanPtr);
        }
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

   case PROSLIC_IOCTL_GET_RINGER_COUNT:
      chan_if.byte_value = RINGING_LAST_ENUM;
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_SET_RINGER:
      if(chan_if.byte_value < RINGING_LAST_ENUM)
      {
        ProSLIC_RingSetup(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_SET_RINGER_STATE:
      if( (chan_if.byte_value & 0xFE) == 0)
      {
        if(chan_if.byte_value)
        {
          ProSLIC_RingStart(chanPtr);
        }
        else
        {
          ProSLIC_RingStop(chanPtr);
        }
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_GET_ZSYNTH_COUNT:
      chan_if.byte_value = IMPEDANCE_LAST_ENUM;
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_SET_ZSYNTH:
      if(chan_if.byte_value < IMPEDANCE_LAST_ENUM)
      {
        ProSLIC_ZsynthSetup(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_SET_RXTX_TS:
      if( ProSLIC_PCMTimeSlotSetup(chanPtr, chan_if.int_values[0], 
        chan_if.int_values[1]) != RC_NONE)
      {
        rc = -EINVAL;
      }
      break;

    case PROSLIC_IOCTL_SET_PCM_ON_OFF:
      if( (chan_if.byte_value & 0xFE) == 0)
      {
        if(chan_if.byte_value)
        {
          ProSLIC_PCMStart(chanPtr);
        }
        else
        {
          ProSLIC_PCMStop(chanPtr);
        }
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    case PROSLIC_IOCTL_GET_PCM_COUNT:
      chan_if.byte_value = PCM_LAST_ENUM;
      if( copy_to_user((void *) __user ioctl_params, &chan_if, sizeof(proslic_chan_if)) != 0)
      {
        rc = -EFAULT; 
      }
      break;

    case PROSLIC_IOCTL_SET_PCM:
      if(chan_if.byte_value < PCM_LAST_ENUM)
      {
        ProSLIC_PCMSetup(chanPtr, chan_if.byte_value);
      }
      else
      {
        rc = -EINVAL;          
      }
      break;

    default:
      break;
  }
  return rc;
}

/*****************************************************************************************************/

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = proslic_open,
  .release = proslic_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
  .ioctl = proslic_ioctl
#else
  .unlocked_ioctl = proslic_ioctl
#endif
};

/*****************************************************************************************************/

int proslic_api_char_dev_init()
{
  int rc;
  unsigned int i;

  proslic_file_info = kzalloc(sizeof(*proslic_file_info) * PROSLIC_NUM_PORTS, GFP_KERNEL);
  
  if(proslic_file_info == NULL)
  {
    return -ENOMEM;
  }

  rc =  alloc_chrdev_region(&proslic_dev, 0, PROSLIC_NUM_PORTS, PROSLIC_CHARDEV_BASE_NAME);
  
  proslic_major_num = MAJOR(proslic_dev);

  if(rc < 0) {
    kfree(proslic_file_info);
    proslic_file_info = NULL;
    printk(KERN_ALERT "%s Failed to register chardevice number with code: %d\n", 
      PROSLIC_CORE_PREFIX, rc);
    return rc;
  }

  for(i = 0; i < PROSLIC_NUM_PORTS; i++)
  {
    cdev_init( &(proslic_file_info[i].cdev), &fops);
    proslic_file_info[i].cdev.owner = THIS_MODULE;

    rc = cdev_add( (&proslic_file_info[i].cdev), (proslic_dev +i), 1); 

    if(rc < 0)
    {
      printk(KERN_ALERT "%s failed to add character device with error code: %d\n",
        PROSLIC_CORE_PREFIX, rc);
      return rc;
    }
  }

  return 0;
}  

/*****************************************************************************************************/

void proslic_api_char_dev_quit()
{
  unsigned int i;

  if(proslic_major_num >= 0)
  {
    unregister_chrdev_region(proslic_dev, PROSLIC_NUM_PORTS);
  }

  if(proslic_file_info)
  {
    for(i = 0; i < PROSLIC_NUM_PORTS; i++)
    {
      cdev_del( &(proslic_file_info[i].cdev) );
    }

    kfree(proslic_file_info);
    proslic_file_info = NULL;
  }

}

