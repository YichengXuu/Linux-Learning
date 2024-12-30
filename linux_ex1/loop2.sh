#!/bin/sh
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

