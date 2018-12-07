/*
** Copyright (c) 2007-2018 by Silicon Laboratories
**
** $Id: si_voice_datatypes.h 7059 2018-04-09 19:37:04Z nizajerk $
**
** si_voice_datatypes.h
** ProSLIC datatypes file
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the header file that contains
** type definitions for the data types
** used in the demonstration code.
**
*/
#ifndef DATATYPES_H
#define DATATYPES_H
#include "proslic_api_config.h"

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

#if defined(PROSLIC_LINUX_KERNEL)
#include <linux/types.h>
typedef u_int8_t            BOOLEAN;
typedef int8_t              int8;
typedef u_int8_t            uInt8;
typedef uInt8               uChar;
typedef int16_t             int16;
typedef u_int16_t           uInt16;
typedef int32_t             int32;
typedef u_int32_t           uInt32;
#elif defined(WIN32)
#include <wtypes.h>
typedef char                int8;
typedef unsigned char       uInt8;
typedef uInt8               uChar;
typedef short int           int16;
typedef unsigned short int	uInt16;
typedef long                int32;
typedef unsigned long       uInt32;
#else
#include <stdint.h>
typedef uint8_t             BOOLEAN;
typedef int8_t              int8;
typedef uint8_t             uInt8;
typedef uint8_t             uChar;
typedef int16_t             int16;
typedef uint16_t            uInt16;
typedef int32_t             int32;
typedef uint32_t            uInt32;
#endif

/*
** RAM data 
*/
typedef uInt32 ramData;


#ifndef PROSLIC_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#define SIVOICE_CALLOC        calloc
#define SIVOICE_FREE          free
#define SIVOICE_MALLOC        malloc
#define SIVOICE_MEMSET        memset
#define SIVOICE_MEMCPY        memcpy
#define SIVOICE_STRCPY        strcpy
#define SIVOICE_ABS           abs
#else
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/kernel.h> /* for abs() */
/* NOTE: kcalloc was introduced in ~2.6.14, otherwise use kzalloc() with (X)*(Y) for the block size */
#define SIVOICE_CALLOC(X,Y)   kcalloc((X),(Y), GFP_KERNEL)
#define SIVOICE_FREE(X)       kfree((X))
#define SIVOICE_MALLOC(X)     kmalloc((X), GFP_KERNEL)
#define SIVOICE_MEMSET        memset
#define SIVOICE_STRCPY        strcpy
#define SIVOICE_ABS           abs
#endif  /* PROSLIC_LINUX_KERNEL */

#endif


