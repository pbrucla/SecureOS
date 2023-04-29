ASM=nasm
DD=dd
CFLAGS=-I.
QEMU=qemu-system-x86_64
LINKER=ld

BUILD_DIR=build
SRC_DIR=source
KERNEL_DIR=$(SRC_DIR)/kernel
BOOT_DIR=$(SRC_DIR)/boot

GCC=gcc
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
	$(ASM) $(KERNEL_DIR)/kernel_entry.asm -f elf64 -o $(BUILD_DIR)/kernel_entry.o
	$(LINKER) -o $(BUILD_DIR)/kernel.bin -Ttext 0x1000 -z noexecstack $(BUILD_DIR)/kernel_entry.o $(OBJ_FILES) --oformat binary

image: boot kernel
	cat $(BUILD_DIR)/boot-sect.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os_image

run: default
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os_image

clean: 
	rm -rf $(BUILD_DIR)
