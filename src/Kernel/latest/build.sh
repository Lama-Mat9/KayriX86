#!/bin/bash

echo "			---- BUILDING KERNEL ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Get the current device's architecture to tweak compiler
BUILDER_ARCH=$(dpkg --print-architecture)

#Following color codes useful for printing
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

#Architecture dependent compiler tweaks possible here
if [ $BUILDER_ARCH == "aarch64" ]
then
	#C - aarch64 running termux (gcc -> clang compatibility mode)
	C_COMPILER="clang -target i386"
	LINKER="ld.lld"
	#We specify to clang that we want it to make 32 bit code. Not arm.

elif [ $BUILDER_ARCH == "amd64" ]
then
	C_COMPILER="gcc"
	LINKER="ld"
else
	echo -e "${RED}Unknown builder architecture. Add your compiler to this script${NC}"
	exit 1
fi

#Removing the old Kernel build
rm -r $SCRIPT_DIR/build/*

#	---- Building the new Kernel ----
$C_COMPILER -ffreestanding -fno-pie -m32 -c $SCRIPT_DIR/src/*.c -o $SCRIPT_DIR/build/kernel.o
	# -m32: Compile as 32 bit code. This can be quite complicated if you have a 64 bit compiler.
	# -fno-pie: Don't make position independent code (our kernel will be loaded at a precise address in memory that linker knows)

#ASM
nasm $SCRIPT_DIR/src/entry_point.asm -f elf -o $SCRIPT_DIR/build/entry_point.o

#Linking them together in order
$LINKER -Ttext 0x1000 --oformat binary -m elf_i386 -o $SCRIPT_DIR/build/kernel.bin $SCRIPT_DIR/build/entry_point.o $SCRIPT_DIR/build/kernel.o
	#-m elf_i386: Link 32 bit files.

RET_CODE=$?	# Get return code of linker command (linker will obviously fail if compiler failed anyways)

#Cleanup unnecessary files that emerged from the process
rm $SCRIPT_DIR/build/*.o

#	---- Printing the result ----

if [ $RET_CODE == 0 ]	#All fine
then
	echo -e "				-> ${GREEN}BUILD SUCCESS${NC}			"
	exit 0
else
	echo -e "				 -> ${RED}BUILD FAILED${NC}			"
	exit 1
fi
