#!/bin/bash

#Find attached drives
lsblk |  grep 'disk'

#Let me enter the name manually just in case
read -p "Disk name:" dsk_name

#Burn it to disk
sudo dd if=./build/BootSector.bin of=/dev/$dsk_name

