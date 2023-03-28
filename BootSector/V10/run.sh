rm -r build/*
nasm -f bin ./src/main.asm -o ./build/BootSector.bin
qemu-virgil ./build/BootSector.bin
