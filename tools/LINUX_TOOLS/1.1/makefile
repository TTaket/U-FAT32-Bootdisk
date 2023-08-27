#生成kernel.bin
default:
	nasm -f elf32 -o kernel_asm.o kernel_asm.asm 
	nasm -f elf32 -o kernel_asm2.o kernel_asm2.asm
	gcc -m32 -fno-pie -o kernel_c.o -c kernel_c.c
	ld -m elf_i386 -s -Ttext 0x1500 kernel_asm.o kernel_asm2.o kernel_c.o
	objcopy -O binary a.out kernel.bin