#include <stdint.h>
#include <stddef.h>

#include <limine.h>

static volatile struct limine_terminal_request term_req = {.id = LIMINE_TERMINAL_REQUEST, .revision = 0};
static struct limine_terminal* term;

void terminal_init() {
    if (!term_req.response || term_req.response->terminal_count < 1)
        return;

    term = term_req.response->terminals[0];
}

void terminal_write(const char* buf, size_t length) {
    term_req.response->write(term, buf, length);
}