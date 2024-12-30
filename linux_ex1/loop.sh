#!/bin/sh
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

