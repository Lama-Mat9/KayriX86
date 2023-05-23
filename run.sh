#!/bin/bash

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Color codes useful for printing
RED='\033[0;31m'
NC='\033[0m'

echo "			---- RUNNING OS ----			"

BUILT_FILE=$(ls $SCRIPT_DIR/build/)	#Get name of the built file independently
echo "Using file: $BUILT_FILE"

# For compatibility reasons with my home setup.
# Add whatever qemu command variation you have to this if statement so that it picks it
if command -v qemu-virgil	#If qemu-virgil works (snap store)
then

	#Separating script prints with the OS's serial console prints by printing a string
	for ((i=0;i<$(tput cols);i++))	#For each rows
	do
		echo -n "="	#Print "=" per column in one row, no newline
	done

	qemu-virgil -serial stdio -drive file=$SCRIPT_DIR//build/$BUILT_FILE,format=raw	#Run it. 
			#Serial console mapped to stdio.
else
	echo -e "${RED}No x86 emulator found. Add yours to this script's list if necessary${NC}"
fi

exit 0
