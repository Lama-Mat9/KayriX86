#!/bin/bash

echo "			---- BUILDING KERNEL ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Following color codes useful for printing
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

#You need to build and use a cross compiler (so that it has no access to your current OS's standard libraries).
#Using your current OS's build of these could work, but is unsafe.
C_COMPILER="i686-elf-gcc"
LINKER="i686-elf-ld"

#Removing the old Kernel build
rm -r $SCRIPT_DIR/build/*

RET_CODE=0

#Go into specific dir just to make sure that output gets there (GCC doesn't wanna let me use -o)
mkdir $SCRIPT_DIR/build/c_objects/
cd $SCRIPT_DIR/build/c_objects/

#	---- Building the new Kernel ----
$C_COMPILER -ffreestanding -fno-pie -m32 -c $(find $SCRIPT_DIR/src/ -type f -iname *.c -print) -Wall -Wextra
	# -m32: Compile as 32 bit code. This can be quite complicated if you have a 64 bit compiler.
	# -fno-pie: Don't make position independent code (our kernel will be loaded at a precise address in memory that linker knows)
	# $(find [...] ): Recursively finds all .c files in our src directory
	#-Wall -Wextra: Give as much warnings as possible. Could help debug.
RET_CODE=$(($RET_CODE + $?))

#ASM
nasm $SCRIPT_DIR/src/entry_point.asm -f elf -o $SCRIPT_DIR/build/entry_point.o
RET_CODE=$(($RET_CODE + $?))

#Linking them together in order (entry point has to be first)
$LINKER -Ttext 0x1000 --oformat binary -m elf_i386 -o $SCRIPT_DIR/build/kernel.bin $SCRIPT_DIR/build/entry_point.o $SCRIPT_DIR/build/c_objects/*.o
	#-m elf_i386: Link 32 bit files.

RET_CODE=$(($RET_CODE + $?))	# Get return code of linker command (linker will obviously fail if compiler failed anyways)

#Cleanup unnecessary files that emerged from the process
rm $SCRIPT_DIR/build/*.o
rm -r $SCRIPT_DIR/build/c_objects/

#	---- Printing the result ----

if [ $RET_CODE == 0 ]	#All fine
then
	echo -e "				-> ${GREEN}BUILD SUCCESS${NC}			"
	exit 0
else
	echo -e "				 -> ${RED}BUILD FAILED${NC}			"
	exit 1
fi
