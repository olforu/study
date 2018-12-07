########################################################################
#
# Generic Makefile for ProSLIC_API Demo
# Copyright 2014-2018, Silicon Labs
#
# $Id: Makefile 7093 2018-04-18 22:56:17Z nizajerk $
#
# Common Usage:  make [VMB<1|2>|SPIDEV|RSPI=1] <CHIPSET_TYPE>=1 [PROSLIC_CFG_SRC=<fn>] [DEBUG=1] [MLT=1] [LOG_ALL=1]
#
# VMB1 | VMB2 | RSPI | SPIDEV - which SPI interface code to compile in.  VMB1 - Windows Only, SPIDEV - Linux Only.
# CHIPSET_TYPE - which chipset/converter to use - see buildtargets.txt for options.
# PROSLIC_CFG_SRC - override the default constants file for the ProSLIC.
# DAA_CFG_SRC - override the default constants file for the DAA portion (Si32178/9 or Si3050)
# DEBUG - compile with debug symbols (useful for gdb or other debugger).
# MLT - compile the ProSLIC MLT library with the API demo.  You may need to adjust the MLT_API_DIR variable for your install.
# LOG_ALL - Log all debug/trace messages to a log file.
# SPI_DEBUG - (VMB2 only) - log all SPI writes (RAM + Register).  Could be used to create a GUI script.
# FLUSH_STDOUT - flush console output - useful for Eclipse or other debug environments where piping is used to redirect STDOUT.
# NO_VDAA - disable DAA menus even if DAA was enabled (Si32178/9)
# NO_LBCAL - do not perform a LBCAL during initialization.
# NO_TSTIN - do not build in the testin code.
#
# There are also some additional build options useful to check out code quality (STRICT for instance), but are more meant
# for internal testing...
#
# Example make VMB2=1 SI3217X_B_FB=1
#
########################################################################
# -------------------
# Variables and Paths
# -------------------
.PHONY: all chk_params

# These could alternately be set as env variables
DEMO_BUILD_DIR ?= ./
PROSLIC_API_DIR ?= $(DEMO_BUILD_DIR)/../../..
MLT_API_DIR ?= $(DEMO_BUILD_DIR)/../../../../proslic_mlt_api
DEMO_SRC_DIR ?= $(DEMO_BUILD_DIR)/../src
DEMO_INC_DIR ?= $(DEMO_BUILD_DIR)/../inc
DEMO_CUSTOM_DIR ?= $(DEMO_BUILD_DIR)/../custom
# Required for the perl script to end in /
AUTOGEN_DIR ?= $(DEMO_BUILD_DIR)/../src/autogen
OBJ_DIR = obj
OBJ = o

PERL_PATH ?= /usr/bin/perl

########################################################################
# Compiler Flags
########################################################################
CFLAGS_COMMON = -D_CONSOLE -DUSB
CFLAGS_NODBG = -O3
CFLAGS_DBG = -ggdb

########################################################################
# Device/Converter Specific Source
########################################################################

include $(DEMO_BUILD_DIR)/../../proslic_api_options.mk

PROSLIC_CFLAGS += -I$(DEMO_INC_DIR) -I$(DEMO_CUSTOM_DIR)

########################################################################
# Demo options - LBCAL, TESTIN, and DAA support (32178/9)
########################################################################

ifeq ($(PROSLIC_CHIPSET),3050)
PROSLIC_CFLAGS +=-DNOFXS_SUPPORT=1
NO_TSTIN=1
NO_LBCAL=1
endif

ifdef NO_LBCAL
PROSLIC_CFLAGS += -DSKIP_LBCAL
endif

ifndef NO_TSTIN
PROSLIC_TSTIN_SUPPORT=1
PROSLIC_CFLAGS += -DTSTIN_SUPPORT=1
endif

ifeq ($(findstring 3217x,$(PROSLIC_CHIPSET)),3217x)
ifdef NO_VDAA
PROSLIC_DAA_SUPPORT=0
endif
endif

# Start with logging enabled at startup of the demo program.
ifdef LOG_ALL
PROSLIC_CFLAGS += -DENABLE_INITIAL_LOGGING=1 -DSI_ENABLE_LOGGING=1
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

include $(DEMO_BUILD_DIR)/../../proslic_api_core.mk

########################################################################
# Demo Source
########################################################################

DEMO_CODE =api_demo.c debug.c user_intf.c demo_common.c

ifeq ($(PROSLIC_FXS_SET),1)
DEMO_CODE+= monitor.c \
	  linefeed.c converter.c ringing.c audio.c \
	  tonegen.c pm_mwi.c interrupt.c proslic_tstin_limits.c \
    proslic_main.c
endif

ifeq ($(PROSLIC_DAA_SUPPORT),1)
DEMO_CODE += daa_main.c daa_audio.c daa_linestate.c
PROSLIC_CFLAGS += -DVDAA_SUPPORT=1
endif


########################################################################
# Platform specific code
########################################################################

include $(DEMO_BUILD_DIR)/../../platform/platform_common.mk

########################################################################
# MLT Source (depends on MLT package 2.4.3 or newer)
########################################################################

ifdef MLT
include $(MLT_API_DIR)/example_code/mlt_api_core.mk
PROSLIC_SRC_CORE += $(MLT_SRC_CORE)  mlt_demo.c example_mlt_tests.c rencal.c
MLT_SRC_DIR = $(MLT_API_DIR)/src $(MLT_API_DIR)/example_code
PROSLIC_CFLAGS += $(MLT_CFLAGS) -DMLT_ENABLED=1 -I$(MLT_API_DIR)/example_code
endif

########################################################################

CFLAGS = $(PROSLIC_CFLAGS) $(CFLAGS_COMMON) $(SPI_FLAGS) -DDEMO_INCLUDE=\"api_demo.h\"

# Figure out which constants file to include for the demo... we assume it's the same as the src file
# with just the extension changed...
ifdef PROSLIC_CFG_SRC
PROSLIC_CONSTANTS_HDR = $(PROSLIC_CFG_SRC:.c=.h)
CFLAGS += -DFXS_CONSTANTS_HDR=\"$(PROSLIC_CONSTANTS_HDR)\"
endif

ifdef DAA_CFG_SRC
DAA_CONSTANTS_HDR = $(DAA_CFG_SRC:.c=.h)
CFLAGS += -DDAA_CONSTANTS_HDR=\"$(DAA_CONSTANTS_HDR)\"
endif

ifeq ($(PROSLIC_OS),mingw)
LFLAGS += $(SPI_IF_LIB)
else
LFLAGS += -lc $(SPI_IF_LIB)
endif

ifdef DEBUG
CFLAGS += $(CFLAGS_DBG)
DBG_EXT = _DBG
else
CFLAGS += $(CFLAGS_NODBG)
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
vpath %.c $(PROSLIC_API_DIR)/src $(DEMO_SRC_DIR) $(PROSLIC_API_DIR)/patch_files $(PROSLIC_PFORM_DIRS) $(PFORM_TIMER_DIR) $(DEMO_CUSTOM_DIR) $(MLT_SRC_DIR) $(PROSLIC_API_DIR)/demo/platform/posix/ui/basic_text $(AUTOGEN_DIR)

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
DEMO_CODE+= $(foreach CHIPSET, $(PROSLIC_CHIPSET), si$(CHIPSET)_options.c)
endif

ifeq ($(wildcard $(AUTOGEN_DIR)/si$(PROSLIC_CHIPSET)_options.c),)
ifdef PROSLIC_CONSTANTS_HDR
$(shell $(PERL_PATH) $(DEMO_BUILD_DIR)/extract_settings.pl $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR) $(AUTOGEN_DIR)); 
endif
ifdef DAA_CONSTANTS_HDR
$(shell $(PERL_PATH) $(DEMO_BUILD_DIR)/extract_settings.pl $(DEMO_CUSTOM_DIR)/$(DAA_CONSTANTS_HDR) $(AUTOGEN_DIR))
endif
endif

si$(PROSLIC_CHIPSET)_options.c: $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR)
	@echo "Regenerating menu files..."
ifdef PROSLIC_CONSTANTS_HDR
	$(PERL_PATH) $(DEMO_BUILD_DIR)/extract_settings.pl $(DEMO_CUSTOM_DIR)/$(PROSLIC_CONSTANTS_HDR)  $(AUTOGEN_DIR)
endif
ifdef DAA_CONSTANTS_HDR
	$(PERL_PATH) $(DEMO_BUILD_DIR)/extract_settings.pl $(DEMO_CUSTOM_DIR)/$(DAA_CONSTANTS_HDR)  $(AUTOGEN_DIR)
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
SRCS := $(PROSLIC_SRC_CORE) $(PROSLIC_PFORM) $(PROSLIC_CHIPSET_SRC) $(PROSLIC_CFG_SRC) $(DEMO_CODE) $(DAA_CFG_SRC) $(PROSLIC_PATCH_SRC)

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
	-$(RM) $(OBJ_DIR)/*.$(OBJ) si3*$(OS_EXT) $(OBJ_DIR)/*.d $(AUTOGEN_DIR)/*_options.c 2>/dev/null

test:
	-@echo $(CFLAGS)
	-@echo "-----------------------"
	-@echo $(OBJS)
	-@echo "-----------------------"
	-@echo $(DEMO_CODE)

