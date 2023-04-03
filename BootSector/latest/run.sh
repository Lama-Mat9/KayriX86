#!/bin/bash

# Compatibility reasons with my home setup
if command -v qemu-virgil
then
	qemu='qemu-virgil'
elif command -v qemu-system-i386
then
	qemu='qemu-system-i386'
fi

rm -r build/*
nasm -f bin ./src/main.asm -o ./build/BootSector.bin
$qemu ./build/BootSector.bin
