#include "serial.h"
#include "io.h"

static int init_serial_port(enum COM_PORT port);
inline static int putc_serial(enum COM_PORT port, char c);

int serial_driver_init()
{
    return init_serial_port(COM1);

    /* TODO initialize all COM ports */
}

static int init_serial_port(enum COM_PORT port)
{
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); //                  (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E); // Set to loopback mode for test byte
    outb(port + 0, 0xAE); // Send test byte

    // If test byte was not returned, serial is faulty
    if (inb(port + 0) != 0xAE)
      return 0;

    // // Otherwise, serial is operational, therefore disable loopback mode
    outb(port + 4, 0x0F);
    return 1;
}

void write_serial(enum COM_PORT port, const char *s)
{
    for (size_t i = 0; s[i] != '\0'; ++i)
        putc_serial(port, s[i]);
}

int read_serial(enum COM_PORT port, char *dest)
{
    return 0;
}

inline static int transmit_buffer_empty(enum COM_PORT port)
{
    /* Read from transmit buffer empty register */
    return inb(port + 5) & 0x20;
}

inline static int putc_serial(enum COM_PORT port, char c)
{
    /* Wait until transmit buffer is empty */
    while (!transmit_buffer_empty(port))
        ;

    /* Write character to output buffer */
    outb(port + 0, c);
}