#pragma once
#include "string.h"

unsigned char inb(unsigned short port);
void outb(unsigned char value, unsigned short port);
void io_wait();
// printf attached to terminal for now
int printf(const char *s);
void unitTest(const int errorNum,const char* errorMessage, const int bool);