#
# $Id: Makefile 5353 2015-11-13 20:05:22Z nizajerk $
#
# Customers will need to set ARCH, CROSS_COMPILE & KDIR to build this module.
#
# KDIR = kernel directory, if native compille, /usr/src/linux should be used.
# CROSS_COMPILE is the prefix for the GCC compiler, leave unset for native compiles.
# ARCH - please see your kernel setting for this.

ARCH ?=arm
CROSS_COMPILE ?=armv7-linux-
obj-m += proslic_sys.o
proslic_sys-objs:= proslic_sys_main.o proslic_sys_spi.o proslic_sys_timer.o 

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

