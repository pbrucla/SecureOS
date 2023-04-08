#!/bin/bash
nasm boot-sect.asm -f bin -o boot-sect.bin
dd if=/dev/zero of=boot-sect.bin seek=5 obs=1MB count=0
