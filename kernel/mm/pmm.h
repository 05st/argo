#ifndef PMM_H
#define PMM_H

void pmm_init();

void* pmm_alloc_block();
void pmm_free_block();

#endif