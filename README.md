# KayriX86
Simple X86 operating system. Making it in order to learn new things.  

Hi! Please consider that this project is nothing more than something that i'm making as a hobby, nothing serious.  
This is my first operating system and i still have lots of things to learn.  
If you read my code, expect to see some werid/incorrect/dumb stuff in here (especially my build scripts geizgbzeiujgnfefz).  
You've been warned!

In any case, it could still be useful to someone to see how my OS looks like to get ideas or debug their own. That's why it's made public !  
You can also point out what i've done wrong too, i could *try* fixing it...

### Quickstart
You can use any x86 emulator you want (boschs and others).
However, the quickest way to run KayriX86 would probably be to grab QEMU for i386 (named qemu-system-i386 on apt) or any QEMU build from your distribution's app store.  
Running:
> qemu-system-i386 build/KayriX86.iso  

It's not the most ideal way, but would work.  
If you plan on using QEMU you can checkout my run.sh script which contains the full command that i use to run mine, and possibly add yours.  

### Building
At the time of writing this you would only require ld, GCC and NASM as well as other basic binaries which come by default on most linux distros.
Once again—my build scripts are really messy. If they don't work for you simply don't bother trying to fix them, there's too much black magic happening in there.
