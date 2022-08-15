#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>

void terminal_init();
void terminal_write(const char* buf, size_t length);

#endif