
# 默认动作
default :
	../tools/make.exe To_Clean_disk
	../tools/make.exe To_Clean_file
	../tools/make.exe Loader
	../tools/make.exe MBR	
	../tools/make.exe updata_disk


# 清除文件
To_Clean_file:
	del Clean_disk.bin
	del Loader.bin 
	del MBR.bin
	

# 格式化硬盘
To_Clean_disk:
	../tools/make.exe Clean_disk
	../tools/dd if=Clean_disk.bin of=../disk.vhd bs=512 count=1
	../tools/dd if=Clean_disk.bin of=../disk.vhd bs=512 count=1 seek=1
	../tools/dd if=Clean_disk.bin of=../disk.vhd bs=512 count=1 seek=2
	../tools/dd if=Clean_disk.bin of=../disk.vhd bs=512 count=1 seek=3

Bochs_Debug:
	del ..\tools\bochs\c.img.lock
	../tools/make.exe Clean_disk
	../tools/dd if=Clean_disk.bin of=../tools/bochs/c.img bs=512 count=1
	../tools/dd if=Clean_disk.bin of=../tools/bochs/c.img bs=512 count=1 seek=1
	../tools/dd if=Clean_disk.bin of=../tools/bochs/c.img bs=512 count=1 seek=2
	../tools/dd if=Clean_disk.bin of=../tools/bochs/c.img bs=512 count=1 seek=3
	../tools/dd if=MBR.bin of=../tools/bochs/c.img bs=512 count=1
	../tools/dd if=Loader.bin of=../tools/bochs/c.img bs=512 count=1 seek=1

#生成对应盘块
Loader:
	../tools/make.exe -r Loader.bin

MBR:
	../tools/make.exe -r MBR.bin

Clean_disk:
	../tools/make.exe -r Clean_disk.bin
	

# 导入
updata_disk: 
	../tools/dd if=MBR.bin of=../disk.vhd bs=512 count=1
	../tools/dd if=Loader.bin of=../disk.vhd bs=512 count=1 seek=1
	
# Looder生成
Loader.bin : Loader.asm Makefile
	../tools/nasm/nasm.exe Loader.asm -o Loader.bin

# MBR生成
MBR.bin :  MBR.asm Makefile
	../tools/nasm/nasm.exe MBR.asm -o MBR.bin

Clean_disk.bin:
	../tools/nasm/nasm.exe Clean_disk.asm -o Clean_disk.bin






