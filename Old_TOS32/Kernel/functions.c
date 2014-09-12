#include "defines.h"
#include "structs.h"
#include "functions.h"

void clear(unsigned int bcolor, unsigned int fcolor)
{
   char *vidmem = (char *) VIDEO_MEM_80x25_03;
   unsigned int i=0;
   while(i < (80*25*2))
   {
      vidmem[i]=' ';
      i++;
      vidmem[i]=fcolor + (bcolor * 16);
      i++;
   }
}

unsigned int printf(char *message, unsigned int col, unsigned int row, unsigned int bcolor, unsigned int fcolor)
{
   char *vidmem = (char *) VIDEO_MEM_80x25_03;
   unsigned int i=0;

   i=(row*80*2) + (col*2);

   while(*message!=0)
   {
      if(*message=='\n')
      {
         row++;
         i=(row*80*2);
         *message++;
      } else {
         vidmem[i]=*message;
         *message++;
         i++;
         vidmem[i]=fcolor + (bcolor * 16);
         i++;
      }
   }
   return 1;
}

void bscreen(void)
{
   printf("Welcome to TOS 32-Bit, Version 1.0 ( Tutorial 7 ) - 2014", 12, 8, BLACK, LIGHTRED);

}

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install()
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);

    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush();
}


void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    idt_load();
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}


unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void *irq_routines[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);

    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

void irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);

    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    outportb(0x20, 0x20);
}

void isrs_install()
{
    idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);

    idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);

    idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);

    idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}

unsigned char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        clear(BLACK,BLACK);
        printf("Exception. System Halted!\n",33, 14, RED, BLACK);
        printf(exception_messages[r->int_no],33, 15, RED, BLACK);
        for (;;);
    }
}

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    scancode = inportb(0x60);

    if (scancode & 0x80)
    {
        /* */
    }
    else
    {
        arr[0] = kbdus[scancode];
        printf((char *)&arr[0],40, 13, BLACK, WHITE);
        printf("You hit a key on the keyboard !!",23, 15, CYAN, BLACK);
        if(arr[0] == 'q')
            reboot_me();
    }
}

void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}

void clear_keyboard_buffers()
{
    unsigned char temp;

    do
    {
        temp = inportb(KBRD_INTRFC); 
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
               inportb(KBRD_IO);

    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);
}

void reboot_me()
{
    clear_keyboard_buffers();
    outportb(KBRD_INTRFC, KBRD_RESET);
}

void timer_handler(struct regs *r)
{
    timer_ticks++;
    if (timer_ticks % 18 == 0)
    {
        ;
    }
}

void timer_install()
{
    irq_install_handler(0, timer_handler);
}

void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

void mouse_handler(struct regs *r)
{
 mouse_bytes[mouse_cycle++] = inportb(0x60);

 if (mouse_cycle == 3)
 { 
    mouse_cycle = 0;

     /* mouse_x=mouse_bytes[1]; */
     /* mouse_y=mouse_bytes[2]; */

    if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40))
      return;
    if (!(mouse_bytes[0] & 0x20))
      mouse_y |= 0xFFFFFF00; //delta-y is a negative value
    if (!(mouse_bytes[0] & 0x10))
      mouse_x |= 0xFFFFFF00; //delta-x is a negative value
    if (mouse_bytes[0] & 0x4)
      printf("Middle mouse button is pressed!", 23,14, RED,BLUE);
    if (mouse_bytes[0] & 0x2)
      printf("Right mouse button is pressed! ", 23,14, RED,BLUE);
    if (mouse_bytes[0] & 0x1)
      printf("Left mouse button is pressed!  ", 23,14, RED,BLUE);
  }
}

inline void mouse_wait(unsigned char a_type)
{
  unsigned int _time_out = 100000;
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(unsigned char a_write)
{
  mouse_wait(1);
  outportb(0x64, 0xD4);
  mouse_wait(1);
  outportb(0x60, a_write);
}

unsigned char mouse_read()
{
  mouse_wait(0);
  return inportb(0x60);
}

void mouse_install()
{
  unsigned char _status;

  mouse_wait(1);
  outportb(0x64, 0xA8);
 
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);
 
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  irq_install_handler(12, mouse_handler);
}