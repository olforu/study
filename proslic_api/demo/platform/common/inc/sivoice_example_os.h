/******************************************************************************
 * Copyright (c) 2014-2016 by Silicon Laboratories
 *
 * $Id: sivoice_example_os.h 5421 2016-01-13 00:55:14Z nizajerk $
 *
 * This file contains proprietary information.
 * No dissemination allowed without prior written permission from
 * Silicon Laboratories, Inc.
 *
 * File Description:
 *
 * This header file defines structure definitions used to communicate with the various
 * OS's supported by the demo applications.
 *
 * This file is subject to change.
 *
 */
#ifndef __SIVOICE_EXAMPLE_OS_HDR__
#define __SIVOICE_EXAMPLE_OS_HDR__

#ifdef SIVOICE_OS_POSIX
#ifdef SIVOICE_VMB2

#include <termios.h>
/*****************************************************************************/
/**@addtogroup SIVOICE_EXAMPLES
 * @{
 */
typedef struct
{
  int fd;
  struct termios old_termios;
} SiVoice_CtrlIF_t;

#endif /* SIVOICE_VMB2 */
#endif /* SIVOICE_OS_POSIX */

#ifdef LINUX_SPIDEV
typedef struct
{
  int spi_fd;
  int reset_fd;
} SiVoice_CtrlIf_t;

#endif

#define SiVoiceMsSleep(mSec) usleep((mSec));

/**@} SIVOICE_EXAMPLES */

#endif /* __SIVOICE_EXAMPLE_OS_HDR__ */

