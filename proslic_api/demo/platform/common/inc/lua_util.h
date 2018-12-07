/*
** Copyright (c) 2012-2016 by Silicon Laboratories
**
** $Id: lua_util.h 5730 2016-06-20 21:58:58Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file contains routines to implement the Lua language hooks.
**
*/
#ifndef LUA_UTIL_H_
#define LUA_UTIL_H_

#include <lua.h>
#include "demo_common.h"
int init_lua(demo_state_t *state);
int quit_lua(void);
void run_lua_script(const char *fn);

#endif /* LUA_UTIL_H_ */
