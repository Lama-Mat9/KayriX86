#!/bin/bash

rm -r build/*
nasm -f bin ./src/main.asm -o ./build/BootSector.bin

# Compatibility reasons with my home setup
if command -v qemu-virgil
then
	qemu-virgil ./build/BootSector.bin
fi
