/******************************************************************************
 * Copyright (c) 2014-2016 by Silicon Laboratories
 *
 * $Id: spi_tests.h 6007 2016-10-03 16:26:33Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements some basic SPI communications test to the ProSLIC.
 *
 */

#ifndef __SPI_TESTS_HDR__
#define __SPI_TESTS_HDR__ 1

#include "proslic.h"
#include "pform_test.h"

/* Entry points to run tests. 0 = OK */
int spiTests(controlInterfaceType *hwIf, uInt8 channel);
int spiTimedTests(controlInterfaceType *hwif, uInt8 channel);

/* Common SPI test functions  0 = OK, used as helper functions for the varius SPI tests */
int spiReadRevIDTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                           uInt8 regAddr);
int spiBasicWriteTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                            uInt8 regAddr);
int spiResetTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                       uInt8 regAddr);

/* Common SPI timed test functions 0 = OK, used as helper functions for various timed SPI tests */
int spiReadTimedTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                           uInt8 regAddr, uInt32 count);
int spiWriteTimedTestCommon(controlInterfaceType *hwIf, uInt8 channel,
                            uInt8 regAddr, uInt32 count);

/**********************************************************
 *  Check for register/RAM access across multiple channels.
 * 
 *
 */
int SpiMultiChanTest(controlInterfaceType *hwIf, uInt8 channel_count);

#endif /* __SPI_TESTS_HDR__ */

