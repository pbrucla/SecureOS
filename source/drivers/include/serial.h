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

/**
 * Return 1 if init is succesful
 * Return 0 otherwise
 */
int serial_driver_init();
void write_serial(COM_PORT port, const string* s);

/* TODO implement this */
int read_serial(COM_PORT port, string *dest);