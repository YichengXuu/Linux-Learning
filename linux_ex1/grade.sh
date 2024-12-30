#!/bin/sh
# if-then-else
echo "Enter your grade:"
read grade
if [ $grade -lt 60 ];then
	echo "Fail"
elif [ $grade -lt 80 ];then
	echo "Pass"
else
	echo "Excellent"
fi
