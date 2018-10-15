#!/bin/bash
start_time=$1
start_money=$2
start_num=`grep -n -w "$1" $3 | cut -d: -f1` 
#echo $start_num
if [ "$start_num" = "" ]; then
	echo wrong date!
	exit
fi
end_num=`awk 'END {print NR}' $3` 
#echo $end_num
final_price=`awk -F' ' 'NR=='${start_num}' {print $4}' $3`
echo $start_money
echo $final_price
position=`echo "($start_money/$final_price/100+1)*100" | bc`
market_value=`echo "$position*$final_price" | bc`
cost=`echo "$position*$final_price" | bc`
total_cost=$cost
total_postion=$position
max_cost=$cost
#echo $position
#echo $market_value
#echo $cost
#exit
let start_num++

while [ $start_num -le $end_num ]
do
	#echo line=$start_num
	date=`awk -F' ' 'NR=='${start_num}' {print $1}' $3`
	closing_price=`awk -F' ' 'NR=='${start_num}' {print $4}' $3`
	highest_price=`awk -F' ' 'NR=='${start_num}' {print $5}' $3`
	lowest_price=`awk -F' ' 'NR=='${start_num}' {print $6}' $3`
	opening_price=`awk -F' ' 'NR=='${start_num}' {print $7}' $3`
	closing_price_previous=`awk -F' ' 'NR=='${start_num}' {print $8}' $3`
	loss=`awk -F' ' 'NR=='${start_num}' {print $9}' $3`
	decline=`awk -F' ' 'NR=='${start_num}' {print $10}' $3`
	if [ "`echo "$closing_price == 0" | bc`" -eq 1 ]; then
		let start_num++ 
		continue
	fi
	#echo $date $closing_price $highest_price $lowest_price $opening_price $closing_price_previous $loss $decline 
	if [ "`echo "1.05*$final_price<=$highest_price" | bc`" -eq 1 ]; then 
		final_price=`echo "$final_price*1.05" | bc`
		position=`echo "($start_money/$final_price/100+1)*100" | bc`			
		echo final price $final_price
		echo sell $position
		if [ "`echo "($total_postion-$position)>0" | bc`" -eq 1 ]; then
			total_cost=`echo "$total_cost-$final_price*$position" | bc`
			total_postion=`echo "$total_postion-$position" | bc`
		else
			total_cost=`echo "$total_cost-$final_price*$total_postion" | bc`
			total_postion=0
		fi
		#average_cost=`echo "$total_cost/$total_postion" | bc`
		market_value=`echo "$total_postion*$closing_price" | bc`
		echo "toal cost=$total_cost"
		echo "market value=$market_value"
	#	exit
	else
		if [ "`echo "0.95*$final_price>=$lowest_price" | bc`" -eq 1 ]; then 
			final_price=`echo "$final_price*0.95" | bc`
			position=`echo "($start_money/$final_price/100+1)*100" | bc`			
			echo final price $final_price
			echo buy $position
			total_cost=`echo "$total_cost+$final_price*$position" | bc`
			total_postion=`echo "$total_postion+$position" | bc`
			#average_cost=`echo "$total_cost/$total_postion" | bc`
			market_value=`echo "$total_postion*$closing_price" | bc`
			echo "toal cost=$total_cost"
			echo "market value=$market_value"
			#exit
			echo here
		else
			let start_num++
			continue
 		fi
	fi
	if [ "`echo "$total_cost>$max_cost" | bc`" -eq 1 ]; then
		max_cost=$total_cost
	fi
	let start_num++
done
echo max cost $max_cost
