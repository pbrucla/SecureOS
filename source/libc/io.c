#include "io.h"
#include "string.h"
#include "terminal_driver.h"

unsigned char inb(unsigned short port)
{
    unsigned char value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
void outb(unsigned char value, unsigned short port)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void io_wait() { outb(0, 0x80); }

int printf(const char *s)
{
    for (int i = 0; s[i] != '\0'; i++) {
        terminal_putchar(s[i]);
    }
    terminal_update_cursor();
}
