#include "timer.h"
#include "io.h"

#define PORT_CHANNEL_0 0x40
#define PORT_CHANNEL_1 0x41
#define PORT_CHANNEL_2 0x42
#define PORT_CMD 0x43

// Channel options
#define CHANNEL_0 (0b00 << 6)
#define CHANNEL_1 (0b01 << 6)
#define CHANNEL_2 (0b10 << 6)
#define READ_BACK (0b11 << 6)

// Access mode options
#define LATCH_COUNT (0b00 << 4)
#define LOBYTE_ONLY (0b01 << 4)
#define HIBYTE_ONLY (0b10 << 4)
#define LOBYTE_HIBYTE (0b11 << 4)

// Operating mode options
#define MODE_0 (0b000 << 1) //interrupt on terminal count
#define MODE_1 (0b001 << 1) //hardware retriggerable one-shot
#define MODE_2 (0b010 << 1) //rate generator
#define MODE_3 (0b011 << 1) //square wave generator
#define MODE_4 (0b100 << 1) //software strobe
#define MODE_5 (0b101 << 1) //hardware strobe

// BCD/Binary mode options
#define BINARY_MODE 0
#define BCD_MODE 1

uint32_t divisor;
uint32_t tick;

void timer_init(uint32_t hz) {
    tick = 0;
    divisor = 1193180 / hz;
    outb(PORT_CMD, CHANNEL_0 | LOBYTE_HIBYTE | MODE_3 | BINARY_MODE);
    uint8_t lowbyte = (uint8_t) (divisor & 0xFF);
    uint8_t highbyte = (uint8_t) ((divisor >> 8) & 0xFF);
    outb(PORT_CHANNEL_0, lowbyte);
    outb(PORT_CHANNEL_0, highbyte);
}

void timer_irq(registers_t *frame) {
    tick++;
    // printf("%x\n", tick);
}
