#include "terminal_driver.h"

uint16_t* current;

void terminal_driver_init() {
  current = VGA_BASE;  
}

void print_char(char c) {
  
}