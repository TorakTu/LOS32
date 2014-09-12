
;*********************************************
; http://www.brokenthorn.com/mos/site2/index.html
;	Boot1.asm
;		- A Simple Bootloader
;
;       Edited by TorakTu
;
;	Operating Systems Development Tutorial
;*********************************************

bits	16

org 0x500

jmp	main

%include "stdio.inc"
%include "Gdt.inc"
%include "A20.inc"
%include "Fat12.inc"
%include "common.inc"

LoadingMsg db 0x0D, 0x0A, "Loading the TOS32 Kernel...", 0x00
msgFailure db 0x0D, 0x0A, "*** FATAL: MISSING OR CURRUPT KERNEL32.SYS. Press Any Key to Reboot", 0x0D, 0x0A, 0x0A, 0x00

main:
	cli
	xor	ax, ax
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x0
	mov	ss, ax
	mov	sp, 0xFFFF
	sti

	call	InstallGDT

	call	EnableA20_KKbrd_Out

	mov	si, LoadingMsg
	call	Puts16

	call	LoadRoot

	mov	ebx, 0
    	mov	bp, IMAGE_RMODE_BASE
	mov	si, ImageName
	call	LoadFile
	mov	dword [ImageSize], ecx
	cmp	ax, 0
	je	EnterStage3
	mov	si, msgFailure
	call	Puts16
	mov	ah, 0
	int     0x16
	int     0x19
	cli
	hlt

EnterStage3:

	cli
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	jmp	CODE_DESC:Stage3

bits 32

Stage3:
	mov	ax, DATA_DESC
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h

CopyImage:
  	mov	eax, dword [ImageSize]
  	movzx	ebx, word [bpbBytesPerSector]
  	mul	ebx
  	mov	ebx, 4
  	div	ebx
   	cld
   	mov     esi, IMAGE_RMODE_BASE
   	mov	edi, IMAGE_PMODE_BASE
   	mov	ecx, eax
   	rep	movsd

	jmp	CODE_DESC:IMAGE_PMODE_BASE

	cli
	hlt

