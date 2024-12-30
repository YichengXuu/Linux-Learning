#!/bin/sh
# Linux Experiment 1

# Shell only support single line comment
# My first shell script echo
echo 'Hello World!'

# the role of semicolon
echo 'This is line one';echo 'This is line two'

# Define Viariable and print
HELLO='Hello World'
echo 'Variable HELLO: $HELLO'

# Use single quatation and double quatation
echo 'This is single quoted: $HELLO'
echo "This is double quoted: $HELLO"

# Variable truncation
echo "First 5 characters: ${HELLO:0:5}"
echo "Last part: ${HELLO:6}"


# Special Viarable
echo "Script name: $0"
echo "Number of arguments: $#"
echo "All arguments: $@"
echo "PID of script: $$"

# Default value of Variable
echo "This is default color: ${COLOR:-blue}"
echo "Initialized color is ${COLOR:=green}"
echo "Updated collor is $COLOR"

# Absolute path
SRC_FILE="/tmp/message1.txt"
DST_FILE="/tmp/message2.txt"
echo "This is a test message">$SRC_FILE
cp $SRC_FILE $DST_FILE
echo "File copied from $SRC_FILE to $DST_FILE"

# Only Read Variable
MAXVAL=65535
readonly MAXVAL
echo "MAXVAL: $MAXVAL"
# MAXVAL=32767 # This line will error

# Environment Variable
export MY_VAR="This is an environment variable"
echo "Environment variable MY_VAR: $MY_VAR"

# I/O
echo "Enter your name and age:"
read name age
echo "Your name is $name, and you are $age years old"

# Conditional Test: File Status
touch /tmp/tmp.txt
[ -w /tmp/tmp.txt ] && echo "/tmp/tmp.txt is writable"
rm /tmp/tmp.txt

# Conditional Test: String
STRING1="hello"
STRING2="world"
[ $STRING1 !=  $STRING2 ] && echo "Strings are not equal"

# Conditional Test: Num Comparation
NUM=10
[ $NUM -eq 10 ] && "NUM is equal to 10"

# LOOP
# for
for i in 1 2 3; do
	echo "For loop iteration: $i"
done

# Command substitution
echo "Listing current directory contents using command substitution:"
files=$(ls)
echo "Files: $files"

echo "Current date and time:"
current_time=$(date)
echo "Now: $current_time"

# while loop
count=1
while [ $count -le 3 ];do
	echo "While loop count: $count"
	count=$((count + 1))
done

# until loop
until [ $count -eq 6 ];do
	echo "Until loop count: $count"
	count=$((count + 1))
done

# break and continue
for i in 1 2 3 4 5;do
	if [ $i -eq 3 ];then
		echo "Breaking at $i"
		break
	fi
	echo "Value before break: $i"
done

for i in 1 2 3 4 5;do
	if [ $i -lt 3 ];then
		echo "Skipping $i"
		continue
	fi
	echo "Value after continue: $i"
done

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

# case
echo "DO you want to continue?(yes/no)"
read response
case $response in
	yes|y|Y)
		echo "You chose to continue."
		;;
	no|n|N) 
		echo "You chose to exit."
		;;
	*) 
		echo "Invalid choice." exit
		1
		;;
esac

# Arithmetic Calculation
result=$((4 + 5))
echo "4 + 5 = $result"

# Self-difined Function
message(){
	echo "This is a message"
}

for ((i=0;i<3;i++));do
	message
done

echo "--------------"
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

echo "-------------"

easy(){
	typeset a
	a=$(($1 + $2))
	b=$(($1 + $2))
	echo "Local easy a is $a"
	echo "Local easy b is $b"
}

# Local Viriable function
a=1
b=2
easy $a $b
echo "Global a is $a"
echo "Global b is $b"

# File reference
. ./sum.sh
sum 1 2 echo 
$?

# clear temporary file
rm -f $SRC_FILE $DST_FILE






























