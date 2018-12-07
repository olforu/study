/******************************************************************************
 * Copyright (c) 2015-2016 by Silicon Laboratories
 *
 * $Id: proslic_vmb2.c 6593 2017-06-30 22:34:05Z mjmourni $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements the VMB2 common functions.
 *
 */

#include <unistd.h>

#include "si_voice_datatypes.h"
#include "si_voice.h"
#include "si_voice_ctrl.h"
#include "proslic_vmb2.h"
#include "sivoice_example_os.h"

#define  SiVoiceByteMask(X) ((X) & 0xFF)
static ctrl_S *Global_interfacePtr;

#define DRVR_PREFIX "VMB2:"

#ifdef SI_SPI_DEBUG
#ifdef SI_ENABLE_LOGGING
/* NOTE: can loosely convert into a GUI script with grep VMB2 <logfn> |cut -b6- */
#define DRVR_PRINT(...) fprintf(SILABS_LOG_FP,__VA_ARGS__);fflush(SILABS_LOG_FP)
#else
#define DRVR_PRINT LOGPRINT
#endif /* LOGGING */
#else
#define DRVR_PRINT(...)
#endif

/******************************************************************************
 *
 * Reset the device
 *
 */

int ctrl_ResetWrapper (void *interfacePtr, int inReset)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  DRVR_PRINT("%s # Reset: %d\n", DRVR_PREFIX, inReset);

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ] = SIVOICE_VMB2_CMD_RESET;

  if(inReset != 0)
  {
    s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = SIVOICE_VMB2_RESET_ASSERT;
  }
  else
  {
    s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = SIVOICE_VMB2_RESET_DEASSERT;
  }

  return SiVoiceVMB2_SendCmd(interfacePtr, s_buf);
}

/******************************************************************************
 * Write to a direct register (8 bits)
 */

int ctrl_WriteRegisterWrapper(void *interfacePtr, unsigned char channel,
                              unsigned char regAddr, unsigned char regData)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  DRVR_PRINT("%sREGISTER %d = %02X # CHANNEL = %d\n", DRVR_PREFIX, regAddr, regData,
             channel);

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[  SIVOICE_VMB2_COMMAND_BYTE  ] = SIVOICE_VMB2_CMD_REG_WRITE;
  s_buf[  SIVOICE_VMB2_TERM_CID_BYTE ] = SiVoiceByteMask(channel);

  s_buf[ SIVOICE_VMB2_ADDR_LOW_BYTE ] = SiVoiceByteMask(regAddr);
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ]    = SiVoiceByteMask(regData);

  if ( SiVoiceVMB2_SendCmd(interfacePtr, s_buf) != RC_NONE )
  {
    return RC_SPI_FAIL;
  }

  return RC_NONE;
}

/******************************************************************************
 * Read the contents of a direct register.
 */

unsigned char ctrl_ReadRegisterWrapper(void *interfacePtr,
                                       unsigned char channel, unsigned char regAddr)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE  ] = SIVOICE_VMB2_CMD_REG_READ;
  s_buf[ SIVOICE_VMB2_TERM_CID_BYTE ] = SiVoiceByteMask(channel);
  s_buf[ SIVOICE_VMB2_ADDR_LOW_BYTE ] = SiVoiceByteMask(regAddr);

  if ( SiVoiceVMB2_ReadData(((ctrl_S *)interfacePtr), s_buf) == RC_NONE )
  {
    return s_buf[ SIVOICE_VMB2_DATA0_BYTE ];
  }
  else
  {
    return 0xFF; /* We didn't read it correctly... */
  }
}

/******************************************************************************
 * Write RAM
 */

int ctrl_WriteRAMWrapper(void *interfacePtr, unsigned char channel,
                         unsigned short address, ramData data)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  DRVR_PRINT("%sRAM %d = %08X # CHANNEL = %d\n", DRVR_PREFIX, address, data,
             channel);

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]  = SIVOICE_VMB2_CMD_RAM_WRITE;
  s_buf[ SIVOICE_VMB2_TERM_CID_BYTE ] = SiVoiceByteMask(channel);

  s_buf[ SIVOICE_VMB2_ADDR_HIGH_BYTE ] = SiVoiceByteMask((address)>>8);
  s_buf[ SIVOICE_VMB2_ADDR_LOW_BYTE ]  = SiVoiceByteMask((address));
  s_buf[ SIVOICE_VMB2_DATA3_BYTE ]     = SiVoiceByteMask((data)>>24);
  s_buf[ SIVOICE_VMB2_DATA2_BYTE ]     = SiVoiceByteMask((data)>>16);
  s_buf[ SIVOICE_VMB2_DATA1_BYTE ]     = SiVoiceByteMask((data)>>8);
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ]     = SiVoiceByteMask(data);

  if( SiVoiceVMB2_SendCmd(interfacePtr, s_buf) != RC_NONE )
  {
    return RC_SPI_FAIL;
  }

  return RC_NONE;
}

/******************************************************************************
 * Read from RAM (indirect registers)
 */

ramData ctrl_ReadRAMWrapper(void *interfacePtr,
                            uInt8 channel,
                            uInt16 ramAddr)

{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ] = SIVOICE_VMB2_CMD_RAM_READ;
  s_buf[ SIVOICE_VMB2_TERM_CID_BYTE ] = SiVoiceByteMask(channel);
  s_buf[ SIVOICE_VMB2_ADDR_HIGH_BYTE ] = SiVoiceByteMask( (ramAddr>>8) );
  s_buf[ SIVOICE_VMB2_ADDR_LOW_BYTE ] = SiVoiceByteMask( ramAddr );

  if( SiVoiceVMB2_ReadData((ctrl_S *)interfacePtr, s_buf) != RC_NONE )
  {
    return RC_SPI_FAIL;
  }

  return ( (s_buf[ SIVOICE_VMB2_DATA3_BYTE ] << 24)
           | (s_buf[ SIVOICE_VMB2_DATA2_BYTE ] << 16)
           | (s_buf[ SIVOICE_VMB2_DATA1_BYTE ] << 8)
           | (s_buf[ SIVOICE_VMB2_DATA0_BYTE ] ) );

}

/******************************************************************************
 *
 */

void SPI_SelectCS(unsigned char chipSelect)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]     = SIVOICE_VMB2_CMD_SPI_CS_SELECT;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ]  = chipSelect & 0x1;

  SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Change the SPI clock frequency setting .
 *
 */

int SPI_setSCLKFreq(unsigned char spiMode)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]     = SIVOICE_VMB2_CMD_SPI_SCLK_SELECT;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ]  = spiMode;

  return SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Read the PCM source setting.
 *
 * Returns the PCM source setting.
 */

unsigned char PCM_readSource()
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ] = SIVOICE_VMB2_CMD_PCM_OUTPUT_ENABLE;

  SiVoiceVMB2_ReadData( Global_interfacePtr, s_buf);

  return s_buf[ SIVOICE_VMB2_DATA0_BYTE ];
}

/******************************************************************************
 * Change the PCM source setting.
 *
 */

int PCM_setSource(unsigned char pcmSelect)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]     = SIVOICE_VMB2_CMD_PCM_OUTPUT_ENABLE;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ]  = pcmSelect;

  return SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Change the PCM clock setting.
 *
 */

int PCM_setPCLKFreq(unsigned char pcmFrequency)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_PCM_PCLK_SELECT;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = pcmFrequency;

  return SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Change the PCM Framesync
 *
 */

int PCM_setFsyncType(unsigned char pcmFramesync)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_PCM_FSYNC_SELECT;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = pcmFramesync;

  return SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Enable/Disable a cross connect on the PCM bus
 *
 */

int PCM_enableXConnect(unsigned char enable)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_PCM_XCONNECT_ENABLE;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = enable;

  return SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);
}

/******************************************************************************
 * Retrieve the firmware ID as an int.
 *
 */

unsigned int GetFirmwareID()
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_FIRMWARE_ID;

  SiVoiceVMB2_ReadData( Global_interfacePtr, s_buf);

  return ((s_buf[ SIVOICE_VMB2_DATA1_BYTE ] <<8)
          | (s_buf[ SIVOICE_VMB2_DATA0_BYTE ]) );
}

/******************************************************************************
 * Configure the PCM source in 1 call.. NOT implemented in VMB2.
 *
 */

unsigned short setPcmSourceExp(unsigned short internal,int freq,int extFsync)
{
  SILABS_UNREFERENCED_PARAMETER(internal);
  SILABS_UNREFERENCED_PARAMETER(freq);
  SILABS_UNREFERENCED_PARAMETER(extFsync);
  return 0;
}

/******************************************************************************
 * Configure the SPI Clock source
 *
 */

void SPI_SelectFormat(unsigned char fmt)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_SPI_SCLK_FMT;
  s_buf[ SIVOICE_VMB2_DATA0_BYTE ] = fmt;

  SiVoiceVMB2_SendCmd( Global_interfacePtr, s_buf);

}
/******************************************************************************
 * Read the EEPROM on the EVB - used for ID 
 */

unsigned char ReadEEPROMByte(unsigned short eAddr)
{
  uInt8 s_buf[ SIVOICE_VMB2_BUFFER_SZ ];

  SIVOICE_MEMSET(s_buf, 0, SIVOICE_VMB2_BUFFER_SZ);
  s_buf[ SIVOICE_VMB2_COMMAND_BYTE ]    = SIVOICE_VMB2_CMD_EEPROM_READ;
  s_buf[ SIVOICE_VMB2_ADDR_LOW_BYTE]    = (eAddr & 0XFF);
  s_buf[ SIVOICE_VMB2_ADDR_HIGH_BYTE]   = (eAddr >> 8) & 0xFF;

  SiVoiceVMB2_ReadData( Global_interfacePtr, s_buf);

  return ((s_buf[ SIVOICE_VMB2_DATA1_BYTE ] <<8)
          | (s_buf[ SIVOICE_VMB2_DATA0_BYTE ]) );
}

/******************************************************************************
 * Initialize the VMB2 and toggle reset
 *
 */

int SPI_Init(ctrl_S *interfacePtr)
{
  if (SiVoiceOpenIF(interfacePtr) != RC_NONE )
  {
    return FALSE;
  }
  Global_interfacePtr = interfacePtr;
  return TRUE;
}

/******************************************************************************
 * Shutdown the VMB2 without reseting the part
 *
 */

int SPI_Close(ctrl_S *interfacePtr)
{
  return SiVoiceCloseIF( interfacePtr );
}

