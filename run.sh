#!/bin/bash

#Color codes useful for printing
RED='\033[0;31m'
NC='\033[0m'

echo "			---- RUNNING OS ----			"

BUILT_FILE=$(ls ./build/)	#Get name of the built file independently
echo "Using file: $BUILT_FILE"

# For compatibility reasons with my home setup.
# Add whatever qemu command variation you have to this if statement so that it picks it
if command -v qemu-virgil	#If qemu-virgil works (snap store)
then
	qemu-virgil -drive file=./build/$BUILT_FILE,format=raw	#Run it
else
	echo -e "${RED}No x86 emulator found. Add yours to this script's list if necessary${NC}"
fi

exit 0