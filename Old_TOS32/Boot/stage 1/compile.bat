echo off

cls

del floppy.img

del *.bin

dd bs=512 count=2880 if=/dev/zero of=floppy.img

yasm-1.3.0-win32 -f bin Boot1.asm -o Boot1.bin

dd if=boot1.bin of=floppy.img bs=512 count=1

echo on

pause