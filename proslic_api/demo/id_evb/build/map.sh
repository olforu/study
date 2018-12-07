#!/bin/bash
# Copyright 2016, Silicon Labs
# Purpose: determine from EEPROM strings on the Silicon Labs 
# ProSLIC/VDAA EVB boards what parameter should be used
# for the API demo Makefile.
# This would work on a VMB1 or VMB2 connected via VCP interface.

echo "Reading EEPROM string - this will take a few seconds"
evb_string=`./id_evb`
echo "Raw read: " $evb_string
converter=`echo $evb_string |cut -d: -f2`
rev=`echo $evb_string |cut -d: -f5`
rev_ver='_'$rev'_'
#echo "Looking for $converter $rev_ver"
#grep_output=`grep $converter map.txt|grep $rev_ver|cut -d: -f2`
grep_output=`grep -i $converter map.txt|cut -d: -f2`
#echo "MAKEFILE setting = $grep_output"
echo "Rev = $rev"
echo $grep_output
