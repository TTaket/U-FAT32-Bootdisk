#生成kernel.bin
default:
#生成临时文件夹
	rm ./temp_src -rf
	mkdir ./temp_src

	rm ./temp_res -rf
	mkdir ./temp_res

#拷贝文件进入到临时文件夹
	-cp ./boot/* ./temp_src
	-cp ./lib/lib_memroy/* ./temp_src
	-cp ./lib/lib_string/* ./temp_src

	
#生成需求文件
	nasm  -o ./temp_src/MBR.bin ./temp_src/MBR.asm
	nasm  -o ./temp_src/Bootsect.bin ./temp_src/Bootsect.asm

	nasm -f elf32 -o ./temp_src/system_asm.o ./temp_src/system_asm.asm
	nasm -f elf32 -o ./temp_src/stringio_asm.o ./temp_src/stringio_asm.asm
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -mpreferred-stack-boundary=2 -lgcc -shared   ./temp_src/system_c.c  -o ./temp_src/system_c.o 
	gcc -c -m16 -march=i386 -masm=intel -nostdlib -fno-pie -ffreestanding -mpreferred-stack-boundary=2 -lgcc -shared  ./temp_src/stringio_c.c -o ./temp_src/stringio_c.o

#连接出bin文件
	ld -m elf_i386 -s -Ttext 0x1000 ./temp_src/system_asm.o ./temp_src/system_c.o ./temp_src/stringio_asm.o ./temp_src/stringio_c.o
	objcopy -O binary a.out ./temp_src/system.bin

#转移
	mv ./temp_src/MBR.bin ./temp_res 
	mv ./temp_src/Bootsect.bin ./temp_res 
	mv ./temp_src/system.bin ./temp_res 
