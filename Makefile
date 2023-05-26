# Change this to your cross-compiler prefix (e.g. i686-elf for MacOS and i686-linux-gnu for WSL & Linux)
PREFIX=i686-linux-gnu
ASM=nasm
DD=dd
CFLAGS=-I.
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

KERNEL_SRC=$(wildcard $(KERNEL_DIR)/*.c)
LIBC_SRC=$(wildcard $(LIBC_DIR)/*.c)
OBJ_FILES=$(patsubst $(LIBC_DIR)/%.c,$(BUILD_DIR)/%.o, $(LIBC_SRC)) \
		  $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o, $(KERNEL_SRC))
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(LIBC_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@

kernel: $(OBJ_FILES) $(KERNEL_DIR)/kernel_entry.asm
	$(ASM) $(KERNEL_DIR)/kernel_entry.asm -f elf32 -o $(BUILD_DIR)/kernel_entry.o
	$(LINKER) -o $(BUILD_DIR)/kernel.elf -Ttext 0x1000 -z noexecstack $(BUILD_DIR)/kernel_entry.o $(OBJ_FILES) 
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
	find source -type f -name '*.c' -or -name '*.h' | xargs dos2unix
	find source/ -type f -name '*.c' -or -name '*.h' | xargs ${FORMAT}

clean: 
	rm -rf $(BUILD_DIR)
