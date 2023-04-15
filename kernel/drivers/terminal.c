#include <stdint.h>
#include <stddef.h>

#include <limine.h>

static volatile struct limine_framebuffer_request framebuffer_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static struct limine_framebuffer* framebuffer;

static volatile struct limine_terminal_request terminal_req = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

static struct limine_terminal* terminal;

void terminal_init() {
    if (!framebuffer_req.response || framebuffer_req.response->framebuffer_count < 1)
        return;
    
    framebuffer = framebuffer_req.response->framebuffers[0];

    if (!terminal_req.response || terminal_req.response->terminal_count < 1)
        return;

    terminal = terminal_req.response->terminals[0];   
}

void terminal_write(const char* buf, size_t length) {
    terminal_req.response->write(terminal, buf, length);
}

void fill_rect() {
    unsigned char* ptr = framebuffer->address;
    uint16_t pw = framebuffer->bpp / 8;

    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            ptr[x*pw + 0] = 0xff;
            ptr[x*pw + 1] = 0xff;
            ptr[x*pw + 2] = 0x00;
        }
        ptr += framebuffer->pitch;
    }
}