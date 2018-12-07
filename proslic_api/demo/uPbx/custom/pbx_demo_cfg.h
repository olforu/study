/*
** Copyright (c) 2016-2017 by Silicon Laboratories
**
** $Id: pbx_demo_cfg.h 6342 2017-03-15 21:27:57Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
**
*/

#ifndef __PBX_DEMO_CFG__
#define __PBX_DEMO_CFG__

#define DEMO_PORT_COUNT           1
#define DEFAULT_CID_FSK_NA        0
#define DEFAULT_CID_RING_NA       0
#define PBX_PCM_PRESET            PCM_16LIN
#define PCM_PRESET_NUM_8BIT_TS    2 /* 1 = uLaw/aLaw, 2 = 16 bit linear */
#define PROSLIC_MAX_RING_PERIODS  2
#define PBX_MWI_ON_TIME           500 /* in mSec */
#define PBX_MWI_OFF_TIME          1000 /* in mSec */
#endif /* __PBX_DEMO_CFG__ */

