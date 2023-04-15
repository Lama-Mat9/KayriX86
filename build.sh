#!/bin/bash

BOOTSECTOR_PATH=./src/Bootsector/latest/
KERNEL_PATH=./src/Kernel/latest/
OUTPUT_FILE=$1		#Get OS file name from script parameters

echo "		---- BUILDING $OUTPUT_FILE ----			"

#Removing old full build
rm -r ./build/*

#Building the latest versions of necessary components
STATUS=0	#We add all the error codes to it. Should be 0 at the end if everything is fine

$BOOTSECTOR_PATH/build.sh
let "STATUS+=$?"			#Add return code to counter

$KERNEL_PATH/build.sh
let "STATUS+=$?"			#Add return code to counter

if [ $STATUS == 0 ]
then
	#Using these components to build a flashable image
	#Whatever bin is in the build dir gets added to the ISO in this order
	find $BOOTSECTOR_PATH/build/ -name '*.bin' -exec cat {} \; >> ./build/$OUTPUT_FILE
	find $KERNEL_PATH/build/ -name '*.bin' -exec cat {} \; >> ./build/$OUTPUT_FILE

	exit $?		#Our return code is the last command's
else
	echo "Encountered $STATUS build errors"
	exit $STATUS
fi
