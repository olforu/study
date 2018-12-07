########################################################################
#
# Generic Makefile for ProSLIC uPBX Demo
# Copyright 2017-2018, Silicon Labs
#
# $Id: Makefile 7065 2018-04-12 20:24:50Z nizajerk $
#
# Common Usage:  make [VMB<1|2>|SPIDEV|RSPI=1] <CHIPSET_TYPE>=1 [PROSLIC_CFG_SRC=<fn>] [DEBUG=1] [LOG_ALL=1] [ENABLE_CID=1]
#
# VMB1 | VMB2 | RSPI | SPIDEV - which SPI interface code to compile in.  VMB1 - Windows Only, SPIDEV - Linux Only.
# CHIPSET_TYPE - which chipset/converter to use - see buildtargets.txt for options.
# PROSLIC_CFG_SRC - override the default constants file for the ProSLIC.
# DEBUG - compile with debug symbols (useful for gdb or other debugger).
# LOG_ALL - Log all debug/trace messages to a log file.
# SPI_DEBUG - (VMB2 only) - log all SPI writes (RAM + Register).  Could be used to create a GUI script.
# FLUSH_STDOUT - flush console output - useful for Eclipse or other debug environments where piping is used to redirect STDOUT.
# NO_LBCAL - do not perform a LBCAL during initialization.
# ENABLE_CID - use the Caller ID framework (separate package you can download from Silicon Labs) to generate type 1 CID.
#            - Requires CID framework 1.1.0 or newer...
# ENABLE_MANUAL_RING - will use manual ring cadencing vs. ring timers (assumes ringer presets are set correctly in terms of ring timers disabled or set for a long period)
#
# MWI_ENABLE - enable MWI (neon light message waiting).  Will toggle neon light when system is idle. pbx_demo_cfg.h has timing settings.
#
# There are also some additional build options useful to check out code quality (STRICT for instance), but are more meant
# for internal testing...
#
# Example make VMB2=1 SI3217X_B_FB=1 ENABLE_CID=1
#
########################################################################
# -------------------
# Variables and Paths
# -------------------
.PHONY: all chk_params

# These could alternately be set as env variables
HOME_DIR = .
PROSLIC_API_DIR ?= $(HOME_DIR)/../../..
DEMO_SRC_DIR ?= $(HOME_DIR)/../src
DEMO_INC_DIR ?= $(HOME_DIR)/../inc
DEMO_CUSTOM_DIR ?= $(HOME_DIR)/../custom
CID_FRAMEWORK_DIR ?= $(HOME_DIR)/../../../../cid_framework
OBJ_DIR = obj
OBJ = o

PERL_PATH ?= /usr/bin/perl

########################################################################
# Compiler Flags
########################################################################
CFLAGS_COMMON = -D_CONSOLE -DUSB
CFLAGS_NODBG = -O3
CFLAGS_DBG = -ggdb -DENABLE_TRACES=1

########################################################################
# Device/Converter Specific Source
########################################################################

include ../../proslic_api_options.mk

PROSLIC_CFLAGS += -I$(DEMO_INC_DIR) -I$(DEMO_CUSTOM_DIR) -DSI_DEMO_XCONNNECT=1

########################################################################
# Demo options - LBCAL and other options...
########################################################################

ifdef NO_LBCAL
PROSLIC_CFLAGS += -DSKIP_LBCAL
endif

# Start with logging enabled at startup of the demo program.
ifdef LOG_ALL
PROSLIC_CFLAGS += -DENABLE_INITIAL_LOGGING=1 -DSI_ENABLE_LOGGING=1 -DENABLE_TRACES
endif

# SPI_DEBUG prints out ALL writes going to the chipsets. Currently supported in the VMB2 example driver.
ifdef SPI_DEBUG
PROSLIC_CFLAGS += -DSI_SPI_DEBUG=1
endif

ifdef FLUSH_STDOUT
PROSLIC_CFLAGS += -DSILABS_USE_SETVBUF=1
endif

########################################################################
# Common Source
########################################################################

include ../../proslic_api_core.mk

########################################################################
# Demo Source
########################################################################

DEMO_CODE = pbx_demo.c pbx_demo_dial_plan.c pbx_demo_exec.c  pbx_demo_dbg_menu.c\
	demo_common.c user_intf.c debug.c

ifdef ENABLE_CID

CID_CORE =  si_cid.c si_cid_enc_core.c \
	        si_cid_enc_na.c si_cid_int.c si_cid_proslic.c si_cid_version.c


CID_DEMO = cid_demo_util.c signaling_na.c cid_demo_na.c cid_demo_proslic.c

PROSLIC_CFLAGS += -I$(CID_FRAMEWORK_DIR)/inc  -I$(CID_FRAMEWORK_DIR)/demo/cid_demo_app/inc
PROSLIC_CFLAGS += -I$(CID_FRAMEWORK_DIR)/demo/generic/inc
PROSLIC_CFLAGS += -DCID_NOT_STANDALONE=1 -DCID_ONLY_DEMO=1
ifdef ENABLE_MANUAL_RING
PROSLIC_CFLAGS += -DCID_MANUAL_RING_ENABLE=1
DEMO_CODE += manual_ring.c
endif

else
# We check if manual ring cadencing is requested...
ifdef ENABLE_MANUAL_RING
PROSLIC_CFLAGS +=-DPROSLIC_MANUAL_RING=1
DEMO_CODE += manual_ring.c
endif
endif # ENABLE_CID

# MWI
ifdef MWI_ENABLE
PROSLIC_CFLAGS += -DPBX_DEMO_ENABLE_MWI=1  -DSIVOICE_NEON_MWI_SUPPORT=1
endif


ifeq ($(PROSLIC_FXS_SET),1)
DEMO_CODE+= pbx_demo_fxs_hook.c \
	pbx_demo_fxs_sm.c
endif

# DAA not supported at this time...

########################################################################
# Platform specific code
########################################################################

include ../../platform/platform_common.mk

########################################################################

CFLAGS = $(PROSLIC_CFLAGS) $(CFLAGS_COMMON) $(SPI_FLAGS) -DDEMO_INCLUDE=\"api_demo.h\"

# Figure out which constants file to include for the demo... we assume it's the same as the src file
# with just the extension changed...
ifdef PROSLIC_CFG_SRC
PROSLIC_CONSTANTS_HDR = $(PROSLIC_CFG_SRC:.c=.h)
CFLAGS += -DFXS_CONSTANTS_HDR=\"$(PROSLIC_CONSTANTS_HDR)\"
endif

#ifdef DAA_CFG_SRC
#DAA_CONSTANTS_HDR = $(DAA_CFG_SRC:.c=.h)
#CFLAGS += -DDAA_CONSTANTS_HDR=\"$(DAA_CONSTANTS_HDR)\"
#endif

ifeq ($(PROSLIC_OS),mingw)
LFLAGS += $(SPI_IF_LIB)
else
LFLAGS += -lc $(SPI_IF_LIB)
endif

ifdef DEBUG
CFLAGS += $(CFLAGS_DBG)
DBG_EXT = _DBG
else
CFLAGS +=$(CFLAGS_NODBG)
endif

ifdef STRICT
CFLAGS += -Werror -Wall -Wextra -Wunused-variable -Wunused-function -Wimplicit-function-declaration
endif

#
# Below are Clang - www.clang.llvm.org  options...
#
ifdef CLANG_CHKMEM
CFLAGS += -fsanitize=memory
LFLAGS += -fsanitize=memory
endif

ifdef CLANG_CHKADDR
CFLAGS += -fsanitize=address
LFLAGS += -fsanitize=address
endif

ifdef CLANG_CHKUDB
CFLAGS += -fsanitize=undefined
LFLAGS += -fsanitize=undefined
endif

########################################################################
# Set VPATHS for .d, .o, and .c files...
########################################################################

vpath %.d $(OBJ_DIR)
vpath %.$(OBJ) $(OBJ_DIR)
vpath %.c $(PROSLIC_API_DIR)/src $(DEMO_SRC_DIR) $(PROSLIC_API_DIR)/patch_files $(PROSLIC_PFORM_DIRS) $(PFORM_TIMER_DIR)\
	 $(DEMO_CUSTOM_DIR) $(PROSLIC_API_DIR)/demo/platform/posix/ui/basic_text ../src/autogen \
	 $(PROSLIC_API_DIR)/demo/api_demo/src\
	$(CID_FRAMEWORK_DIR)/src/core $(CID_FRAMEWORK_DIR)/demo/generic/src  $(CID_FRAMEWORK_DIR)/src/encoders \
	$(CID_FRAMEWORK_DIR)/demo/generic/src $(CID_FRAMEWORK_DIR)/demo/cid_demo_app/src \
	$(CID_FRAMEWORK_DIR)/demo/cid_demo_app/signaling $(CID_FRAMEWORK_DIR)/src

########################################################################
#  Main entry point... NOTE: we call make again since
#  if we don't gcc complains the autogenerated menu file
#  doesn't exists...
########################################################################

all: chk_params
	$(MAKE) $(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT)

########################################################################
ifneq ($(wildcard $(PERL_PATH)),)
CFLAGS +=-DPERL=1
ifdef PROSLIC_CONSTANTS_HDR
DEMO_CODE+= si$(PROSLIC_CHIPSET)_options.c
endif
ifdef DAA_CONSTANTS_HDR
DEMO_CODE+= si3050_options.c
endif

endif
ifeq ($(wildcard ../src/autogen/si$(PROSLIC_CHIPSET)_options.c),)
ifdef PROSLIC_CONSTANTS_HDR
$(shell $(PERL_PATH) extract_settings.pl $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR));
endif
ifdef DAA_CONSTANTS_HDR
DEMO_CODE+= si3050_options.c
$(shell $(PERL_PATH) extract_settings.pl $(DEMO_CUSTOM_DIR)/$(DAA_CONSTANTS_HDR))
endif
endif

si$(PROSLIC_CHIPSET)_options.c: $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR)
	@echo "Regenerating menu files..."
ifdef PROSLIC_CONSTANTS_HDR
	$(PERL_PATH) extract_settings.pl $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR)
endif
ifdef DAA_CONSTANTS_HDR
	$(PERL_PATH) extract_settings.pl $(DEMO_CUSTOM_DIR)/$(DAA_CONSTANTS_HDR)
endif

########################################################################
#
# Lua scripting support
#

ifdef LUA
DEMO_CODE += lua_util.c
CFLAGS += -DLUA=1
endif

########################################################################
# Pull it all together
########################################################################
SRCS := $(PROSLIC_SRC_CORE) $(PROSLIC_PFORM) $(PROSLIC_CHIPSET_SRC) \
		$(PROSLIC_CFG_SRC) $(DEMO_CODE) $(DAA_CFG_SRC) $(PROSLIC_PATCH_SRC) \
		$(CID_CORE) $(CID_DEMO)

o_files_tmp := $(patsubst %.c,%.$(OBJ),$(notdir $(SRCS)))
OBJS := $(addprefix $(OBJ_DIR)/,$(o_files_tmp))
d_files := $(OBJS:.$(OBJ)=.d)

########################################################################
chk_params:
ifndef PROSLIC_CFG_SRC
ifndef DAA_CFG_SRC
	$(error Need to select a chipset/converter combination. Examples: SI3217X_C_LCQC3W, SI3226X_C_FB)
endif
endif

ifndef SPI_FLAGS
	$(error Missing platform type: Need to set to VMB1=1 or VBM2=1 or SPIDEV=1 or RSPI=1)
endif

ifeq ($(PROSLIC_OS),unknown)
$(error "OS setup not detected")
endif

########################################################################

-include $(d_files)

$(OBJ_DIR)/%.$(OBJ) : %.c
	$(CC) -c $(CFLAGS) -o $(OBJ_DIR)/$(notdir $@) -MD $<

$(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

clean:
	-$(RM) $(OBJ_DIR)/*.$(OBJ) si3*$(OS_EXT) $(OBJ_DIR)/*.d ../src/autogen/*_options.c 2>/dev/null

test:
	-echo $(CFLAGS)
