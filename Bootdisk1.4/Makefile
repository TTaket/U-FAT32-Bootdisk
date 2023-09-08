
#路径定义
TOOL_PATH = ../tools/
ASSEMBLE  = ../tools/nasm/
DEBUG_PATH= ../tools/bochs/
BASE_PATH = ../

#行为定义
MAKE      = $(TOOL_PATH)make
NASM      = $(ASSEMBLE)nasm
DD        = $(TOOL_PATH)dd
LD        = $(TOOL_PATH)ld
OBJCOPY   = $(TOOL_PATH)objcopy
DEL       = del
COPY      = copy

#标志定义
GCC_FLAG  = -m32 -fno-pie
NASM_FLAG = -f elf32
LD_FLAG   = -m i386pe -e _start -s -Ttext 0x1500
OBJCOPY_FLAG = -O binary

#目标定义 
TAR_DEBUG = $(DEBUG_PATH)c.img
TAR_DISK = $(BASE_PATH)disk.vhd


# 默认动作
default :
	$(MAKE) To_Disk

#=========0级行为===============
#用户直接调用 顶级行为 调用1级行为
To_Debug:
	$(MAKE) Clean_Debug
#	$(MAKE) Clean_file
#   $(MAKE) NeedFile 这部分工作由linux makefile 完成
	$(MAKE) Updata_Debug

To_Disk:
	$(MAKE) Clean_disk
#	$(MAKE) Clean_file
#   $(MAKE) NeedFile 这部分工作由linux makefile 完成
	$(MAKE) Updata_Disk
#===============================


#=========1级行为===============
#形成局部功能 达到某一任务 调用二级行为
#生成需要文件：

# 清除文件
Clean_file:
	-$(DEL) *.bin
	-$(DEL) *.o
	-$(DEL) *.out
	
# 格式化硬盘
Clean_disk:
	$(MAKE) Clean_disk.bin
	$(DD) if=Clean_disk.bin of=$(TAR_DISK) bs=512 count=200

# 格式化Debug镜像
Clean_Debug:
	$(DEL) ..\tools\bochs\c.img.lock
	$(MAKE) Clean_disk.bin
	$(DD) if=Clean_disk.bin of=$(TAR_DEBUG) bs=512 count=200

# 导入磁盘
Updata_Disk: 
	$(DD) if=MBR.bin of=$(TAR_DISK) bs=512 count=1
	$(DD) if=Bootsect.bin of=$(TAR_DISK) bs=512 count=50 seek=20
	$(DD) if=system.bin of=$(TAR_DISK) bs=512 count=50 seek=70
#$(DD) if=BOOT.img of=$(TAR_DISK) bs=512 count=100000 seek=1024

# 导入DEBUG镜像
Updata_Debug:
	$(DD) if=MBR.bin of=$(TAR_DEBUG) bs=512 count=1
	$(DD) if=Bootsect.bin of=$(TAR_DEBUG) bs=512 count=50 seek=20
	$(DD) if=system.bin of=$(TAR_DEBUG) bs=512 count=50 seek=70
#$(DD) if=BOOT.img of=$(TAR_DEBUG) bs=512 count=100000 seek=1024
#===============================

#=========2级行为===============
#生成对应的镜像文件 并不一定都会被调用
# 镜像文件生成
%.bin:%.asm
	$(NASM) $^ -o $@
%.o:%.asm
	$(NASM) $(NASM_FLAG) -o $@ $^
%.o:%.c
	gcc $(GCC_FLAG) -c $^ -o $@ 
#===============================



	


	









