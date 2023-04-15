#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "idt.h"
#include "mm/pmm.h"

#include "drivers/terminal.h"

#include "libc/string.h"

void _start() {
    terminal_init();
    terminal_write("Argo OS\n", 9);
    
    idt_init();
    pmm_init();
    
    void* addr = pmm_alloc_block();
    
    char* buf = (char*)addr;
    itoa((size_t)addr, buf, 10);

    terminal_write(buf, 16);
    
    pmm_free_block(addr);
    
    __asm__ ("cli; hlt");
}