#生成kernel.bin
default:
	nasm -f elf32 -o Bootsect1.o Bootsect1.asm 
	nasm -f elf32 -o stringio_asm.o stringio_asm.asm
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -mpreferred-stack-boundary=2 -lgcc -shared   Kernel_c.c  -o Kernel_c.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -mpreferred-stack-boundary=2 -lgcc -shared  stringio_c.c -o stringio_c.o 
	ld -m elf_i386 -s -Ttext 0x8000 Bootsect1.o stringio_asm.o stringio_c.o Kernel_c.o
	objcopy -O binary a.out kernel.bin
