ASM=nasm
DD=dd
CFLAGS=-I.
QEMU=qemu-system-i386
LINKER=i686-elf-ld
LINKER_FILE=linker.ld
OBJCOPY=i686-elf-objcopy

BUILD_DIR=build
SRC_DIR=source
KERNEL_DIR=$(SRC_DIR)/kernel
BOOT_DIR=$(SRC_DIR)/boot

GCC=i686-elf-gcc
GCC_FLAGS=-ffreestanding

default: 
	mkdir -p $(BUILD_DIR)
	$(MAKE) image

boot: $(BOOT_DIR)/*.asm
	$(ASM) $(BOOT_DIR)/boot-sect.asm -i $(BOOT_DIR) -f bin -o $(BUILD_DIR)/boot-sect.bin
	$(DD) if=/dev/zero of=$(BUILD_DIR)/boot-sect.bin seek=5 obs=1024 count=0

SRC_FILES=$(wildcard $(KERNEL_DIR)/*.c)
OBJ_FILES=$(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@

kernel: $(OBJ_FILES) $(KERNEL_DIR)/kernel_entry.asm
	$(ASM) $(KERNEL_DIR)/kernel_entry.asm -f elf32 -o $(BUILD_DIR)/kernel_entry.o
	$(LINKER) -o $(BUILD_DIR)/kernel.elf -Ttext 0x1000 -z noexecstack $(BUILD_DIR)/kernel_entry.o $(OBJ_FILES) 
	$(OBJCOPY) --only-keep-debug $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.sym
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin

image: boot kernel
	cat $(BUILD_DIR)/boot-sect.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os_image

run: default
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os_image

clean: 
	rm -rf $(BUILD_DIR)
