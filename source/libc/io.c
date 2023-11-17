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

#define getReg(a) (asm(a))
void printRegs(){
    printf("test");
    int i;
    char regList[7] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"};
    for(i = 0; i < 7; ++i){
        register int reg getReg(regList[i]);
        printf(regList[i]);
        printf(": ");
        terminal_put64(i);
        printf("\n");
    }
}
