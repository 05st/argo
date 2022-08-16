#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <idt.h>

#include <drivers/terminal.h>
#include <mm/pmm.h>

void _start() {
    terminal_init();
    terminal_write("Argo OS\n", 9);
    
    idt_init();
    pmm_init();
    
    for (;;)
        __asm__ ("hlt");
}