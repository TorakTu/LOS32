#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

typedef int size_t;

#define KBRD_INTRFC 0x64
#define KBRD_BIT_KDATA 0
#define KBRD_BIT_UDATA 1
#define KBRD_IO 0x60
#define KBRD_RESET 0xFE
#define bit(n) (1<<(n))
#define check_flag(flags, n) ((flags) & bit(n))

int timer_ticks = 0;
int arr[256];

unsigned char mouse_cycle=0;
signed char mouse_bytes[3];
signed char mouse_x=0;
signed char mouse_y=0;

unsigned char kbdus[128] =
{
   0,
  27,
 '1',
 '2',
 '3',
 '4',
 '5',
 '6',
 '7',
 '8',
 '9',
 '0',
 '-',
 '=',
'\b',
'\t',
 'q',
 'w',
 'e',
 'r',
 't',
 'y',
 'u',
 'i',
 'o',
 'p',
 '[',
 ']',
'\n',
   0,
 'a',
 's',
 'd',
 'f',
 'g',
 'h',
 'j',
 'k',
 'l',
 ';',
'\'',
 '`',
   0,
'\\',
 'z',
 'x',
 'c',
 'v',
 'b',
 'n',
 'm',
 ',',
 '.',
 '/',
   0,
 '*',
   0,
 ' ',
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,	
   0,
   0,
   0,
   0,
   0,
 '-',
   0,
   0,
   0,
 '+',
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
};

/* External Functions */
/* --GDT-- */
extern void gdt_flush();

/* --IDT-- */
extern void idt_load();

/* --IRQ-- */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* --ISR-- */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* Internal Functions */
/* --GDT-- */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

/* --IDT-- */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

/* --IRQ-- */
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();

/* --ISR-- */
void isrs_install();

/* --I/O PORT FUNCTIONS-- */
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

/* --MEMORY FUNCTIONS-- */
void *memset(void *dest, char val, size_t count);

/* --TIMER FUNCTIONS-- */
void timer_wait(int ticks);
void timer_install();

/* --KEYBOARD FUNCTIONS-- */
void clear_keyboard_buffers();

/* --MOUSE FUNCTIONS-- */
void mouse_handler(struct regs *a_r);
unsigned char mouse_read();
void mouse_install();

/* --EXTRA FUNCTIONS-- */
void clear(unsigned int bcolor, unsigned int fcolor);
unsigned int printf(char *message, unsigned int col, unsigned int row, unsigned int bcolor, unsigned int fcolor);
void bscreen(void);
void reboot_me();

#endif