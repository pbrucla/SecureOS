#pragma once

#include <stdbool.h>

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
void write_serial(enum COM_PORT port, const char *s);


/* Blocking read, reads nbytes */
int read_serial(enum COM_PORT port, int nbytes, char *dest);

/* True if there is pending data to be read, False otherwise*/
inline bool read_available(enum COM_PORT port);

/* TODO implement non-blocking read api */