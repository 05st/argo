#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <idt.h>

#include <drivers/terminal.h>

void _start() {
    terminal_init();
    terminal_write("Argo OS\n", 8);
    
    idt_init();
    
    for (;;)
        __asm__ ("hlt");
}