#
# Copyright 2015-2016, Silicon Labs
#
# $Id: Makefile 6086 2016-10-27 21:31:17Z nizajerk $
#
# User will need to set ARCH, CROSS_COMPILE, KDIR, & PROSLIC_API_DIR (ABS PATH)
# In addition, PROSLIC_CHIPSET & PROSLIC_CONVERTER needs to be set for each chipset used.
#
# Tested with ProSLIC API 8.1.2
#


#
# Since we can't really use vpath to define paths to search code,
# prefix the _SRC listing to include the actual path of the code.
# This assumes symbolic links are created to src, patch_files, config_src.
#
# The alternative is to copy EVERYTHING into 1 directory, which
# would make it later to compare the Silicon Labs release code to
# what is being used on the current platform.
#

TEST_DIR = $(PROSLIC_API_DIR)/demo/pform_test

TEST_SRC = pform_test_module.c spi_tests.c  timer_tests.c spi_tests_common.c 

ifdef DAA
TEST_SRC += spi_tests_daa.c
TEST_MODE = -DDAA_TESTS=1
else
TEST_SRC += spi_tests.c 
TEST_MODE = -DPROSLIC_TESTS=1
endif

SRC :=  $(addprefix src/, $(TEST_SRC)) \
				platform/common/src/platform.c \
				api_src/si_voice.c \
				api_src/si_voice_version.c


PROSLIC_API_CUSTOM := $(PROSLIC_API_DIR)/demo/pform_test/custom

PROSLIC_INCDIRS := -I$(TEST_DIR)/inc -I$(PROSLIC_API_CUSTOM) \
	-I$(PROSLIC_API_DIR)/../proslic_sys \
	-I$(PROSLIC_API_DIR)/inc \
	-I$(PROSLIC_API_DIR)/demo/platform/common/inc \
	-I$(PROSLIC_API_DIR)/demo/linux_kernel/proslic_sys

EXTRA_CFLAGS := $(PROSLIC_CFLAGS) $(PROSLIC_INCDIRS) -DPROSLIC_LINUX_KERNEL=1 $(TEST_MODE)

obj-m += proslic_pform_test.o
proslic_pform_test-objs:= $(SRC:.c=.o)


all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

