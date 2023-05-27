#!/bin/bash

echo "			---- BUILDING KERNEL ----			"

#Get path of current script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#Following color codes useful for printing
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

#You need to build and use a cross compiler (so that it has no access to your current OS's standard libraries).
#Using your current OS's build of these could work, but is unsafe. (At least use -m32 if you need to use a 64bit compiler).
C_COMPILER="i686-elf-gcc"
LINKER="i686-elf-ld"

#Removing the old Kernel build
rm -r $SCRIPT_DIR/build/*

RET_CODE=0

#ASM
nasm $SCRIPT_DIR/src/entry_point.asm -f elf -o $SCRIPT_DIR/build/entry_point.o
nasm $SCRIPT_DIR/src/asm/*.asm -f elf -o $SCRIPT_DIR/build/asm_objects.o
RET_CODE=$(($RET_CODE + $?))

#	---- Building the new Kernel ----
$C_COMPILER -ffreestanding -fno-pie -Ttext 0x1000 -Wl,--oformat=binary -I $SCRIPT_DIR/src/ $SCRIPT_DIR/build/entry_point.o $SCRIPT_DIR/build/asm_objects.o $(find $SCRIPT_DIR/src/ -type f -iname *.c -print) -o $SCRIPT_DIR/build/Kernel.bin  -Wall -Wextra -nostdlib -lgcc 
	# -freestanding: We are in a "freestanding environment". No libraries available or anything.
	# -fno-pie: Don't make position independent code (our kernel will be loaded at a precise address in memory)
	# -Ttext: Pass the address of where our code will be loaded in memory. The bootloader decides where that is.
	# -Wl,--oformat=binary: -Wl allows to pass options to the linker (probably ld). We ask ld to not use elf format.
	# -I: Defines the root of where the #includes directives should be relative to.
	# nasm's .o files: We gotta gcc as linker because he is the only one who knows where it's libgcc is. Therefore GCC links nasm's files as well as compiling.
	# $(find [...] ): Recursively finds all .c files in our src directory
	# -Wall -Wextra: Give as much warnings as possible. Could help debug.
	# -nostdlib: Disable automatic linking with libc and libgcc. We don't use libc because gcc's libc belongs to the OS we're running. However we want libgcc.
	# -lgcc: Enable linking with gcc's libgcc, which is a major part of gcc. I have noticed for example that gcc requires it for 64 bit divisions (not 32 though).
	#	-> -lgcc has to be put AFTER all the files that might use it in our compile command.
	
RET_CODE=$(($RET_CODE + $?))

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
