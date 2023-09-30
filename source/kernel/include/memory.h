#pragma once
#include <stddef.h>

void init_paging();
void* kmalloc(size_t size, unsigned int flags);
