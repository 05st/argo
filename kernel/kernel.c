#include <stdint.h>
#include <stddef.h>

#include "limine.h"

static volatile struct limine_terminal_request term_req = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

void _start() {
    if (!term_req.response || term_req.response->terminal_count < 1)
        return;

    struct limine_terminal* term = term_req.response->terminals[0];
    term_req.response->write(term, "Argo OS", 11);
    
    for (;;)
        __asm__("hlt");
}