#pragma once
#include <stdint.h>
#include "idt.h"

void timer_init(uint32_t hz);
void timer_irq(registers_t *frame);
