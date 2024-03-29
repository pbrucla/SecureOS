#pragma once
#include <stdint.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BASE 0x800b8000;

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};
// initialize global variables
void terminal_driver_init();
uint8_t terminal_driver_loaded();
// manipulate char buffer
void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg,
                         uint8_t row, uint8_t col);
void terminal_putchar(char c);
void terminal_put64(unsigned long long n);
void terminal_clear();
// cursor
void terminal_enable_cursor();
void terminal_disable_cursor();
void terminal_update_cursor();
void terminal_move_cursor(uint8_t row, uint8_t col);
