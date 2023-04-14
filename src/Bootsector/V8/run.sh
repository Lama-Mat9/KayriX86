rm -r build/*
nasm -f bin ./src/BootSector.asm -o ./build/BootSector.bin
qemu-virgil ./build/BootSector.bin
