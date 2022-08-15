#include <stdint.h>

#include <drivers/terminal.h>

extern void* isr_stub_table [];

struct __attribute__((__packed__)) idt_entry_t {
    uint16_t isr_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attrs;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t zero;
};

struct __attribute__((__packed__)) idtr_t {
    uint16_t limit;
    uint64_t base;
};

__attribute__((aligned(0x10)))
static struct idt_entry_t idt[256];
static struct idtr_t idtr;

void exception_handler() {
    terminal_write("Encountered exception\n", 23);
    __asm__ volatile ("cli; hlt");
}

void idt_set_descriptor(int i, void* isr, uint8_t attrs) {
    struct idt_entry_t* entry = &idt[i];

    entry->isr_low = (uint64_t)isr & 0xffff;
    entry->selector = 0x28;
    entry->ist = 0;
    entry->attrs = attrs;
    entry->isr_mid = ((uint64_t)isr >> 16) & 0xffff;
    entry->isr_high = ((uint64_t)isr >> 32) & 0xffffffff;
    entry->zero = 0;
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(struct idt_entry_t) * 256 - 1;

    for (uint8_t i = 0; i < 32; i++)
        idt_set_descriptor(i, isr_stub_table[i], 0x8e);

    __asm__ volatile ("lidt %0" :: "m" (idtr));
    __asm__ volatile ("sti");
}