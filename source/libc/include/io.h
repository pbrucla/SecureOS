#pragma once
#include "string.h"

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);
void io_wait();
// printf attached to terminal for now
int printf(const char *s, ...);
int printch(char c);
void unitTest(const int errorNum,const char* errorMessage, const int bool);
void PrintRegs();
