#include "terminal_driver.h"
#include "io.h"
#include <stdint.h>

uint8_t cursor_row = 0;
uint8_t cursor_col = 0;
uint16_t *vga_buffer = (uint16_t *)VGA_BASE;
enum vga_color foreground = VGA_COLOR_WHITE;
enum vga_color background = VGA_COLOR_BLACK;

static uint8_t has_initialized = 0;

static inline uint8_t vga_color_byte(enum vga_color fg, enum vga_color bg)
{
    return (bg << 4) | fg;
}

static inline uint16_t vga_entry(uint8_t c, uint8_t color)
{
    return (color << 8) | c;
}

uint8_t terminal_driver_loaded(void) { return has_initialized; }

void terminal_driver_init()
{
    has_initialized = 1;
    // initialize settings
    terminal_enable_cursor();
}

void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg,
                         uint8_t row, uint8_t col)
{
    int index = row * VGA_WIDTH + col;
    vga_buffer[index] = vga_entry(c, vga_color_byte(fg, bg));
}

void terminal_putchar(char c)
{
    switch (c) {
    case '\n':
        cursor_row++;
        cursor_col = 0;
        break;
    case '\b':
        if (cursor_col) {
            cursor_col--;
            terminal_putentryat('\0', foreground, background, cursor_row,
                                cursor_col);
        } else if (cursor_row) {
            cursor_row--;
            cursor_col = VGA_WIDTH - 1;
        }
        break;
    default:
        terminal_putentryat(c, foreground, background, cursor_row, cursor_col);
        if (++cursor_col == VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
        break;
    }
    if (cursor_row == VGA_HEIGHT) {
        cursor_row = 0;
        terminal_clear();
    }
}

void terminal_put64(unsigned long long n)
{
    char temp;
    for (int i = 1; i <= 16; i++) {
        temp = n >> (64 - i * 4);
        temp &= 0xF;
        if (temp < 10)
            temp += '0';
        else
            temp += 'A' - 10;

        terminal_putchar(temp);
    }
}

void terminal_clear()
{
    const int VGA_SIZE = VGA_WIDTH * VGA_HEIGHT;
    for (int i = 0; i < VGA_SIZE; i++) {
        vga_buffer[i] = vga_entry('\0', vga_color_byte(foreground, background));
    }
}

void terminal_enable_cursor()
{
    uint8_t cursor_start = 0;
    uint8_t cursor_end = 15;
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void terminal_disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
    return;
}

void terminal_update_cursor()
{
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_move_cursor(uint8_t row, uint8_t col)
{
    cursor_row = row;
    cursor_col = col;
    terminal_update_cursor();
}
