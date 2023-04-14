#!/bin/bash

echo "			---- BUILDING BOOTSECTOR ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Removing old Bootsector build
rm -r $SCRIPT_DIR/build/*

#Building the Bootsector
nasm -f bin $SCRIPT_DIR/src/main.asm -o $SCRIPT_DIR/build/bootsector.bin -i $SCRIPT_DIR

RET_CODE=$?	# Get return code of compile command

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
