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

default: 
	mkdir -p $(BUILD_DIR)
	$(MAKE) boot
	$(MAKE) kernel
	$(MAKE) image

boot: $(BOOT_DIR)/*.asm
	$(ASM) $(BOOT_DIR)/boot-sect.asm -i $(BOOT_DIR) -f bin -o $(BUILD_DIR)/boot-sect.bin
	$(DD) if=/dev/zero of=$(BUILD_DIR)/boot-sect.bin seek=5 obs=1024 count=0

kernel: $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/kernel_entry.asm
	$(GCC) -ffreestanding -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o
	$(ASM) $(KERNEL_DIR)/kernel_entry.asm -f elf64 -o $(BUILD_DIR)/kernel_entry.o
	$(LINKER) -o $(BUILD_DIR)/kernel.bin -Ttext 0x1000 $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/kernel.o --oformat binary

image: boot kernel
	cat $(BUILD_DIR)/boot-sect.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os_image

run: default
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os_image

clean: 
	rm -rf $(BUILD_DIR)
