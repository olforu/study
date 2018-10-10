#!/bin/bash
start_time=$1
start_money=$2
start_num=`awk '/'$1'/{print NR}' $3` 
echo $start_num
