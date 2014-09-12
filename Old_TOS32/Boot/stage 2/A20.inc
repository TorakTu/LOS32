%ifndef __A20_INC_67343546FDCC56AAB872_INCLUDED__
%define __A20_INC_67343546FDCC56AAB872_INCLUDED__

bits	16

EnableA20_KKbrd:

	cli
	push	ax
	mov	al, 0xdd
	out	0x64, al
	pop	ax
	ret

EnableA20_KKbrd_Out:

	cli
	pusha

        call    wait_input
        mov     al,0xAD
        out     0x64,al
        call    wait_input

        mov     al,0xD0
        out     0x64,al
        call    wait_output

        in      al,0x60
        push    eax
        call    wait_input

        mov     al,0xD1
        out     0x64,al
        call    wait_input

        pop     eax
        or      al,2
        out     0x60,al

        call    wait_input
        mov     al,0xAE
        out     0x64,al

        call    wait_input
	popa
        sti
        ret

wait_input:
        in      al,0x64
        test    al,2
        jnz     wait_input
        ret

wait_output:
        in      al,0x64
        test    al,1
        jz      wait_output
        ret

EnableA20_Bios:
	pusha
	mov	ax, 0x2401
	int	0x15
	popa
	ret

EnableA20_SysControlA:
	push	ax
	mov	al, 2
	out	0x92, al
	pop	ax
	ret

%endif
