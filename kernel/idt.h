#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init();
void idt_set_descriptor(int i, void* isr, uint8_t attrs);

#endif