ASM=nasm
DD=dd
CFLAGS=-I.
QEMU=qemu-system-x86_64
BUILD_DIR=build
SRC_DIR=source

default: source/*.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/boot-sect.asm -i $(SRC_DIR) -f bin -o $(BUILD_DIR)/boot-sect.bin
	$(DD) if=/dev/zero of=$(BUILD_DIR)/boot-sect.bin seek=5 obs=1024 count=0

run: default
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/boot-sect.bin

clean: 
	rm -rf $(BUILD_DIR)