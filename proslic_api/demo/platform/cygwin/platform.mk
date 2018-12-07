########################################################################
#
# Makefile for Cygwin specific code.
# Copyright 2014-2018, Silicon Labs
#
# $Id: platform.mk 7065 2018-04-12 20:24:50Z nizajerk $
#
########################################################################

ifdef VMB1
SPI_FLAGS = -DVMB1
SPI_IF_LIB = -L$(PROSLIC_PFORM_ROOT)/$(PROSLIC_OS)/bin -lquad_io
PROSLIC_PFORM += proslic_spiGci_usb.c
endif

ifdef VMB2
SPI_FLAGS = -DVMB2
SPI_IF_LIB = -L$(PROSLIC_PFORM_ROOT)/$(PROSLIC_OS)/bin -lvmb2_dll
PROSLIC_PFORM += spi_pcm_vcp.c
endif

ifdef RSPI
SPI_FLAGS = -DRSPI
PROSLIC_PFORM += rspi_client.c rspi_common.c
endif

ifdef RSPI_TXT_CFG
SPI_FLAGS += -DRSPI_TXT_CFG
endif

ifdef RSPI_NO_STD_OUT
SPI_FLAGS += -DRSPI_NO_STD_OUT
endif

ifdef LUA
LFLAGS += -llua
endif

PROSLIC_PFORM += proslic_timer.c

PROSLIC_PFORM_DIRS += $(PROSLIC_PFORM_ROOT)/$(PROSLIC_OS)/spi
PROSLIC_PFORM_DIRS += $(PROSLIC_PFORM_ROOT)/posix/timer
PROSLIC_CFLAGS     += -I$(PROSLIC_PFORM_ROOT)/posix/timer -I$(PROSLIC_PFORM_ROOT)/$(PROSLIC_OS)/spi
PROSLIC_CFLAGS     += -I$(PROSLIC_PFORM_ROOT)/common/inc

OS_EXT=.exe
PROSLIC_CFLAGS += -DPOSIX=1
