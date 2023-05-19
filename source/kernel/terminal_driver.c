#include <stdint.h>
#include "io.h"
#include "terminal_driver.h"

uint8_t cursor_row;
uint8_t cursor_col;
uint16_t* vga_buffer;
enum vga_color foreground;
enum vga_color background;

static inline uint8_t vga_color_byte(enum vga_color fg, enum vga_color bg) {
    return (bg << 4) | fg;
}

static inline uint16_t vga_entry(uint8_t c, uint8_t color) {
    return (color << 8) | c;
}

void terminal_driver_init() {
    //initialize global variables
    cursor_row = 0;
    cursor_col = 0;
    vga_buffer = (uint16_t*) VGA_BASE;
    foreground = VGA_COLOR_WHITE;
    background = VGA_COLOR_BLACK;
    //initialize settings
    terminal_enable_cursor();
}

void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg, uint8_t row, uint8_t col) {
    int index = row*VGA_WIDTH + col;
    vga_buffer[index] = vga_entry(c, vga_color_byte(fg, bg));
}

void terminal_putchar(char c) {
    switch (c) {
        case '\n':
            cursor_row++;
            cursor_col = 0;
            break;
        default:
            terminal_putentryat(c, foreground, background, cursor_row, cursor_col);
            break;
    }

    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            cursor_row = 0;
        }
    }
}

void terminal_clear() {
    const int VGA_SIZE = VGA_WIDTH*VGA_HEIGHT;
    for (int i = 0; i < VGA_SIZE; i++) {
        vga_buffer[i] = vga_entry('\0', vga_color_byte(foreground, background));
    }
}

void terminal_enable_cursor() {
    uint8_t cursor_start = 0;
    uint8_t cursor_end = 15;
    outb(0x0A, 0x3D4);
	outb((inb(0x3D5) & 0xC0) | cursor_start, 0x3D5);
 
	outb(0x0B, 0x3D4);
	outb((inb(0x3D5) & 0xE0) | cursor_end, 0x3D5);
}

void terminal_disable_cursor() {
    outb(0x0A, 0x3D4);
    outb(0x20, 0x3D5);
    return;
}

void terminal_update_cursor() {
    uint16_t pos = cursor_row*VGA_WIDTH + cursor_col;
    outb(0x0F, 0x3D4);
	outb((uint8_t) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((uint8_t) ((pos >> 8) & 0xFF), 0x3D5);
}

void terminal_move_cursor(uint8_t row, uint8_t col) {
    cursor_row = row; cursor_col = col;
    terminal_update_cursor();
}