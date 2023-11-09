#pragma once

#include "string.h"

enum COM_PORT {
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
    COM5 = 0x5F8,
    COM6 = 0x4F8,
    COM7 = 0x5E8,
    COM8 = 0x4E8
};

int init_serial();
void write_uart(COM_PORT port, const string* s);
int read_uart(COM_PORT port, string *dest);