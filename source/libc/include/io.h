#pragma once
#include "string.h"

unsigned char inb(unsigned short port);
void outb(unsigned char value, unsigned short port);
void outbb(unsigned short port, unsigned char value);
void io_wait();
// printf attached to terminal for now
int printf(const char *s, ...);
int printch(char c);