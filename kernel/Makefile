override MAKEFLAGS += -rR
override KERNEL := argo.elf

define DEFAULT_VAR =
    ifeq ($(origin $1), default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1), undefined)
        override $(1) := $(2)
    endif
endef

$(eval $(call DEFAULT_VAR, CC, cc))
$(eval $(call DEFAULT_VAR, LD, ld))
$(eval $(call DEFAULT_VAR, CFLAGS, -g -O2 -pipe))
$(eval $(call DEFAULT_VAR, CPPFLAGS, ))
$(eval $(call DEFAULT_VAR, NASMFLAGS, -F dwarf -g))
$(eval $(call DEFAULT_VAR, LDFLAGS, ))

override CFLAGS += \
    -Wall \
    -Wextra \
    -std=c17 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIE \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

override CPPFLAGS := \
    -I. \
    $(CPPFLAGS) \
    -MMD \
    -MP

override LDFLAGS += \
    -nostdlib \
    -static \
    -m elf_x86_64 \
    -z max-page-size=0x1000 \
    -T linker.ld

ifeq ($(shell $(LD) --help 2>&1 | grep 'no-pie' >/dev/null 2>&1; echo $$?),0)
    override LDFLAGS += -no-pie
endif

override NASMFLAGS += \
    -Wall \
    -f elf64

override CFILES := $(shell find -L . -type f -name '*.c')
override ASFILES := $(shell find -L . -type f -name '*.S')
override NASMFILES := $(shell find -L . -type f -name '*.asm')
override OBJ := $(CFILES:.c=.o) $(ASFILES:.S=.o) $(NASMFILES:.asm=.o)
override HEADER_DEPS := $(CFILES:.c=.d) $(ASFILES:.S=.d)

.PHONY: all
all: $(KERNEL)

limine.h:
	curl -Lo $@ https://github.com/limine-bootloader/limine/raw/trunk/limine.h

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

-include $(HEADER_DEPS)

%.o: %.c limine.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.S limine.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.asm
	nasm $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -f limine.h