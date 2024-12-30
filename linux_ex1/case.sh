#!/bin/sh
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
