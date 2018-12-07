#!/bin/bash 
PWD="$1"
if [ "$PWD" == "" ];then
    echo "arg1 is the root path of compiled project"
    exit -1
fi
TOOL_DIR="apps_proc/oe-core/build/tmp-glibc/sysroots"
TOOLCHAIN="$TOOL_DIR/x86_64-linux/usr/libexec/arm-oe-linux-gnueabi/gcc/arm-oe-linux-gnueabi/4.9.2/:$TOOL_DIR/x86_64-linux/usr/bin/arm-oe-linux-gnueabi"
X86_TOOL="$TOOL_DIR/x86_64-linux/usr/bin/:$TOOL_DIR/x86_64-linux/usr/sbin"
SYSROOT="$TOOL_DIR/mdm9607"
[ "`echo "$PATH" | grep "$TOOL_DIR"`" = "" ] && PATH=$TOOL_DIR:$PATH
[ "`echo "$PATH" | grep "$TOOLCHAIN"`" = "" ] && PATH=$TOOLCHAIN:$PATH
[ "`echo "$PATH" | grep "$X86_TOOL"`" = "" ] && PATH=$PATH:$X86_TOOL
#PATH="$TOOLCHAIN:$X86_TOOL:$TOOL_DIR:$PATH"

CROSS_COMPILER=arm-oe-linux-gnueabi-
CC="${CROSS_COMPILER}gcc -march=armv7-a -mfloat-abi=softfp -mfpu=neon --sysroot=$SYSROOT"
CPP="${CROSS_COMPILER}g++ -march=armv7-a -mfloat-abi=softfp -mfpu=neon --sysroot=$SYSROOT"
AS=${CROSS_COMPILER}as
LD=${CROSS_COMPILER}ld
armv7=${CROSS_COMPILER}ar
NM=${CROSS_COMPILER}nm
STRIP=${CROSS_COMPILER}strip
OBJCOPY=${CROSS_COMPILER}objcopy
OBJDUMP=${CROSS_COMPILER}objdump

export PATH OUTPATH SRCPATH IMAGE_OUT IMAGEPATH IMAGEOUT CC CPP AS LD NM STRIP OBJCOPY OHJDUMP CUSTEMN CROSS_COMPILER SUBBUILDSYS
