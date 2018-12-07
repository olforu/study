########################################################################
#
# Generic Makefile for ProSLIC_API Demo with Linux kernel driver.
# Copyright 2015-2017, Silicon Labs
#
# $Id: Makefile 6387 2017-04-18 16:30:24Z nizajerk $
#
# Usage:  make 
#
#
########################################################################
# -------------------
# Variables and Paths
# -------------------
.PHONY: all 

# These could alternately be set as env variables
DEMO_BUILD_DIR ?= ./
PROSLIC_API_DIR ?= $(DEMO_BUILD_DIR)/../../../..
DEMO_SRC_DIR ?= $(DEMO_BUILD_DIR)/../src
DEMO_INC_DIR ?= $(DEMO_BUILD_DIR)/../inc
DEMO_CUSTOM_DIR ?= $(DEMO_BUILD_DIR)/../custom

OBJ_DIR = obj
OBJ = o

########################################################################
# Compiler Flags
########################################################################
CFLAGS_COMMON = -D_CONSOLE -w
CFLAGS_NODBG = -O3
CFLAGS_DBG = -ggdb

########################################################################
# Device/Converter Specific Source
########################################################################

PROSLIC_CFLAGS += -I$(DEMO_INC_DIR) -I$(DEMO_CUSTOM_DIR) -I$(PROSLIC_API_DIR)/inc
PROSLIC_CFLAGS += -I$(PROSLIC_API_DIR)/demo/linux_kernel/proslic_core
PROSLIC_CFLAGS += -I$(PROSLIC_API_DIR)/demo/platform/common/inc

########################################################################
# Demo Source - different from the normal demo since it does not 
# communicate directly with the ProSLIC API.  Instead, it uses IOCTL's
# to communicate with the kernel module.
########################################################################

DEMO_CODE = api_demo.c user_intf.c debug.c linefeed.c \
            converter.c ringing.c audio.c tonegen.c

########################################################################
# Pull it all together
########################################################################

SRCS := $(DEMO_CODE)
OUT_FILE_PREFIX := api_demo

o_files_tmp := $(patsubst %.c,%.$(OBJ),$(notdir $(SRCS)))
OBJS := $(addprefix $(OBJ_DIR)/,$(o_files_tmp))
d_files := $(patsubst %.$(OBJ),%.d, $(OBJS))

CFLAGS = $(PROSLIC_CFLAGS) $(CFLAGS_COMMON) $(SPI_FLAGS)
LFLAGS = -lc $(SPI_IF_LIB)

ifdef DEBUG
CFLAGS += $(CFLAGS_DBG)
DBG_EXT = _DBG
else
CFLAGS += $(CFLAGS_NODBG)
endif

ifdef STRICT
CFLAGS += -Werror -Wall -Wextra -Wpedantic -Wunused-variable -Wunused-function -Wimplicit-function-declaration

endif

########################################################################
vpath %.d $(OBJ_DIR)
vpath %.$(OBJ) $(OBJ_DIR)
vpath %.c $(DEMO_SRC_DIR) $(PROSLIC_API_DIR)/demo/platform/posix/ui/basic_text

all: $(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT)

-include $(d_files)

$(OBJ_DIR)/%.$(OBJ) : %.c
	$(CC) -c $(CFLAGS) -o $(OBJ_DIR)/$(notdir $@) -MD $<

$(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

clean:
	-$(RM) $(OBJ_DIR)/*.$(OBJ) $(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT) $(OBJ_DIR)/*.d

