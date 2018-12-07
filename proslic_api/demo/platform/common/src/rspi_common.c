/*
** Copyright 2017, Silicon Labs
** rspi_common.c
**
** $Id: rspi_common.c 6106 2017-08-01 23:56:16Z ketian $
** Rspi server implementation file
**
** Author(s):
** ketian
**
** Distributed by:
** Silicon Labs, Inc
**
** File Description:
** This is the implementation file for the SPI/PCM driver used
** in the ProSLIC demonstration code. It calls the library
** that initializes and talks to the voice motherboard.
**
**
*/


 #include "rspi_common.h"

 /*
 ** Function: RspiPrint
 **
 ** Description:
 ** printf's are conditionally disabled
 **
 ** Input Parameters:
 ** pointer to start of string
 **
 ** Return:
 ** none
 **
 */
 void RspiPrint(char* message)
 {
   #ifndef RSPI_NO_STD_OUT
   printf("%s", message);
   #endif
 }

 uInt8 RspiChecksum(char* buffer)
 {
   uInt8 sum = 0;
   uInt8 i = 0;
   for(i = 0; i < RSPI_CHECKSUM_LOCATION; i++)
   {
     sum += buffer[i];
   }
   return sum;
 }
