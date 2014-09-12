/*
;*********************************************
;
;       Original Tutorial : Bona Fide OS Developer
;
;       http://www.osdever.net/
;
;       Edited by TorakTu
;
;	OS Protected Mode Development Tutorial
;
;*********************************************
*/

#include "defines.h"

extern void gdt_install();
extern void idt_install();
extern void isrs_install();
extern void irq_install();
extern void timer_install();
extern void keyboard_install();
extern void mouse_install();

extern void clear_keyboard_buffers();
extern void clear(unsigned int bcolor, unsigned int fcolor);
extern void bscreen(void);

void main(void)
{

   gdt_install();
   idt_install();
   isrs_install();
   irq_install();
   timer_install();
   keyboard_install();
   mouse_install();

   __asm__ __volatile__ ("sti");

   clear_keyboard_buffers();
   clear(BLACK,BLACK);
   bscreen();
   for (;;);
}
