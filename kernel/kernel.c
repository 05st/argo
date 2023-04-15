#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "idt.h"
#include "mm/pmm.h"

#include "drivers/terminal.h"

void _start() {
    terminal_init();
    terminal_write("Argo OS\n", 9);
    
    idt_init();
    pmm_init();
    
    __asm__ ("cli");
    for (;;)
        __asm__ ("hlt");
}