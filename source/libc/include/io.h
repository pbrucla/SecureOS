#pragma once
#include "string.h"

unsigned char inb(unsigned short port);
void outb(unsigned char value, unsigned short port);
//printf attached to terminal for now
int printf(const string* s);