/*
** Copyright 2015-2016, Silicon Labs
** vmb2_vcp_wrapper.h
** VMB2 VCP driver header file
**
** Author(s): 
** cdp
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** File Description:
** This is the header file for VMB2 driver 
**
** Dependancies:
** vmb2_dll.lib, vmb2_dll.dll
**
*/

/*
** PCLK Frequency Selects 
*/
#define PCLK_SEL_8192    0x00
#define PCLK_SEL_4096    0x01
#define PCLK_SEL_2048    0x02
#define PCLK_SEL_1024    0x03
#define PCLK_SEL_512     0x04
#define PCLK_SEL_1536    0x05
#define PCLK_SEL_768     0x06
#define PCLK_SEL_1544    0x07

/*
** SCLK Freqency Selects 
*/
#define SCLK_SEL_12000   1
#define SCLK_SEL_8000    2
#define SCLK_SEL_4000    5 
#define SCLK_SEL_2000    11
#define SCLK_SEL_1000    23

/*
** FSYNC Selects
*/
#define FSYNC_SEL_SHORT  0x00
#define FSYNC_SEL_LONG   0x01

/*
** PCM OE Selects
*/
#define PCM_SRC_EXTERNAL  0x00
#define PCM_SRC_INTERNAL  0x01

/*
** DLL Function: getPortNum
**
** Description: 
** Returns the COM port # assigned to the open VCP 
**
** Input Parameters: 
** none
**
** Return:
** port number
*/
int getPortNum();

/*
** DLL Function: initVMB
**
** Description: 
** Initializes motherboard communication
**
** Input Parameters: 
** none
**
** Return:
** 1 - success
** 0 - failed to open port
*/
int initVMB();

/*
** DLL Function: GetVmbHandle
**
** Description: 
** Returns HANDLE to open VCP port
**
** Input Parameters: 
** none
**
** Return:
** handle
*/
unsigned long GetVmbHandle();

/*
** DLL Function: closeVMB
**
** Description: 
** Closes VCP port
**
** Input Parameters: 
** none
**
** Return:
** 1 - success
** 0 - failed to close port
*/
int closeVMB();

/*
** DLL Function: spiReadReg
**
** Description: 
** Reads a single ProSLIC register
**
** Input Parameters: 
** regAddr 	Address of register to read
** channel	Channel
**
** Return:
** register contents
*/
unsigned char spiReadReg (unsigned char regAddr, unsigned char channel);

/*
** DLL Function: spiWriteReg
**
** Description: 
** Writes a single ProSLIC register
**
** Input Parameters: 
** regAddr 	Address of register to write
** regData 	Data to write to register
** channel	Channel 
**
** Return:
** none
*/
void spiWriteReg (unsigned char regAddr, unsigned char regData, unsigned char channel);
void spiWrite2Reg (unsigned char regAddr0,unsigned char regData0,unsigned char regAddr1,unsigned char regData1,unsigned char channel);

void spiPollMstrstat(unsigned long resetTime, unsigned char mask);

void spiSelectClockFormat(unsigned short clockFormat);

/*
** DLL Function: spiReadRAM
**
** Description: 
** Reads a single ProSLIC RAM location
**
** Input Parameters: 
** ramAddr	Address of RAM location to read
** channel	Channel
**
** Return:
** data read from RAM location
*/
unsigned long spiReadRAM(unsigned short ramAddr, unsigned char channel);

/*
** DLL Function: spiWriteRAM
**
** Description: 
** Writes a single ProSLIC RAM location
**
** Input Parameters: 
** ramAddr	Address of RAM location to write
** ramData 	Data to write to RAM location
** channel      Channel
**
** Return:
** none
*/
void spiWriteRAM(unsigned short ramAddr, unsigned long ramData,
                 unsigned char channel);

/*
** DLL Function: spiSetSCLKFreq
**
** Description: 
** Sets the reset pin of the ProSLIC high
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void spiSetSCLKFreq(unsigned char sclk_freq_select);

/*
** DLL Function: spiReset0
**
** Description: 
** Sets the reset pin of the ProSLIC low
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void spiReset0();

/*
** DLL Function: spiReset1
**
** Description: 
** Sets the reset pin of the ProSLIC high
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void spiReset1();

/*
** DLL Function: spiSelectCS
**
** Description: 
** Selects which CS to use
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void spiSelectCS(unsigned char cs);

/*
** DLL Function: spiSetCS
**
** Description: 
** Sets the selected CS to cs
**
** Input Parameters: 
** cs - value to set cs pin to
**
** Return:
** none
*/
void spiSetCS(unsigned char cs);

/*
** DLL Function: spiGetSelectedCS
**
** Description: 
** retrieves which cs is selected
**
** Input Parameters: 
** none
**
** Return:
** which cs is in use
*/
unsigned char spiGetSelectedCS();

/*
** DLL Function: spiReadEEPROM
**
** Description: 
** Reads a unsigned char from EVB's EEPROM
**
** Input Parameters: 
** eAddr 	Address of unsigned char to read
**
** Return:
** memory contents
*/
unsigned char spiReadEEPROM (unsigned short eAddr);

/*
** DLL Function: spiWriteEEPROM
**
** Description: 
** Write a unsigned char to EVB's EEPROM
**
** Input Parameters: 
** eAddr 	Address of unsigned char to write
** eData    Data to write
**
** Return:
** none
*/
void spiWriteEEPROM (unsigned short eAddr,unsigned char eData);

/*
** DLL Function: pcmSetPCLKFreq
**
** Description: 
** Select PCLK Frequency
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void pcmSetPCLKFreq(unsigned char pclk_freq_select);

/*
** DLL Function: pcmSetFsyncType
**
** Description: 
** Select FSYNC pulse type
**
** Input Parameters: 
** none
**
** Return:
** none
*/
void pcmSetFsyncType(unsigned char fsync_select);

/*
** DLL Function: pcmSetSource
**
** Description: 
** Sets the PCM source (internal/external)
**
** Input Parameters: 
** 0 - External PCM
** 1 - Internal PCM
**
** Return:
** none
*/
void pcmSetSource(unsigned char pcm_internal_select);

/*
** DLL Function: pcmReadSource
**
** Description: 
** Reads the PCM source (internal/external)
**
** Input Parameters: 
** none
**
** Return:
** 0 - External PCM
** 1 - Internal PCM
*/
unsigned char pcmReadSource();

/*
** DLL Function: pcmCpldXconnect
**
** Description: 
** Reads the PCM source (internal/external)
**
** Input Parameters: 
** xconnect_enable - 0 = disable, 1 = enable
**
** Return:
** none
*/
void pcmCpldXconnect(unsigned char xconnect_enable);

/*
** DLL Function: spiReadDregSi321x
**
** Description: 
** Read the DReg on Si321x 
**
** Input Parameters: 
** regAddr 	register address
** chan    channel to read
**
** Return:
** register value
*/
unsigned char spiReadDregSi321x(unsigned char regAddr,unsigned char chan);

/*
** DLL Function: spiWriteDregSi321x
**
** Description: 
** Write the DReg on Si321x 
**
** Input Parameters: 
** regAddr 	register address
** regData  data to write
** chan    channel to read
**
** Return:
** none
*/
void spiWriteDregSi321x(unsigned char regAddr,unsigned char regData,
						unsigned char chan);
/*
** DLL Function: spiReadIregSi321x
**
** Description: 
** Read the IReg on Si321x 
**
** Input Parameters: 
** regAddr 	register address
** chan    channel to read
**
** Return:
** register value
*/
unsigned short spiReadIregSi321x(unsigned char regAddr,unsigned char chan);

/*
** DLL Function: spiWriteIregSi321x
**
** Description: 
** Write the IReg on Si321x 
**
** Input Parameters: 
** regAddr 	register address
** regData  data to write
** chan    channel to read
**
** Return:
** none
*/
void spiWriteIregSi321x(unsigned char regAddr,unsigned short regData,unsigned char chan);

/*
** DLL Function:Si3239xReadReg
**
** Description: 
** Read Si3239x SPI Reg
**
** Input Parameters: 
** regAddr 	Address of reg to read
**
** Return:
** status[11:0],data[11:0]
*/
unsigned long si3239xReadReg (unsigned char regAddr);

/*
** DLL Function: getFirmwareID
**
** Description: 
** Returns the Firware id in the form
**  bits[7:4]   Major Revision #
**  bits[3:0]   Minor Revision #
**
** Input Parameters: 
** none
**
** Return:
** firmware revision ID
*/
unsigned int getFirmwareID();


/*
** DLL Function:Si3239xWriteReg
**
** Description: 
** Write Si3239x SPI Reg
**
** Input Parameters: 
** regAddr 	Address of reg to read
** regData  Data to write
**
** Return:
** status[11:0]
*/
unsigned short si3239xWriteReg (unsigned char regAddr,unsigned short regData);

/*
** DLL Function:spiSi3217x_Init_RevB
**
** Description: 
** Initialize the Si3217x_revB with delay
**
** Input Parameters: 
** reset_delay 5us units of delay
**
** Return:
** none
*/
void spiSi3217x_Init_RevB(unsigned long reset_delay);
