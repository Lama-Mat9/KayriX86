#!/bin/bash

#
#	Quick and dirty script to setup and launch the latest version quickly
#

#Build the OS before launching it
bash build.sh

STATUS=$?	#Get the build's return code (failed or succeeded)

#Color codes useful for printing
RED='\033[0;31m'
NC='\033[0m'

if [ $STATUS != 0 ]	#If the build failed
then
	echo -e "${RED}OS won't be run after build failure. Exiting.${NC}"
	exit $STATUS
fi

#Else the build was successful
bash run.sh

exit 0
