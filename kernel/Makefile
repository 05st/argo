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

CFLAGS ?= -O2 -g -Wall -Wextra -Wpedantic -pipe
NASMFLAGS ?= -F dwarf -g
LDFLAGS ?=
PPFLAGS ?=

override CFLAGS +=			\
	-I. 					\
	-std=c17 				\
	-ffreestanding 			\
	-fno-stack-protector 	\
	-fno-stack-check		\
	-fno-pie				\
	-fno-pic				\
	-m64					\
	-march=x86-64			\
	-mabi=sysv				\
	-mno-80387				\
	-mno-mmx				\
	-mno-sse				\
	-mno-sse2				\
	-mno-red-zone			\
	-mcmodel=kernel			\
	-MMD

override LDFLAGS +=				\
	-nostdlib					\
	-static						\
	-Wl,--build-id=none			\
	-Wl,-z,max-page-size=0x1000 \
	-Wl,-T,linker.ld

override NASMFLAGS += \
	-f elf64

override CFILES := $(shell find ./ -type f -name '*.c')
override ASFILES := $(shell find ./ -type f -name '*.S')
override NASMFILES = $(shell find ./ -type f -name '*.asm')
override OBJ := $(CFILES:.c=.o) $(ASFILES:.S=.o) $(NASMFILES:.asm=.o)
override HEADER_DEPS := $(CFILES:.c=.d) $(ASFILES:.S=.d)

.PHONY: all
all: $(KERNEL)

limine.h:
	curl https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -o $@

$(KERNEL): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

-include $(HEADER_DEPS)

%.o: %.c limine.h
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)
	
.PHONY: distclean
distclean: clean
#	rm -f limine.h