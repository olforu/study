########################################################################
#
# Makefile for Linux specific code.
# Copyright 2014-2018, Silicon Labs
#
# $Id: platform.mk 7065 2018-04-12 20:24:50Z nizajerk $
#
########################################################################
ifdef VMB1
$(error "VMB1 is not supported under Linux - sorry")
endif

ifdef VMB2
SPI_FLAGS = -DVMB2
PROSLIC_PFORM += proslic_vmb2_posix.c proslic_vmb2.c
endif

ifdef SPIDEV
SPI_FLAGS = -DLINUX_SPIDEV
PROSLIC_PFORM += proslic_spidev.c
PROSLIC_PFORM_DIRS += $(PROSLIC_PFORM_ROOT)/linux/spi
PROSLIC_CFLAGS     += -I$(PROSLIC_PFORM_ROOT)/linux/spi
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

# This may differ from Linux distribution to distribution...
ifdef LUA
PROSLIC_CFLAGS += -I/usr/include/lua5.2
LFLAGS += -llua5.2
endif

PROSLIC_PFORM += proslic_timer.c

#
# Linux desktop VMB2 drivers use the POSIX compatible driver,
# assuming there's a device node...
#
PROSLIC_PFORM_DIRS += $(PROSLIC_PFORM_ROOT)/posix/timer
PROSLIC_PFORM_DIRS += $(PROSLIC_PFORM_ROOT)/posix/spi
PROSLIC_CFLAGS     += -I$(PROSLIC_PFORM_ROOT)/common/inc -I$(PROSLIC_PFORM_ROOT)/posix/spi
PROSLIC_CFLAGS     += -I$(PROSLIC_PFORM_ROOT)/posix/timer

PROSLIC_CFLAGS += -DPOSIX=1
