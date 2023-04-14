#!/bin/bash

#
#	Quick and dirty script to setup and launch the latest version quickly
#

#Build the OS before launching it
./build.sh

echo "			---- RUNNING OS ----			"

# For compatibility reasons with my home setup.
# Add whatever qemu command variation you have to this if statement so that it picks it
if command -v qemu-virgil	#If qemu-virgil works (snap store)
then
	qemu-virgil -drive file=./build/KayriX86.iso,format=raw		#Run it
fi
