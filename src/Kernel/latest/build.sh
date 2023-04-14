#!/bin/bash

echo "			---- BUILDING KERNEL ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Removing the old Kernel build
rm -r $SCRIPT_DIR/build/*

#Building the new Kernel
gcc -ffreestanding -c $SCRIPT_DIR/src/*.c -o $SCRIPT_DIR/build/kernel.o
ld -Ttext 0x1000 --oformat binary -o $SCRIPT_DIR/build/kernel.bin $SCRIPT_DIR/build/kernel.o
rm $SCRIPT_DIR/build/kernel.o	#Cleanup unnecessary file

RET_CODE=$?	# Get return code of linker command (linker will obviously fail if compiler failed anyways)

#Following color codes useful for printing
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ $RET_CODE == 0 ]	#All fine
then
	echo -e "				-> ${GREEN}BUILD SUCCESS${NC}			"
	exit 0
else
	echo -e "				 -> ${RED}BUILD FAILED${NC}			"
	exit 1
fi
