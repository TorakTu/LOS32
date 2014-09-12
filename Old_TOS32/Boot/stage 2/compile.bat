echo off

cls

del F:\loader.sys

yasm-1.3.0-win32 -f bin Stage2.asm -o loader.sys

move loader.sys F:\

echo on

pause