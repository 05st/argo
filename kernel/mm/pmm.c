#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

#include <drivers/terminal.h>

static uint8_t* bitmap;

static volatile struct limine_memmap_request memmap_req = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void pmm_init() {
    struct limine_memmap_response* resp = memmap_req.response;

    uint64_t range = 0;
    for (size_t i = 0; i < resp->entry_count; i++) {
        struct limine_memmap_entry* entry = resp->entries[i];
        uint64_t entry_range = entry->base + entry_range;
        if (entry->type == LIMINE_MEMMAP_USABLE && entry_range > range)
            range = entry_range;
    }

    uint64_t pages = range / 4096;
    uint64_t bitmap_size = (((pages / 8) + 4096 - 1) / 4096) * 4096; // num pages rounded up * 4096
    
    for (size_t i = 0; i < resp->entry_count; i++) {
        struct limine_memmap_entry* entry = resp->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) {
            bitmap = (uint8_t*)entry->base;
            break;
        }
    }
    
    for (size_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0xff;

    for (size_t i = 0; i < resp->entry_count; i++) {
        struct limine_memmap_entry* entry = resp->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
            for (size_t j = 0; j < entry->length; j += 4096)
                bitmap[(entry->base + j) / 4096 / 8] &= ~(1 << (((entry->base + j) / 4096) % 8));
    }
}

void pmm_alloc() {
    
}