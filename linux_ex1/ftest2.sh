#!/bin/sh
sum(){
	x=$1
	y=$2
	result=$((x+y))
	return $result
}

echo "Enter two numbers:"
read num1 num2
sum $num1 $num2
echo "The sum is $?"
