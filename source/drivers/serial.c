#include "serial.h"

static int init_serial_port(COM_PORT port);
inline static int putc_serial(COM_PORT port, char c);

int init_serial()
{
    /* TODO initialize all COM ports */
    init_serial_port(COM1);
}

static int init_serial_port(COM_PORT port)
{

}

void write_serial(COM_PORT port, const string* s)
{
    for (size_t i = 0; i < s->len; ++i)
        putc_serial(port, s->data[i]);
}

int read_serial(COM_PORT port, string* dest)
{

}

inline static int putc_serial(COM_PORT port, char c)
{

}