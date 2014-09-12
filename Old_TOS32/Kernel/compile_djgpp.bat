echo off

yasm-1.3.0-win32 -f coff -o start.o start_djgpp.asm

gcc -Ofast -ffreestanding -c -o functions.o functions.c

gcc -Ofast -ffreestanding -c -o kernel.o kernel.c

ld -T linker.ld --oformat binary -o kernel32.sys start.o kernel.o functions.o

move *.sys F:\

del *.o

pause

echo on