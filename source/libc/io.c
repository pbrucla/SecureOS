#include "io.h"
#include "string.h"
#include "terminal_driver.h"
#include <stdarg.h>

unsigned char inb(unsigned short port)
{
    unsigned char value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void io_wait() { outb(0, 0x80); }

int printf(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '%') {
            switch (s[++i]) {
            case 'x':
                terminal_put64(va_arg(args, int));
                break;
            }
        } else
            terminal_putchar(s[i]);
    }
    terminal_update_cursor();
}

int printch(char c)
{
    terminal_putchar(c);
    terminal_update_cursor();
}

void unitTest(const int errorNum,const char* errorMessage, const int bool){
    terminal_put64(errorNum);
    terminal_putchar(':');
    terminal_putchar(' ');
    printf(errorMessage);
    
    if(bool==1)  asm("hlt");
}

void printRegs(){
    int i;
    asm("mov %%eax, %0" : "+g" (i)::);
    printf("eax: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%ebx, %0" : "+g" (i)::);
    printf("ebx: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%ecx, %0" : "+g" (i)::);
    printf("ecx: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%edx, %0" : "+g" (i)::);
    printf("edx: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%esi, %0" : "+g" (i)::);
    printf("esi: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%edi, %0" : "+g" (i)::);
    printf("edi: ");
    terminal_put64(i);
    printf("\n");
    
    asm("mov %%ebp, %0" : "+g" (i)::);
    printf("ebp: ");
    terminal_put64(i);
    printf("\n");
}
