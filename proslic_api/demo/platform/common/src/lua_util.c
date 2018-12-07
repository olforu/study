/*
** Copyright (c) 2012-2016 by Silicon Laboratories
**
** $Id: lua_util.c 5714 2016-06-17 19:25:26Z nizajerk $
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This file contains routines to implement the Lua API hooks.
**
*/
#if defined(WIN32) && !defined(__GNUC__)
#include <time.h>
#else
#include <unistd.h> /* sleep() */
#endif

#include "lua_util.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "demo_common.h"
#include "si_voice_datatypes.h"
#include "si_voice.h"
#include "proslic.h"

#ifdef CID_ONLY_DEMO
#include "cid_demo_cfg.h"
#include "cid_demo.h"
extern BOOLEAN cid_dbg_cfg_send_raw(chanState_t *port, char *char_buf, int is_corrupted);
#endif

typedef int (*proslic_fptr_no_arg_t)(proslicChanType *);
typedef int (*proslic_fptr_uint8_arg_t)(proslicChanType *, uInt8);
typedef int (*proslic_fptr_int_arg_t)(proslicChanType *, int);
/* typedef int (*proslic_fptr_3int_arg_t)(proslicChanType *, int32, int, int); */
static demo_state_t *luaDemoState; /* Local copy of demo state so we could pull chanPtr info*/


/********************************************************************
 * Forward declarations...
 */
static int sivoice_readreg(lua_State *);
static int sivoice_writereg(lua_State *);
static int proslic_readRAM(lua_State *);
static int proslic_writeRAM(lua_State *);

static int lua_proslic_ToneGenSetup(lua_State *);
static int lua_proslic_ToneGenStop(lua_State *);
static int lua_proslic_ToneGenStart(lua_State *);

static int lua_proslic_SetLinefeedStatus(lua_State *);

static int lua_proslic_RingStop(lua_State *);
static int lua_proslic_RingStart(lua_State *);
static int lua_proslic_RingSetup(lua_State *);

static int lua_proslic_AudioGainSetup(lua_State *);

static int lua_proslic_EnableCID(lua_State *L);
static int lua_proslic_DisableCID(lua_State *L);
static int lua_proslic_FSKSetup(lua_State *L);

#ifdef CID_ONLY_DEMO
static int lua_send_cid_data_raw(lua_State *L);
#endif

static int lua_ring_N_times(lua_State *);
static int lua_si_sleep(lua_State *);

/********************************************************************/

lua_State *luaS;
static int totalChannelCount;

typedef struct
{
  const char *name;
  lua_CFunction func;
} luaL_reg;

static const luaL_reg lua_functions[] =
{
		{ "SiVoice_WriteReg", sivoice_writereg },
		{ "SiVoice_ReadReg", sivoice_readreg },
		{ "ProSLIC_WriteRAM", proslic_writeRAM},
		{ "ProSLIC_ReadRAM", proslic_readRAM},

		{ "ProSLIC_ToneGenSetup", lua_proslic_ToneGenSetup },
		{ "ProSLIC_ToneGenStop", lua_proslic_ToneGenStop },
		{ "ProSLIC_ToneGenStart", lua_proslic_ToneGenStart },

		{ "ProSLIC_RingStop", lua_proslic_RingStop },
		{ "ProSLIC_RingStart", lua_proslic_RingStart },
		{ "ProSLIC_RingSetup", lua_proslic_RingSetup },

		{ "ProSLIC_EnableCID", lua_proslic_EnableCID },
		{ "ProSLIC_FSKSetup", lua_proslic_FSKSetup },
		{ "ProSLIC_DisableCID", lua_proslic_DisableCID },

		{ "ProSLIC_SetLinefeedStatus", lua_proslic_SetLinefeedStatus },

    { "ProSLIC_AudioGainSetup", lua_proslic_AudioGainSetup },
#ifdef CID_ONLY_DEMO
    { "CID_SendCID_raw", lua_send_cid_data_raw},
#endif
    { "ProSLIC_Ring_N_Times", lua_ring_N_times},
    { "si_sleep", lua_si_sleep },

		{ NULL, NULL}
};

/********************************************************************
 ** get_lua_int_args
 *
 * Gets the num_args integer arguments.
 */
int get_lua_args(lua_State *L, const char *function_name, long *arg_array)
{
	int rc = 0;
	int i, num_args;

	num_args = lua_gettop(L);

	for(i = 1; i <= num_args; i++)
	{
		if(!lua_isnumber(L,i))
		{
            if(lua_isboolean(L, i))
            {
                arg_array[(i-1)] = lua_toboolean(L, i);
            }
            else
            {
			    char buf[80];
			    sprintf(buf, "argument %d: incorrect type to %s", i, function_name);
			    lua_pushstring(L, buf);
			    lua_error(L);
			    rc = -2;
            }
		}
		else
		{
			arg_array[(i-1)] = lua_tointeger(L, i);
		}
	}

	return rc;
}

/********************************************************************
 * Verify the argument list matches what is expected.  0 = OK, otherwise
 * error
 *
 * arg_types:  C = channel, u = Uint8, U = Uint16, i = int8, I = int16, s = string b = boolean, L = int32 
 */

int chk_lua_args(lua_State *L, const char *function_name, int num_expected_args, const char *arg_types)
{
	int i;
	unsigned long argUL;
	long argL;
	int rc = 0;

	/* Did we get the expected number of arguments ? */
	if(num_expected_args != lua_gettop(L))
	{
		return -1;
	}

	for(i = 1; i <= num_expected_args; i++)
	{
		switch(arg_types[i-1])
		{
			case 'C':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argL = lua_tointeger(L, i);

				if((argL >= totalChannelCount) || ( argL < 0))
				{
					rc = -3;
				}
				break;

			case 'u':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argUL = lua_tointeger(L, i);

				if(argUL > 255)
				{
					rc = -3;
				}
				break;

			case 'U':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argUL = lua_tointeger(L, i);

				if(argUL > 65535)
				{
					rc = -3;
				}
				break;

			case 'L':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argUL = lua_tointeger(L, i);

        /* On 32 bit systems this may cause a compiler warning since it will
           be always false.
        */
				if(argUL > 0xFFFFFFFFUL)
				{
					rc = -3;
				}
				break;

			case 'i':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argL = lua_tointeger(L, i);

				if(argL > 127)
				{
					rc = -3;
				}
				break;

			case 'b':
				if(!lua_isboolean(L, i))
				{
					rc = -2;
				}
				break;

			case 'I':
				if(!lua_isnumber(L, i))
				{
					rc = -2;
				}
				argL = lua_tointeger(L, i);

				if(argL > 32767)
				{
					rc = -3;
				}
				break;

			case 's':
				if(!lua_isstring(L, i))
				{
					rc = -2;
				}
				break;

			default:
				rc = -4;
				break;
		}

		if(rc)
		{
			break;
		}
	}

	if(rc)
	{
		luaL_error(L,"argument %d: incorrect type to %s rc = %d", i, function_name, rc);
	}

	return rc;
}

/********************************************************************
 * Set a Lua global variable
 */
static void set_lua_global(lua_State *L, const char *var_name, const char *var_value)
{
	lua_pushstring(L, var_value);
  lua_setglobal(L, var_name);
}

/********************************************************************
 * Lua interface to writing to a register.
 */
static int sivoice_writereg(lua_State *L)
{
	long int_array[3];
    
	if((chk_lua_args(L, __FUNCTION__, 3,"Cuu") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, SiVoice_WriteReg(chanPtr, int_array[1], int_array[2]) );
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1; /* Return the number of return values on the stack */
}

/********************************************************************
 * Lua interface to writeram
 */
static int proslic_writeRAM(lua_State *L)
{
	long int_array[3];
    
	if((chk_lua_args(L, __FUNCTION__, 3,"CUL") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, ProSLIC_WriteRAM(chanPtr, int_array[1], int_array[2]) );
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1; /* Return the number of return values on the stack */
}

/********************************************************************
 * Lua interface to reading RAM
 */
static int proslic_readRAM(lua_State *L)
{
  long int_array[2];
   
	if((chk_lua_args(L, __FUNCTION__, 2,"CU") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
      SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
      lua_pushnumber(L, ProSLIC_ReadRAM(chanPtr, int_array[1]));
	    lua_pushnumber(L,0); /* error code */
	}
    else
    {
	    lua_pushnumber(L,0); /* value read */
	    lua_pushnumber(L, -1);
    }
	return 2;
}

/********************************************************************
 * Lua interface to reading a register
 */
static int sivoice_readreg(lua_State *L)
{
  long int_array[2];
   
	if((chk_lua_args(L, __FUNCTION__, 2,"Cu") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
	    SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, (int)int_array[0]);
      lua_pushnumber(L, SiVoice_ReadReg(chanPtr, int_array[1]));
	    lua_pushnumber(L,0); /* error code */
	}
    else
    {
	    lua_pushnumber(L,0); /* value read */
	    lua_pushnumber(L, -1);
    }
	return 2;
}

/********************************************************************
 * Lua interface to sleep() - in terms of seconds
 */
static int lua_si_sleep(lua_State *L)
{
  long int_array[1];
   
	if((chk_lua_args(L, __FUNCTION__, 1,"U") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
#if defined(WIN32) && !defined(__GNUC__)
        Sleep(1000*int_array[0]);
        lua_pushnumber(L, 0);
#else
        lua_pushnumber(L, sleep(int_array[0]));
#endif
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 2;
}

/********************************************************************
 */
static int lua_no_arg_wrapper(lua_State *L, proslic_fptr_no_arg_t fptr)
{
  long int_array[1];
   
	if((chk_lua_args(L, __FUNCTION__, 1,"C") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, (fptr)(chanPtr) );
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}

/********************************************************************
 */
static int lua_uint8_arg_wrapper(lua_State *L, proslic_fptr_uint8_arg_t fptr, char *args)
{
  long int_array[2];
   
	if((chk_lua_args(L, __FUNCTION__, 2, args) == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, (fptr)(chanPtr, int_array[1]));
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}

/********************************************************************
 */
static int lua_int_arg_wrapper(lua_State *L, proslic_fptr_int_arg_t fptr, char *args)
{
  long int_array[2];
   
	if((chk_lua_args(L, __FUNCTION__, 2, args) == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, (fptr)(chanPtr, int_array[1]));
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}

/********************************************************************
 */
#if 0
static int lua_3int_arg_wrapper(lua_State *L, proslic_fptr_3int_arg_t fptr, char *args)
{
    int int_array[3];
   
	if((chk_lua_args(L, __FUNCTION__, 3, args) == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr = demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, (fptr)(chanPtr, int_array[1], int_array[2], int_array[3]));
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}
#endif

/********************************************************************
 */
static int lua_proslic_ToneGenStop(lua_State *L)
{
    return(lua_no_arg_wrapper(L, ProSLIC_ToneGenStop));
}

/********************************************************************
 */
static int lua_proslic_ToneGenStart(lua_State *L)
{
    return(lua_uint8_arg_wrapper(L, ProSLIC_ToneGenStart, "Cb") );
}

/********************************************************************
 */
static int lua_proslic_ToneGenSetup(lua_State *L)
{
    return(lua_int_arg_wrapper(L, ProSLIC_ToneGenSetup, "Cu") );
}

/********************************************************************
 */
static int lua_proslic_SetLinefeedStatus(lua_State *L)
{
    return(lua_uint8_arg_wrapper(L, ProSLIC_SetLinefeedStatus, "Cu") );
}

/********************************************************************
 */
static int lua_proslic_RingStop(lua_State *L)
{
    return(lua_no_arg_wrapper(L, ProSLIC_RingStop));
}

/********************************************************************
 */
static int lua_proslic_RingStart(lua_State *L)
{
    return(lua_no_arg_wrapper(L, ProSLIC_RingStart));
}

/********************************************************************
 */
static int lua_proslic_RingSetup(lua_State *L)
{
    return(lua_int_arg_wrapper(L, ProSLIC_RingSetup, "Cu") );
}

/********************************************************************
 */
static int lua_proslic_EnableCID(lua_State *L)
{
    return(lua_no_arg_wrapper(L, ProSLIC_EnableCID));
}

/********************************************************************
 */
static int lua_proslic_DisableCID(lua_State *L)
{
    return(lua_no_arg_wrapper(L, ProSLIC_DisableCID));
}

/********************************************************************
 */
static int lua_proslic_FSKSetup(lua_State *L)
{
    return(lua_int_arg_wrapper(L, ProSLIC_FSKSetup, "Cu"));
}

/********************************************************************
 * Power ringing (TIA-777A) send FSK data  - expects, channel, signaling
 * hex string, 0 = include CRC, 1 = corrupted CRC, 2 = no CRC
 */
#define SI_CID_DEMO_MAX_BYTES 100
#define PREAMBLE_LEN 11
#define PREAMBLE_BYTE 0x55

#ifdef CID_ONLY_DEMO
static int lua_send_cid_data_raw(lua_State *L)
{
   
	if(( chk_lua_args(L, __FUNCTION__, 4, "Cssu") == 0) )
	{
        chanState_t *port  = &(proslic_ports[lua_tointeger(L, 1)]);

        char *string     = (char *)lua_tostring(L, 3);
        int is_corrupted = lua_tointeger(L, 4);

        if(strcmp(lua_tostring(L, 2), "USPWR") == 0)
        {
            lua_pushnumber(L, cid_dbg_cfg_send_raw(port, string, is_corrupted) );
            return 1;
        }
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}
#endif
/********************************************************************
 */
static int lua_proslic_AudioGainSetup(lua_State *L)
{
  long int_array[4];
   
	if((chk_lua_args(L, __FUNCTION__, 4, "CIIu") == 0)
	 && (get_lua_args(L, __FUNCTION__, int_array) == 0))
	{
        SiVoiceChanType_ptr chanPtr =demo_get_cptr(luaDemoState, int_array[0]);
        lua_pushnumber(L, ProSLIC_AudioGainSetup(chanPtr, int_array[1], int_array[2], int_array[3]));
	}
    else
    {
	    lua_pushnumber(L, -1);
    }
	return 1;
}

/********************************************************************
 */
static int lua_ring_N_times(lua_State *L)
{
    int channel = lua_tointeger(L, 1);
    int times_to_run = lua_tointeger(L, 2);
    int i;
    SiVoiceChanType_ptr chanPtr;

    ProslicInt arrayIrqs[MAX_PROSLIC_IRQS];
    proslicIntType irqs;

    if((channel >= totalChannelCount) || (channel < 0))
    {
      char buf[80];
      sprintf(buf, "argument %d: incorrect value to %s", 1, __FUNCTION__);
      lua_pushstring(L, buf);
      lua_error(L);
    }

    if(times_to_run < 1) 
    {
        char buf[80];
		sprintf(buf, "argument %d: incorrect type to %s", 2, __FUNCTION__);
		lua_pushstring(L, buf);
		lua_error(L);
    }

    irqs.irqs = arrayIrqs;

    chanPtr = demo_get_cptr(luaDemoState, channel);
    ProSLIC_RingStart(chanPtr);

    printf("%s(%d): Waiting for %d rings\n", __FUNCTION__, __LINE__, times_to_run);

    do
    {
      	if (ProSLIC_GetInterrupts(chanPtr, &irqs) != 0)
	    {  
            for(i = 0; i < irqs.number; i++)
            {
                if(irqs.irqs[i] == IRQ_RING_T1)
                {
                    times_to_run--;
                    printf("%s(%d): Ring done\n", __FUNCTION__, __LINE__);
                }

                if(irqs.irqs[i] == IRQ_LOOP_STATUS)
                {
                    ProSLIC_RingStop(chanPtr);
	                lua_pushnumber(L,-1); /* error code */
                    return 1;
                }
            }
        }

    }while(times_to_run);

    ProSLIC_RingStop(chanPtr);
    lua_pushnumber(L,0); /* error code */
    return 1;
}

/********************************************************************
 * Run a Lua script..
 */
void run_lua_script(const char *fn)
{
	if(luaL_loadfile(luaS, fn) == 0)
	{
		if(lua_pcall(luaS, 0,0,0) != 0)
		{
			printf( "error running %s: %s",
					fn,
					lua_tostring(luaS, -1));
		}
	}
    else
    {
			printf( "error running %s: %s",
					fn,
					lua_tostring(luaS, -1));
    }
}

/********************************************************************
 * Initialize the Lua environment
 */
int init_lua(demo_state_t *state)
{
	const luaL_reg *function;
	char buf[10];

	printf("Lua code compiled in:  %s\n", LUA_COPYRIGHT);

	luaS = luaL_newstate(); /* Lua 5.2 uses this instead of lua_open()
	                          see http://www.lua.org/manual/5.1/manual.html#7.3*/

	if(luaS == 0)
	{
		return -1;
	}

	luaL_openlibs(luaS);
	totalChannelCount = state->totalChannelCount;
	luaDemoState = state;

	/* Register all functions to be seen by lua */
	for(function = lua_functions; function->func != NULL; function++)
	{
		lua_register(luaS, function->name, function->func);
	}

	/* Predefine some globals */
	sprintf(buf,"%d", totalChannelCount);
	set_lua_global(luaS, "PROSLIC_NUM_CHAN", buf);
	set_lua_global(luaS, "PROSLIC_VERSION", ProSLIC_Version() );

	return 0;
}

/********************************************************************
 * Teardown the Lua environment
 */
int quit_lua()
{
	lua_close(luaS);
    return 0;
}
