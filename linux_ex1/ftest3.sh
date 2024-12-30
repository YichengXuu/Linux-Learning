#!/bin/sh
easy(){
	typeset a
	a=$(($1 + $2))
	b=$(($1 + $2))
	echo "Local easy a is $a"
	echo "Local easy b is $b"
}

a=1
b=2
easy $a $b
echo "global a is $a, global b is $b"
