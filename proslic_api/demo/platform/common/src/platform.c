/******************************************************************************
 * Copyright (c) 2015-2018 by Silicon Labs
 *
 * $Id: platform.c 7120 2018-04-20 19:54:07Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Labs, Inc.
 *
 * File Description:
 *
 * This file implements common VMB/VMB2 functions needed for the various demos.
 *
 */

#ifndef PROSLIC_LINUX_KERNEL
#include <ctype.h>
#include <stdio.h>
#include "proslic_timer.h"
#include "user_intf.h"
#else
#include "proslic_sys.h"
#endif

#include "proslic.h"
#include "spi_main.h"

/***********************************/

static void vmbReset(SiVoiceControlInterfaceType *ctrlInterface)
{
  /* Setup code keeps part in reset at prompt.... */
  ctrlInterface->Reset_fptr(ctrlInterface->hCtrl,0);

  /* For ISI, we needed to extend the delay to access the part -
     somewhere between 1 - 2 seconds is needed.
     Normally this delay can be shorter. */
  ctrlInterface->Delay_fptr(ctrlInterface->hTimer,1500);
}


/************************************/
#ifdef VMB2
static const uInt32 functionsBitmask = DEFAULT_VMB2_BITMASK;
#elif defined(RSPI)
static uInt32 functionsBitmask = DEFAULT_RSPI_BITMASK;
#elif defined(VMB1)
static const uInt32 functionsBitmask = DEFAULT_VMB1_BITMASK;
#elif defined(LINUX_SPIDEV)
static const uInt32 functionsBitmask = DEFAULT_SPI_DEV_BITMASK;
#endif
#ifdef LINUX_SPIDEV
extern uint32_t get_max_spi_speed();
extern void set_spi_speed(uint32_t speed);
#endif
#ifdef VMB1
#include "DLLWrapper.h"
#endif

void vmbSetup(SiVoiceControlInterfaceType *ProHWIntf)
{
#ifndef SILABS_NO_PROMPTS
    uInt8 m;
    char resp[4];
    int pcm_src = 1;

#ifdef RSPI
    uInt8 function[1] = {RSPI_GET_BIT_MASK};
    functionsBitmask = RspiServerInterface(function, sizeof(function), 3, NULL);
#endif

#ifdef VMB1
    int pcm_enums[] = {128,64,32,16,8,4,512,12,24};
#else
    int sclk_freq;
    int pclk_freq;
#endif

#ifdef LINUX_SPIDEV
    uint32_t spi_speed = 2000000; /* Hz */
    uint32_t max_spi_speed;
#else
    uInt16 firmware_version = DEFAULT_FIRMWARE_VER;
    int isISI = 0;
#endif

    SILABS_UNREFERENCED_PARAMETER(ProHWIntf);

    /* Determines if board is VMB1, VMB2, SPI Dev, or Custom */
    if((uInt32)(functionsBitmask & BOARD_MODEL) == IS_VMB1)
    {
      printf("Demo: VMB1 Connected\n");
    }
    else if((uInt32)(functionsBitmask & BOARD_MODEL) == IS_VMB2)
    {
#if defined(VMB2) || defined(RSPI)
      printf("Demo: VMB2 VCP Connected via %s\n", SPI_GetPortNum(ProHWIntf->hCtrl));
      firmware_version = GetFirmwareID();
      printf("Demo: VMB2 Firmware Version %d.%d\n", ((firmware_version >> 8)&0x00FF),
             (firmware_version & 0x00FF));

      /* Set some defaults */
      SPI_setSCLKFreq(VMB2_SCLK_1000);
      PCM_setPCLKFreq(VMB2_PCLK_2048);
#endif
    }
    else if((uInt32)(functionsBitmask & BOARD_MODEL) == IS_SPI_DEV)
    {
      printf("Demo: Linux SPI Dev Connected\n");
    }
    else
    {
      printf("Demo: Custom board Connected\n");
    }

    printf("\nChange Default Settings (y/n) ??   ->");
    FLUSH_STDOUT;
    (void) fgets(resp, 3, stdin);

    if(toupper((int)*resp) == 'Y')
    {
#ifndef LINUX_SPIDEV
      /* Determines if ISI is supported */
      if((uInt32)(functionsBitmask & ISI_CAPABLE) != 0)
      {
        /* Releases prior to firmware rev 1.3 did not support ISI */
        if(firmware_version >= FIRMWARE_ISI)
        {
          printf("\nDo you want to use SPI or ISI (s/i) ?? ->");
          FLUSH_STDOUT;
          (void) fgets(resp, 3, stdin);
          printf("\n\n");
          fflush(stdout);
        }
        else
        {
          *resp = 'S';
        }
      }
      else
      {
        *resp = 'S';
      }

      if(toupper((int)*resp) == 'I')
      {
#ifndef VMB1
        SPI_SelectFormat(2);
        printf("VMB2 now set for ISI.\n");
        isISI = 1;
#endif
      }

      /* Determines if SCLK can be modified */
      if((uInt32)(functionsBitmask & MODIFY_SCLK) != 0)
      {
        if(toupper((int)*resp) == 'S')
        {
          printf("\n\n");
          printf("-----------------\n");
          printf("Select SCLK Freq\n");
          printf("-----------------\n");
          printf("0. Default \n");
          printf("1. 1MHz\n");
          printf("2. 2MHz\n");
          printf("3. 4MHz\n");
          printf("4. 8MHz\n");
          printf("5. 12MHz\n");
          printf("-----------------\n");
          printf("Selection -> ");
          FLUSH_STDOUT;

          (void) fgets(resp, 3, stdin);
          printf("\n\n");
          fflush(stdout);
          #ifndef VMB1
          switch(*resp)
          {
            case '0':
              sclk_freq = VMB2_SCLK_1000;
              break;
            case '1':
              sclk_freq = VMB2_SCLK_1000;
              break;
            case '2':
              sclk_freq = VMB2_SCLK_2000;
              break;
            case '3':
              sclk_freq = VMB2_SCLK_4000;
              break;
            case '4':
              sclk_freq = VMB2_SCLK_8000;
              break;
            case '5':
              sclk_freq = VMB2_SCLK_12000;
              break;
            default:
              sclk_freq = VMB2_SCLK_1000;
          }
          SPI_setSCLKFreq(sclk_freq);
          #endif
        }
      }

      /* Determines if PCM source can be modified */
      if((uInt32)(functionsBitmask & MODIFY_PCM_SOURCE) != 0)
      {
        printf("-----------------\n");
        printf("Select PCM Source\n");
        printf("-----------------\n");
        printf("0. Internal \n");
        printf("1. External\n");
        printf("-----------------\n");
        printf("Selection -> ");

        FLUSH_STDOUT;
        (void) fgets(resp, 3, stdin);
        printf("\n\n");

        switch(*resp)
        {
          case '1':
            pcm_src = 0;
            if(isISI == 1)
            {
              printf("NOTE: ISI needs to have a 2.048 MHz PCLK\n");
            }
            break;

          default:
            pcm_src = 1;  /* internal */
            break;
        }

        /* Setup hardware */
        #ifndef VMB1
          PCM_setSource(pcm_src);
        #endif

        if( (pcm_src == 1) && (isISI == 0))
        {
          printf("\n\n");
          printf("-----------------\n");
          printf("Select PCLK Freq\n");
          printf("-----------------\n");
          printf("0. Default \n");
          printf("1. 8192kHz\n");
          printf("2. 4096kHz\n");
          printf("3. 2048kHz\n");
          printf("4. 1024kHz\n");
          printf("5. 512kHz\n");
#ifdef VMB1
          printf("6. 256kHz\n");
          printf("7. 32768kHz\n");
          printf("8. 768kHz\n");
          printf("9. 1536kHz\n");
#else
          printf("6. 1536kHz\n");
          printf("7. 768kHz\n");
          printf("8. 1544kHz\n");
#endif
          printf("-----------------\n");
          printf("Selection -> ");

          FLUSH_STDOUT;

          (void) fgets(resp, 3, stdin);
          m = atoi(resp);
          printf("\n\n");
          fflush(stdout);
#ifdef VMB1
          if((m <= 0) || (m > 9))
          {
            printf("Invalid choice, defaulting to 2.048 Mhz\n");
            m = 3;
          }
          setPcmSourceExp(pcm_src, pcm_enums[m-1], 0);
#else
          if((m < 1)||(m > 8))
          {
            pclk_freq = 3;
          }
          else
          {
            pclk_freq = m-1;
          }
          PCM_setPCLKFreq(pclk_freq);
#endif
        }
      }

    /* Determines if Frame sync can be modified */
    if((uInt32)(functionsBitmask & MODIFY_FSYNC) != 0)
    {
      if( (pcm_src == 1) && (isISI == 0))
      {
        printf("-----------------\n");
        printf("Select Framesync type\n");
        printf("-----------------\n");
        printf ("0. FSYNC Short\n");
        printf ("1. FSYNC Long\n");
        printf("-----------------\n");
        printf("Selection -> ");

        FLUSH_STDOUT;

        (void) fgets(resp, 3, stdin);
        m = atoi(resp);
        printf("\n\n");
        fflush(stdout);
#ifndef VMB1
        if(m == 1)
        {
          PCM_setFsyncType(VMB2_FSYNC_LONG);
        }
        else
        {
          PCM_setFsyncType(VMB2_FSYNC_SHORT);
        }
#endif
      }
    }
#ifndef VMB1

    /* Determines if Loopback can be modified */
    if((uInt32)(functionsBitmask & MODIFY_LOOPBACK) != 0)
    {

        /* PCM Cross-connect (loopback), requires firmware version 1.7 or higher */
        /* Allowed for Single channel devices, but will appear to do nothing */

        if(firmware_version >= FIRMWARE_LOOPBACK)
        {
          printf("-----------------\n");
          printf("Select PCM Cross-connect configuration\n");
          printf("-----------------\n");
          printf ("0. Loopback Enabled\n");
          printf ("1. Loopback Disabled\n");
          printf("-----------------\n");
          printf("Selection -> ");
          FLUSH_STDOUT;
          (void) fgets(resp, 3, stdin);
          m = atoi(resp);
          printf("\n\n");
          fflush(stdout);
          if(m == 0)
          {
            PCM_enableXConnect(0x01);
          }
          else
          {
            PCM_enableXConnect(0x00);
          }
        }
      }
#endif
#else
      /* Determines if SPI clock rate can be modified */
      if((uInt32)(functionsBitmask & MODIFY_SPI_CLOCK) != 0)
      {
        max_spi_speed = get_max_spi_speed();
        printf("Enter SPI clock rate: (1000-%u) -> ", max_spi_speed);

        FLUSH_STDOUT;

        (void) fgets(resp, 9, stdin);
        spi_speed = strtoul(resp,NULL, 0);
      }

    if((uInt32)(functionsBitmask & MODIFY_SPI_CLOCK) != 0)
    {
      set_spi_speed(spi_speed);
    }
#endif /* ! LINUX_SPIDEV */
  }
#endif /* NO PROMPTS */
}

/* vmbSetup() calls */

/************************************/
void initControlInterfaces(SiVoiceControlInterfaceType *ProHWIntf, void *spiIf,
                           void *timerObj)
{

  SiVoice_setControlInterfaceCtrlObj (ProHWIntf, spiIf);
  SiVoice_setControlInterfaceTimerObj (ProHWIntf, timerObj);
  SiVoice_setControlInterfaceSemaphore (ProHWIntf, NULL);

#ifndef PROSLIC_LINUX_KERNEL
  SiVoice_setControlInterfaceReset (ProHWIntf, ctrl_ResetWrapper);

  SiVoice_setControlInterfaceWriteRegister (ProHWIntf, ctrl_WriteRegisterWrapper);
  SiVoice_setControlInterfaceReadRegister (ProHWIntf, ctrl_ReadRegisterWrapper);
  SiVoice_setControlInterfaceWriteRAM (ProHWIntf, ctrl_WriteRAMWrapper);
  SiVoice_setControlInterfaceReadRAM (ProHWIntf, ctrl_ReadRAMWrapper);

  SiVoice_setControlInterfaceDelay (ProHWIntf, time_DelayWrapper);
  SiVoice_setControlInterfaceTimeElapsed (ProHWIntf, time_TimeElapsedWrapper);
  SiVoice_setControlInterfaceGetTime (ProHWIntf, time_GetTimeWrapper);
#else
  /* The kernel code uses an array of functions that are exported to the global symbol table,
   * therefore, we'll access them this way to initialize the control interface.
   */
  SiVoice_setControlInterfaceReset (ProHWIntf, proslic_spi_if.reset);

  SiVoice_setControlInterfaceWriteRegister (ProHWIntf, proslic_spi_if.write_reg);
  SiVoice_setControlInterfaceReadRegister (ProHWIntf, proslic_spi_if.read_reg);
  SiVoice_setControlInterfaceWriteRAM (ProHWIntf, proslic_spi_if.write_ram);
  SiVoice_setControlInterfaceReadRAM (ProHWIntf, proslic_spi_if.read_ram);

  SiVoice_setControlInterfaceDelay (ProHWIntf, proslic_timer_if.slic_delay);
  SiVoice_setControlInterfaceTimeElapsed (ProHWIntf,
                                          proslic_timer_if.elapsed_time);
  SiVoice_setControlInterfaceGetTime (ProHWIntf, proslic_timer_if.get_time);

#endif

#ifndef PROSLIC_LINUX_KERNEL
  ctrl_ResetWrapper(spiIf,
                    1); /* Since we may be changing clocks in vmbSetup() , keep the part in reset */

  /*
    	** Configuration of SPI and PCM internal/external
  */
  vmbSetup(ProHWIntf);

  /*
   ** Call host/system reset function (omit if ProSLIC tied to global reset)
   */
#ifndef SILABS_NO_PROMPTS
  printf("Demo: Resetting device...\n");
#endif
#endif
  vmbReset(ProHWIntf);
}

/************************************/
void destroyControlInterfaces(SiVoiceControlInterfaceType *ProHWIntf)
{
#if defined(VMB2) || defined(RSPI) || defined(LINUX_SPIDEV)
  SPI_Close(ProHWIntf->hCtrl);
#else
  SILABS_UNREFERENCED_PARAMETER(ProHWIntf);
#endif

}
