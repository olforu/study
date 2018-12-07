########################################################################
#
# Generic Makefile for ProSLIC_API Demos, platform specific files.
# Copyright 2014, Silicon Labs
#
# OS/platforms supported: Cygwin (32 bit tested) & Linux (Ubuntu 12.04 tested - 64 bit)
#
# $Id: platform_common.mk 7071 2018-04-16 21:41:02Z nizajerk $
#
########################################################################

#
# First determine which OS is used...
#
ifndef ($(PROSLIC_OS))

	UNAME_S := $(shell uname -s)
	UNAME_O := $(shell uname -o)

	ifeq ($(UNAME_S),Linux)
		PROSLIC_OS ?=linux
	endif

	ifeq ($(UNAME_O),Cygwin)
		PROSLIC_OS ?=cygwin
	endif

  #Mingw environment...
	ifeq ($(UNAME_O),Msys)
		PROSLIC_OS ?=mingw
	endif
    
	ifeq ($(PROSLIC_OS),)
		PROSLIC_OS= unknown
	endif
endif # OS determination

PROSLIC_PFORM_ROOT ?= $(PROSLIC_API_DIR)/demo/platform
PROSLIC_PFORM_DIR  ?= $(PROSLIC_PFORM_ROOT)/$(PROSLIC_OS)
PROSLIC_PFORM_DIRS = $(PROSLIC_PFORM_DIR) $(PROSLIC_TIMER_DIR) $(PROSLIC_PFORM_ROOT)/common/src \
  $(PROSLIC_PFORM_DIR)/posix/ui/basic_text

include $(PROSLIC_PFORM_DIR)/platform.mk

ifndef PROSLIC_DEMO_NO_SPI
PROSLIC_PFORM += platform.c
endif

