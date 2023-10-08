# Change this to your cross-compiler prefix (e.g. i686-elf for MacOS and i686-linux-gnu for WSL & Linux)
PREFIX=i686-linux-gnu
ASM=nasm
DD=dd
CFLAGS=-I. -O2
QEMU=qemu-system-i386
LINKER=$(PREFIX)-ld
LINKER_FILE=linker.ld
OBJCOPY=$(PREFIX)-objcopy

BUILD_DIR=build
SRC_DIR=source
KERNEL_DIR=$(SRC_DIR)/kernel
BOOT_DIR=$(SRC_DIR)/boot
LIBC_DIR=$(SRC_DIR)/libc

INCLUDES=$(KERNEL_DIR)/include
LIBC=$(LIBC_DIR)/include

GCC=$(PREFIX)-gcc
GCC_FLAGS=-ffreestanding -I$(INCLUDES) -I$(LIBC)
FORMAT=clang-format -i

default: 
	mkdir -p $(BUILD_DIR)
	$(MAKE) image

boot: $(BOOT_DIR)/*.asm
	$(ASM) $(BOOT_DIR)/boot-sect.asm -i $(BOOT_DIR) -f bin -o $(BUILD_DIR)/boot-sect.bin

LIBC_SRC=$(wildcard $(LIBC_DIR)/*.c)
OBJ_FILES=$(patsubst $(LIBC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIBC_SRC)) \
		  $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(KERNEL_DIR)/*.c)) \
		  $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(wildcard $(KERNEL_DIR)/*.asm))
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm
	$(ASM) $< -f elf32 -o $@
$(BUILD_DIR)/%.o: $(LIBC_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@

kernel: $(OBJ_FILES)
	$(LINKER) -o $(BUILD_DIR)/kernel.elf -Ttext 0x80100000 -z noexecstack $(BUILD_DIR)/kernel_entry.o $(OBJ_FILES) 
	$(OBJCOPY) --only-keep-debug $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.sym
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin

image: boot kernel
	cat $(BUILD_DIR)/boot-sect.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os_image
	$(DD) if=/dev/zero of=$(BUILD_DIR)/os_image seek=1 obs=1024 count=0 conv=notrunc

run: default
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os_image

dockerun:
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os_image

format:
	find source -type f -name '*.c' -or -name '*.h' -or -name '*.asm' | xargs dos2unix
	find source/ -type f -name '*.c' -or -name '*.h' | xargs $(FORMAT)

clean: 
	rm -rf $(BUILD_DIR)

.PHONY: clean format dockerun run image kernel boot default
