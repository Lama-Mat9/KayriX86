#!/bin/bash

BOOTSECTOR_PATH=./src/Bootsector/latest/
KERNEL_PATH=./src/Kernel/latest/
OUTPUT_FILE_SIZE=1440000	#1.44mb image file
OUTPUT_FILE_NAME=KayriX86.iso

echo "		---- BUILDING $OUTPUT_FILE_NAME ----			"

#Removing old full build
rm -r ./build/*

#Building the latest versions of necessary components
STATUS=0	#We add all the error codes to it. Should be 0 at the end if everything is fine

$BOOTSECTOR_PATH/build.sh
let "STATUS+=$?"			#Add return code to counter

$KERNEL_PATH/build.sh
let "STATUS+=$?"			#Add return code to counter

#Following color codes useful for printing
NC='\033[0m'
ORANGE='\033[0;33m'
GREEN='\033[0;32m'

if [ $STATUS == 0 ]
then
	echo "			---- CREATING OUTPUT FILE ----			"

	#Using these components to build a flashable image
	#Whatever bin is in the build dir gets added to the ISO in this order
	find $BOOTSECTOR_PATH/build/ -name '*.bin' -exec cat {} \; >> ./build/$OUTPUT_FILE_NAME
	find $KERNEL_PATH/build/ -name '*.bin' -exec cat {} \; >> ./build/$OUTPUT_FILE_NAME
	OUTSIZE=$(wc -c < ./build/$OUTPUT_FILE_NAME)	# See how many bytes the OS is currently
	
	if [ $OUTSIZE -gt $OUTPUT_FILE_SIZE ]	#If the result is bigger than the size we're asked to generate
	then	#Don't forget to use gt instead of > as it would be interpreted as a redirection
		echo -e "${ORANGE}WARN:${NC} Resulting file size is bigger than given file size parameter."
		echo "You should modify the output file size in the build script to avoid errors."
	else
		SIZE_DELTA=$(($OUTPUT_FILE_SIZE-$OUTSIZE))		#Calculate the number of empty bytes that we should add
		head -c $SIZE_DELTA /dev/zero >> ./build/$OUTPUT_FILE_NAME	#Add empty bytes until we reach the desired file size
	fi
	
	#Checking if the result seems ok
	if [ -f ./build/$OUTPUT_FILE_NAME ] && [ $(wc -c < ./build/$OUTPUT_FILE_NAME) == $OUTPUT_FILE_SIZE ]
	then
		echo -e "				-> ${GREEN}READY${NC}			"
	else
		echo -e "				-> ${ORANGE}OUTPUT NOT OK${NC}			"
	fi

	exit 0
else
	echo "Encountered $STATUS build errors"
	exit $STATUS
fi
