
# 默认动作

default :
	../tools/make.exe boot
	../tools/make.exe updata_disk

# 其他指令
boot:
	../tools/make.exe -r boot.bin
	
# 导入
updata_disk:
	../tools/dd if=boot.bin of=../disk.vhd bs=512 count=1
	
# 镜像文件生成
boot.bin : boot.asm Makefile
	../tools/nasm/nasm.exe boot.asm -o boot.bin




