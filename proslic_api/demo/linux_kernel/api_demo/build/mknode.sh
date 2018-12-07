#!/bin/bash
# Copyright 2015, Silcion Labs
# This is a simple shell script to create a set if inode entries in /dev
# 

slic_devid=`grep proslic /proc/devices |cut -d' ' -f1 -`
num_devices=0 #0 to N

for i in `seq 0 $num_devices`;
do
  echo "Creating device $i"
  mknod /dev/proslic$i c $slic_devid $i
done
