
# 默认动作
default :
	../tools/make.exe To_Clean_disk
	../tools/make.exe To_Clean_file
	../tools/make.exe Bootsect1.bin 
	../tools/make.exe Bootsect2.bin 
	../tools/make.exe MBR	
	../tools/make.exe updata_disk


# 清除文件
To_Clean_file:
	del Clean_disk.bin
	del Bootsect1.bin 
	del Bootsect2.bin 
	del MBR.bin
	

# 格式化硬盘
To_Clean_disk:
	../tools/make.exe Clean_disk
	../tools/dd if=Clean_disk.bin of=../disk.vhd bs=512 count=200

Bochs_Debug:
	del ..\tools\bochs\c.img.lock
	../tools/make.exe Clean_disk
	../tools/dd if=Clean_disk.bin of=../tools/bochs/c.img bs=512 count=200
	../tools/make.exe To_Clean_file
	../tools/make.exe MBR	
	../tools/make.exe Bootsect1.bin 
	../tools/make.exe Bootsect2.bin 
	../tools/dd if=MBR.bin of=../tools/bochs/c.img bs=512 count=1
	../tools/dd if=Bootsect1.bin of=../tools/bochs/c.img bs=512 count=1 seek=20
	../tools/dd if=Bootsect2.bin of=../tools/bochs/c.img bs=512 count=1 seek=50

#生成对应盘块
Bootsect1:
	../tools/make.exe -r Bootsect1.bin
	
Bootsect2:
	../tools/make.exe -r Bootsect2.bin

MBR:
	../tools/make.exe -r MBR.bin

Clean_disk:
	../tools/make.exe -r Clean_disk.bin
	

# 导入
updata_disk: 
	../tools/dd if=MBR.bin of=../disk.vhd bs=512 count=1
	../tools/dd if=Bootsect1.bin of=../disk.vhd bs=512 count=1 seek=20
	../tools/dd if=Bootsect2.bin of=../disk.vhd bs=512 count=1 seek=50
	
# Bootsect1.生成
Bootsect1.bin : Bootsect1.asm Makefile
	../tools/nasm/nasm.exe Bootsect1.asm -o Bootsect1.bin

# Bootsect2生成
Bootsect2.bin : Bootsect2.asm Makefile
	../tools/nasm/nasm.exe Bootsect2.asm -o Bootsect2.bin

# MBR生成
MBR.bin :  MBR.asm Makefile
	../tools/nasm/nasm.exe MBR.asm -o MBR.bin

Clean_disk.bin: Clean_disk.bin Makefile
	../tools/nasm/nasm.exe Clean_disk.asm -o Clean_disk.bin






