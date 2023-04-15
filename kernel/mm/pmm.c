#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <limine.h>

#include "drivers/terminal.h"
#include "libc/string.h"

static uint64_t num_blocks;
static uint64_t bitmap_size;

static uint8_t* bitmap;

static volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

inline void pmm_set(size_t bit) {
    bitmap[bit / 8] |= 1 << (bit % 8);
}

inline void pmm_reset(size_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

inline bool pmm_test(size_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init() {
    struct limine_memmap_response* res = memmap_req.response;
    struct limine_memmap_entry** entries = res->entries;

    uint64_t highest_addr = 0;
    for (size_t i = 0; i < res->entry_count; i++) {
        struct limine_memmap_entry* entry = res->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uint64_t entry_range = entry->base + entry->length;
            if (entry_range > highest_addr)
                highest_addr = entry_range;
        }
    }

    // Limine guarantees entry->base and entry->length are 4096 byte aligned
    // Therefore highest_addr is also 4096 byte aligned

    num_blocks = highest_addr / 4096;
    bitmap_size = num_blocks / 8;

    for (size_t i = 0; i < res->entry_count; i++) {
        struct limine_memmap_entry* entry = entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) {
            bitmap = (uint8_t*)entry->base;
            break;
        }
    }

    for (size_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0xff;

    uint64_t usable = 0;
    for (size_t i = 0; i < res->entry_count; i++) {
        struct limine_memmap_entry* entry = res->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            for (size_t j = 0; j < entry->length; j += 4096) {
                pmm_reset((entry->base + j) / 4096);
                usable += 4096;
            }
        }
    }
    
    char buf[16];
    terminal_write("Initialized PMM with ", 22);
    terminal_write(itoa(usable / 1024, buf, 10), 16);
    terminal_write(" KiB of usable memory\n", 23);
}

void* pmm_alloc_block() {
    for (size_t i = 0; i < bitmap_size; i++) {
        if (bitmap[i] & 0xff)
            continue;
        
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (bitmap[i] & (1 << bit))
                continue;
            
            bitmap[i] |= (1 << bit);
            return (void*)((i * 8 + bit) * 4096);
        }
    }

    return 0; // Out of memory
}

void pmm_free_block(void* ptr) {
    size_t addr = (size_t)ptr;
    pmm_reset(addr / 4096);
}