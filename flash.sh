#!/bin/bash

#Find attached drives
lsblk | grep 'disk'

#Let me enter the name manually just in case
read -p "Disk name:" dsk_name

#Format the disk as fat16
sudo mkfs.fat -F 16 /dev/$dsk_name 144000

#Burn the OS to disk, not overwriting the BPB that should already be there
sudo dd if=./build/KayriX86.iso of=/dev/$dsk_name count=3 ibs=1	obs=1		#Burn the three byte jmp
sudo dd if=./build/KayriX86.iso of=/dev/$dsk_name skip=62 seek=62 ibs=1 obs=1	#Burn what's after the BPB
	#if: Input file
	#of: Output file
	#count: Number of blocks to copy
	#skip: Skip the first n blocks before starting to write in output file
	#ibs: Input block size (in bytes)
	#obs: Output block size (in bytes)
#The trick is that dd is designed to copy blocks of data (512 bytes), not simply bytes.
#As we need to copy specific byte locations, we say that the size of one block is one byte.
#Therefore we can specify how many blocks to skip as how many bytes to skip (1block = 1byte)
