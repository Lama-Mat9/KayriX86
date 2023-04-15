#!/bin/bash

#
#	Quick and dirty script to setup and launch the latest version quickly
#

#Build the OS before launching it
./build.sh

STATUS=$?	#Get the build's return code (failed or succeeded)

if [ $STATUS != 0 ]	#If the build failed
then
	echo "OS won't be run after build failure. Exiting."
	exit $STATUS
fi

#Else the build was successful

echo "			---- RUNNING OS ----			"

BUILT_FILE=$(ls ./build/)	#Get name of the built file independently
echo "Using file: $BUILT_FILE"

# For compatibility reasons with my home setup.
# Add whatever qemu command variation you have to this if statement so that it picks it
if command -v qemu-virgil	#If qemu-virgil works (snap store)
then
	qemu-virgil -drive file=./build/$BUILT_FILE,format=raw	#Run it
fi

exit 0
