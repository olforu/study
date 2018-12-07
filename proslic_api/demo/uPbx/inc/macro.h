/*
** Copyright (c) 2013-2016 by Silicon Laboratories, Inc.
**
** $Id: macro.h 5654 2016-05-13 23:44:33Z nizajerk $
**
** macro.h
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains macros for commonly used procedures
**
*/

#ifndef __SILABS_DEMO_MACRO_HDR__
#define __SILABS_DEMO_MACRO_HDR__  1
/*
** Function pointer macros
** Note: Presume pState->currentChanPtr is the channel structure
*/

#define Delay           pState->currentChanPtr->deviceId->ctrlInterface->Delay_fptr
#define pProTimer       pState->currentChanPtr->deviceId->ctrlInterface->hTimer
#define TimeElapsed     pState->currentChanPtr->deviceId->ctrlInterface->timeElapsed_fptr
#define GetTime         pState->currentChanPtr->deviceId->ctrlInterface->getTime_fptr

/*
** Expression macros
*/
#define TOGGLE(x)               ((x) ^= (1 << (x&0x0001)))
#define GET_ENABLED_STATE(x)    (((x) == 0)?"disabled":"enabled")
#define GET_ACTIVE_STATE(x)     (((x) == 0)?"inactive":"active")
#define GET_ABRUPT_STATE(x)     (((x) == 0)?"smooth":"abrupt")
#define TEST_RESULT(X)          (((X) == RC_TEST_PASSED)?"PASSED":"FAILED")
#define GET_BOOL_STATE(X)       ((X) ?"true":"false")
#define SI_MIN(X,Y) ((X)>(Y)?(Y):(X))

#if 0
/*
** Bitmasks
*/
#define DIAG_SEL_HR_VTIPC   0x11
#define DIAG_SEL_HR_VRINGC  0x12
#define DIAG_SEL_HR_VBAT    0x13
#define DIAG_SEL_HR_VDC     0x14
#define DIAG_SEL_HR_ILONG   0x16
#define DIAG_SEL_HR_VLONG   0x17
#define DIAG_SEL_HR_ILOOP   0x1A
#endif
#endif /* __SILABS_DEMO_MACRO_HDR__ */

