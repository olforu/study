#!/bin/bash
#LTE data throughput analysis script V01 by Duke Zeng, LTE SW part of Skysoft.
#usage: LTE_Statistics_Script.sh [ASR log file] [Output file]
if [ -z "$1" ]; then
	echo "usage: LTE_Statistics_Script.sh [ASR log file] [Output file]"
	exit
fi

echo "Analysing ..."

RSRP0=0
RSRP1=0
RSRQ0=0
RSRQ1=0
SNR0=0
SNR1=0
DL_MCS=0
DL_BLER=0
CQI=0
UL_MCS=0
UL_BLER=0
Num=0
Num=`grep "ulbler" -i "$1" | awk 'END{print NR}'`
#echo Num=$Num
if [ "$Num" -eq 0 ]; then
	echo "Wrong input file!"
	exit
fi
#handle RSRP0
for i in `grep "rsrpA0dBm" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/rsrpA0dBm/){print $n}}}' | awk -F" " '{print $2}' `
do
	RSRP0=`echo "$RSRP0+$i" | bc`	
done
RSRP0=`echo "scale=1;$RSRP0/$Num-140" | bc`
#RSRP0=`echo "-$RSRP0"`


#handle RSRP1
for i in `grep "rsrpA1dBm" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/rsrpA1dBm/){print $n}}}' | awk -F" " '{print $2}' `
do
	RSRP1=`echo "$RSRP1+$i" | bc`	
done
RSRP1=`echo "scale=1;$RSRP1/$Num-140" | bc`
#RSRP1=`echo "-$RSRP1"`


#handle SNR 
for i in `grep "scSNR" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/scSNR/){print $n}}}' | grep -o "[0-9][0-9]*\.[0-9]*"`
#grep "scSNR" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/scSNR/){print $n}}}'
do
#echo i=$i
	SNR0=`echo "$SNR0+$i" | bc`	
done
SNR0=`echo "scale=1;$SNR0/$Num" | bc`
SNR1=$SNR0

#handle DL MCS
for i in `grep "dlmcs" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/averDlMcs/){print $n}}}' | awk -F" " '{print $2}'`
do
#echo i=$i
	DL_MCS=`echo "$DL_MCS+$i" | bc`	
done
DL_MCS=`echo "scale=1;$DL_MCS/$Num/2" | bc`


#handle DL Bler
for i in `grep "dlbler" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/dlBler/){print $n}}}' | sed 's/.*%\([0-9]*\)(.*/\1/'`
do
#echo i=$i
	DL_BLER=`echo "$DL_BLER+$i" | bc`	
done
DL_BLER=`echo "scale=1;$DL_BLER/$Num" | bc`
DL_BLER=`echo "$DL_BLER%"`




#handle CQI
for i in `grep "cqi" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/averCQI/){print $n}}}' | awk -F" " '{print $2}'`
do
#echo i=$i
	CQI=`echo "$CQI+$i" | bc`	
done
CQI=`echo "scale=1;$CQI/$Num/2" | bc`



#handle UL Mcs
for i in `grep "ulmcs" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/averUlMcs/){print $n}}}' | awk -F" " '{print $2}'`
do
#echo i=$i
	UL_MCS=`echo "$UL_MCS+$i" | bc`	
done
UL_MCS=`echo "scale=1;$UL_MCS/$Num" | bc`

#handle UL Bler
for i in `grep "ulbler" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/ulBler/){print $n}}}' | sed 's/.*%\([0-9]*\).*/\1/g'`
do
	UL_BLER=`echo "$UL_BLER+$i" | bc`	
done
UL_BLER=`echo "scale=1;$UL_BLER/$Num" | bc`
UL_BLER=`echo "$UL_BLER%"`



#RSRQ is special in the log,can not get RSRQ0 and RSRQ1 seperately
Num=`grep "servingcell" -i  "$1" | awk 'END{print NR}'`
for i in `grep "servingcell" -i  "$1" | awk -F"," '{for(n=1;n<NF;n++){if($n~/rsrq/){print $n}}}' | awk -F" " '{print $3}' `
do
	RSRQ0=`echo "$RSRQ0+$i" | bc`	
done
RSRQ0=`echo "scale=1;$RSRQ0/$Num" | bc`
RSRQ1=$RSRQ0
if [ -z "$2" ];then
	printf "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n" RSRP0	RSRP1 RSRQ0	RSRQ1 SNR0 SNR1	DL_MCS DL_BLER CQI UL_MCS UL_BLER
	printf "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n" $RSRP0 $RSRP1  $RSRQ0  $RSRQ1  $SNR0   $SNR1   $DL_MCS  $DL_BLER  $CQI  $UL_MCS  $UL_BLER
else
	printf "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n" RSRP0	RSRP1 RSRQ0	RSRQ1 SNR0 SNR1	DL_MCS DL_BLER CQI UL_MCS UL_BLER > "$2"
	printf "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n" $RSRP0 $RSRP1  $RSRQ0  $RSRQ1  $SNR0   $SNR1   $DL_MCS  $DL_BLER  $CQI  $UL_MCS  $UL_BLER >> "$2"

	echo "Done"
fi

