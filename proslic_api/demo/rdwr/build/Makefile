########################################################################
#
# Generic Makefile for ProSLIC_API Platform tester
# Copyright 2015-2017, Silicon Labs
#
# $Id: Makefile 7065 2018-04-12 20:24:50Z nizajerk $
#
# Usage:  make [VMB<1|2>|RSPI=1] [SPIDEV=1]
#
# Example make VMB2=1
#
########################################################################
# -------------------
# Variables and Paths
# -------------------
.PHONY: all chk_params

# These could alternately be set as env variables
DEMO_BUILD_DIR ?= ./
PROSLIC_API_DIR ?= $(DEMO_BUILD_DIR)/../../..

DEMO_SRC_DIR ?= $(DEMO_BUILD_DIR)/../src
DEMO_INC_DIR ?= $(DEMO_BUILD_DIR)/../inc
DEMO_CUSTOM_DIR ?= $(DEMO_BUILD_DIR)/../custom

OBJ_DIR = obj
OBJ = o
OUT_FILE_PREFIX = rdwr

# --------------
# Compiler Flags
# --------------
CFLAGS_COMMON = -D_CONSOLE -DUSB
CFLAGS_NODBG = -O3
CFLAGS_DBG = -ggdb

PROSLIC_CFLAGS += -I$(DEMO_INC_DIR) -I$(DEMO_CUSTOM_DIR) -I$(PROSLIC_API_DIR)/inc

# -------------
# Demo options
# -------------
# SPI_DEBUG prints out ALL writes going to the chipsets. Currently supported in the VMB2 example driver.
ifdef SPI_DEBUG
PROSLIC_CFLAGS += -DSI_SPI_DEBUG=1
endif

# -----------
# Demo Source
# -----------
DEMO_CODE = si_rdwr.c si_voice.c si_voice_version.c

# --------------
# Platform specific code
# --------------
include $(DEMO_BUILD_DIR)/../../platform/platform_common.mk

ifeq ($(PROSLIC_OS),unknown)
$(error "OS setup not detected")
endif

# -------------------------
# Pull it all together
# -------------------------
SRCS := $(PROSLIC_SRC_CORE) $(PROSLIC_PFORM) $(DEMO_CODE)

o_files_tmp := $(patsubst %.c,%.$(OBJ),$(notdir $(SRCS)))
OBJS := $(addprefix $(OBJ_DIR)/,$(o_files_tmp))
d_files := $(patsubst %.$(OBJ),%.d, $(OBJS))

CFLAGS = $(PROSLIC_CFLAGS) $(CFLAGS_COMMON) $(SPI_FLAGS) $(TEST_TYPE)

ifeq ($(PROSLIC_OS),mingw)
LFLAGS = $(SPI_IF_LIB)
else
LFLAGS = -lc $(SPI_IF_LIB)
endif

ifdef DEBUG
CFLAGS += $(CFLAGS_DBG)
DBG_EXT = _DBG
else
CFLAGS += $(CFLAGS_NODBG)
endif

ifdef STRICT
CFLAGS += -Werror -Wall
endif


# --------------------
#
# ---------------
vpath %.d $(OBJ_DIR)
vpath %.$(OBJ) $(OBJ_DIR)
vpath %.c $(DEMO_SRC_DIR) $(PROSLIC_PFORM_DIRS) $(PROSLIC_API_DIR)/src

all: chk_params $(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT)

chk_params:
ifndef SPI_FLAGS
	$(error Need to either set VMB1=1 or VBM2=1 or SPIDEV=1 or RSPI=1)
endif


-include $(d_files)


$(OBJ_DIR)/%.$(OBJ) : %.c
	$(CC) -c $(CFLAGS) -o $(OBJ_DIR)/$(notdir $@) -MD $<


$(OUT_FILE_PREFIX)$(DBG_EXT)$(OS_EXT): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

clean:
	-$(RM) $(OBJ_DIR)/*.$(OBJ) $(OUT_FILE_PREFIX)$(OS_EXT) $(OBJ_DIR)/*.d
