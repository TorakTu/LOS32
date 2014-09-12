%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__

bits	16

Puts16:
		pusha
                call CursorHide
.Loop1:
		lodsb
		or	al, al
		jz	Puts16Done
		mov	ah, 0eh
		int	10h
		jmp	.Loop1
Puts16Done:
		popa
		ret


bits 32

%define		VIDMEM	0xB8000
%define		COLS	80
%define		LINES	25
%define		CHAR_ATTRIB 63

_CurX db 0
_CurY db 0

Putch32:

	pusha
	mov	edi, VIDMEM

	xor	eax, eax

	mov	ecx, COLS*2
	mov	al, byte [_CurY]
	mul	ecx
	push	eax

	mov	al, byte [_CurX]
	mov	cl, 2
	mul	cl
	pop	ecx
	add	eax, ecx

	xor	ecx, ecx
	add	edi, eax


	cmp	bl, 0x0A
	je	.Row

	mov	dl, bl
	mov	dh, CHAR_ATTRIB
	mov	word [edi], dx

	inc	byte [_CurX]
	cmp	byte [_CurX], COLS
	je	.Row
	jmp	.done

.Row:
	mov	byte [_CurX], 0
	inc	byte [_CurY]
.done:
	popa
	ret

Puts32:

	pusha
	push	ebx
	pop	edi

.loop:


	mov	bl, byte [edi]
	cmp	bl, 0
	je	.done
	call	Putch32
	inc	edi
	jmp	.loop

.done:
	mov	bh, byte [_CurY]
	mov	bl, byte [_CurX]
	call	MovCur

	popa
	ret

bits 32

MovCur:

	pusha
        xor	eax, eax
	mov	ecx, COLS
	mov	al, bh
	mul	ecx
	add	al, bl
	mov	ebx, eax
	mov	al, 0x0f
	mov	dx, 0x03D4
	out	dx, al

	mov	al, bl
	mov	dx, 0x03D5
	out	dx, al
	xor	eax, eax

	mov	al, 0x0e
	mov	dx, 0x03D4
	out	dx, al

	mov	al, bh
	mov	dx, 0x03D5
	out	dx, al

	popa
	ret

bits 32

ClrScr32:

	pusha
	cld
	mov	edi, VIDMEM
	mov	cx, 2000
	mov	ah, CHAR_ATTRIB
	mov	al, ' '	
	rep	stosw

	mov	byte [_CurX], 0
	mov	byte [_CurY], 0
	popa
	ret

bits 32

GotoXY:
	pusha
	mov	[_CurX], al
	mov	[_CurY], ah
	popa
	ret


CursorHide:
        push  cx
        push  ax
        mov   ch,0x1c
        mov   cl,0x1b
        mov   ah,1
        int   10h
        pop   ax
        pop   cx
        ret 

CursorShow:
        push  cx
        push  ax
        mov   ch,0x0b 
        mov   cl,0x0c 
        mov   ah,1
        int   10h
        pop   ax
        pop   cx
        ret

%endif
