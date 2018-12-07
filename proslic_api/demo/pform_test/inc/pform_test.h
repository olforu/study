/******************************************************************************
 * Copyright (c) 2014-2018 by Silicon Laboratories
 *
 * $Id: pform_test.h 7065 2018-04-12 20:24:50Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This file implements a simple platform validation tool.
 *
 */

#ifndef __PFORM_TEST_HDR__
#define __PFORM_TEST_HDR__
#include "proslic_api_config.h"

#ifndef PROSLIC_LINUX_KERNEL
#define TEST_LOG LOGPRINT ("%-32.32s: ", __FUNCTION__); LOGPRINT
#define REPORT_LOG(RESULT) LOGPRINT("%-32.32s: %s\n", __FUNCTION__, \
  (RESULT == 0 ? "PASSED" : "FAILED"))
#else
#define PROSLIC_TEST_PREFIX "ProSLIC_TST: "
#define TEST_LOG(fmt,...) LOGPRINT ("%s %-32.32s: " fmt, PROSLIC_TEST_PREFIX, __FUNCTION__, __VA_ARGS__);
#define REPORT_LOG(RESULT) LOGPRINT("%s %-32.32s: %s\n", PROSLIC_TEST_PREFIX, __FUNCTION__, \
  (RESULT == 0 ? "PASSED" : "FAILED"))
#endif

#define LOOP_COUNT 10000 /* Used for timed tests */

typedef int (*silabs_test)(controlInterfaceType *hwIf, uInt8 channel);
int runTests(controlInterfaceType *hwIf, uInt8 channel, silabs_test *testList);

#endif /* __PFORM_TEST_HDR__ */
