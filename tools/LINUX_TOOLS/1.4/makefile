#生成kernel.bin
default:
#生成临时文件夹
	rm ./temp_src -rf
	mkdir ./temp_src

	rm ./temp_res -rf
	mkdir ./temp_res

#拷贝文件进入到临时文件夹
	-cp ./boot/* ./temp_src
	-cp ./lib/lib_brk/* ./temp_src
	-cp ./lib/lib_cmd/* ./temp_src
	-cp ./lib/lib_file/* ./temp_src
	-cp ./lib/lib_math/* ./temp_src
	-cp ./lib/lib_string/* ./temp_src
	-cp ./lib/lib_sys/* ./temp_src


#生成需求文件
	nasm  -o ./temp_src/MBR.bin ./temp_src/MBR.asm
	nasm  -o ./temp_src/Bootsect.bin ./temp_src/Bootsect.asm

	nasm -f elf32 -o ./temp_src/system_asm.o ./temp_src/system_asm.asm
	nasm -f elf32 -o ./temp_src/brk_asm.o ./temp_src/brk_asm.asm
	nasm -f elf32 -o ./temp_src/stringio_asm.o ./temp_src/stringio_asm.asm
	nasm -f elf32 -o ./temp_src/sys_asm.o ./temp_src/sys_asm.asm

	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/system_c.c  -o ./temp_src/system_c.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/brk_c.c  -o ./temp_src/brk_c.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/sys_c.c  -o ./temp_src/sys_c.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/cd.c  -o ./temp_src/cd.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/ls.c  -o ./temp_src/ls.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/mkdir.c  -o ./temp_src/mkdir.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/touch.c  -o ./temp_src/touch.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared   ./temp_src/fat32.c  -o ./temp_src/fat32.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared  ./temp_src/math.c -o ./temp_src/math.o
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -lgcc -shared  ./temp_src/stringio_c.c -o ./temp_src/stringio_c.o

#连接出bin文件
	ld -m elf_i386 -s -Ttext 0x1000 ./temp_src/system_asm.o ./temp_src/brk_asm.o ./temp_src/system_c.o ./temp_src/sys_c.o  ./temp_src/sys_asm.o ./temp_src/stringio_asm.o ./temp_src/stringio_c.o ./temp_src/brk_c.o ./temp_src/cd.o ./temp_src/ls.o ./temp_src/mkdir.o ./temp_src/touch.o ./temp_src/fat32.o ./temp_src/math.o     
	objcopy -O binary a.out ./temp_src/system.bin

#转移
	mv ./temp_src/MBR.bin ./temp_res 
	mv ./temp_src/Bootsect.bin ./temp_res 
	mv ./temp_src/system.bin ./temp_res 
