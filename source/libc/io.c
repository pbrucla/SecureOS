#include "io.h"

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
