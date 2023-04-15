#!/bin/bash

echo "			---- BUILDING KERNEL ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Removing the old Kernel build
rm -r $SCRIPT_DIR/build/*

#	---- Building the new Kernel ----
#C
gcc -ffreestanding -fno-pie -m32 -c $SCRIPT_DIR/src/*.c -o $SCRIPT_DIR/build/kernel.o
	# -m32: Compile as 32 bit code. This can be quite complicated if you have a 64 bit compiler.
	# -fno-pie: Don't make position independent code (our kernel will be loaded at a precise address in memory that linker knows)

#ASM
nasm $SCRIPT_DIR/src/entry_point.asm -f elf -o $SCRIPT_DIR/build/entry_point.o

#Linking them together in order
ld -Ttext 0x1000 --oformat binary -m elf_i386 -o $SCRIPT_DIR/build/kernel.bin $SCRIPT_DIR/build/entry_point.o $SCRIPT_DIR/build/kernel.o
	#-m elf_i386: Link 32 bit files.

RET_CODE=$?	# Get return code of linker command (linker will obviously fail if compiler failed anyways)

#Cleanup unnecessary files that emerged from the process
#rm $SCRIPT_DIR/build/*.o

#	---- Printing the result ----
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
