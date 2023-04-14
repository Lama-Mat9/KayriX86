#!/bin/bash

echo "		---- BUILDING KayriX86 ----			"

#Removing old full build
rm -r ./build/*

#Building the latest versions of necessary components
STATUS=0	#We add all the error codes to it. Should be 0 at the end if everything is fine

./src/Bootsector/latest/build.sh
let "STATUS+=$?"			#Add return code to counter

./src/Kernel/latest/build.sh
let "STATUS+=$?"			#Add return code to counter

if [ $STATUS == 0 ]
then
	#Using these components to build a flashable image
	cat ./src/Bootsector/latest/build/bootsector.bin ./src/Kernel/latest/build/kernel.bin > ./build/KayriX86.iso
	exit $?		#Our return code is the last command's
else
	echo "Encountered $STATUS build errors"
fi
