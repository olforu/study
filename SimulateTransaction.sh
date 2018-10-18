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
total_transaction_fees=0
threshold_sell=1.06
threshold_buy=0.94
num_of_transaction=1
echo total position=$total_postion
echo market value=$market_value
echo total cost=$total_cost
#exit
let start_num++
function TransactionFees
{
	#$1 turnover 成交金额
	#$2 买入或者卖出
	commission_rate=0.0003
	transfer_fee_rate=0.00002
	stamp_duty_rate=0.001
	certificate_fee_rate=0.00002
	commission=`echo "$1*$commission_rate" | bc`
	if [ "`echo "$commission<5" | bc`" -eq 1 ];then
		commission=5
	fi
	transfer_fee=`echo "$1*$transfer_fee_rate" | bc`
	stamp_duty=`echo "$1*$stamp_duty_rate" | bc`
	certificate_fee=`echo "$1*$certificate_fee_rate" | bc`
	case $2 in
		buy) 
			echo "$commission+$transfer_fee" | bc 
			;;
		sell)	
			echo "$commission+$transfer_fee+$stamp_duty+$certificate_fee" | bc
			;;
		*) echo "wrong parameter!"	
	esac
}

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
	if [ "`echo "$threshold_sell*$final_price<=$highest_price" | bc`" -eq 1 ]; then 
		final_price=`echo "$final_price*$threshold_sell" | bc`
		position=`echo "($start_money/$final_price/100+1)*100" | bc`			
		echo final price $final_price
		echo sell $position
		if [ "`echo "$position>0" | bc`" -eq 1 ];then
			let num_of_transaction++
		fi
		if [ "`echo "($total_postion-$position)>0" | bc`" -eq 1 ]; then
			cost=`echo "$final_price*$position" | bc`
			transaction_fees=`TransactionFees  $cost sell`
		#	echo transaction_fee=$transaction_fees
		#	exit
 			total_cost=`echo "$total_cost-$final_price*$position-$transaction_fees" | bc`
			total_postion=`echo "$total_postion-$position" | bc`
			total_transaction_fees=`echo "$total_transaction_fees+$transaction_fees" | bc`
		else
			cost=`echo "$final_price*$total_postion" | bc`
			transaction_fees=`TransactionFees  $cost sell`
			echo transaction fee=$transaction_fees
			total_cost=`echo "$total_cost-$final_price*$total_postion-$transaction_fees" | bc`
			total_postion=0
			total_transaction_fees=`echo "$total_transaction_fees+$transaction_fees" | bc`
		fi
		#average_cost=`echo "$total_cost/$total_postion" | bc`
		market_value=`echo "$total_postion*$closing_price" | bc`
		echo "total cost=$total_cost"
		echo "market value=$market_value"
	#	echo total fee=$transaction_fees
	#	exit
	else
		if [ "`echo "$threshold_buy*$final_price>=$lowest_price" | bc`" -eq 1 ]; then 
			let num_of_transaction++
			final_price=`echo "$final_price*$threshold_buy" | bc`
			position=`echo "($start_money/$final_price/100+1)*100" | bc`			
			echo final price $final_price
			echo buy $position
			cost=`echo "$final_price*$position" | bc`
			transaction_fees=`TransactionFees  $cost buy`
			#TransactionFees  $cost buy
		#	echo transaction_fee=$transaction_fees
		#	exit
			total_cost=`echo "$total_cost+$final_price*$position-$transaction_fees" | bc`
			total_postion=`echo "$total_postion+$position" | bc`
			total_transaction_fees=`echo "$total_transaction_fees+$transaction_fees" | bc`
			#average_cost=`echo "$total_cost/$total_postion" | bc`
			market_value=`echo "$total_postion*$closing_price" | bc`
			echo "toal cost=$total_cost"
			echo "market value=$market_value"
			#exit
		else
			let start_num++
	#echo line $start_num
			continue
 		fi
	fi
	if [ "`echo "$total_cost>$max_cost" | bc`" -eq 1 ]; then
		max_cost=$total_cost
	fi
	let start_num++
done

echo max cost $max_cost
echo total tax=$total_transaction_fees
echo total times of transaction=$num_of_transaction
