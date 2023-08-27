
#路径定义
TOOL_PATH = ../tools/
ASSEMBLE  = ../tools/nasm/
DEBUG_PATH= ../tools/bochs/
BASE_PATH = ../

#行为定义
MAKE      = $(TOOL_PATH)make.exe -r
NASM      = $(ASSEMBLE)nasm.exe
DD        = $(TOOL_PATH)dd.exe
DEL       = del
COPY      = copy

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
	$(MAKE) Clean_file
	$(MAKE) NeedFile
	$(MAKE) Updata_Debug

To_Disk:
	$(MAKE) Clean_disk
	$(MAKE) Clean_file
	$(MAKE) NeedFile
	$(MAKE) Updata_Disk
#===============================


#=========1级行为===============
#形成局部功能 达到某一任务 调用二级行为
# 清除文件
Clean_file:
	$(DEL) *.bin
	
# 格式化硬盘
Clean_disk:
	$(MAKE) Clean_disk
	$(DD) if=Clean_disk.bin of=$(TAR_DISK) bs=512 count=200

# 格式化Debug镜像
Clean_Debug:
	$(DEL) ..\tools\bochs\c.img.lock
	$(MAKE) Clean_disk
	$(DD) if=Clean_disk.bin of=$(TAR_DEBUG) bs=512 count=200

# 导入磁盘
Updata_Disk: 
	$(DD) if=MBR.bin of=$(TAR_DISK) bs=512 count=1
	$(DD) if=Bootsect1.bin of=$(TAR_DISK) bs=512 count=1 seek=20
	$(DD) if=Bootsect2.bin of=$(TAR_DISK) bs=512 count=1 seek=50

# 导入DEBUG镜像
Updata_Debug:
	$(DD) if=MBR.bin of=$(TAR_DEBUG) bs=512 count=1
	$(DD) if=Bootsect1.bin of=$(TAR_DEBUG) bs=512 count=1 seek=20
	$(DD) if=Bootsect2.bin of=$(TAR_DEBUG) bs=512 count=1 seek=50

#生成需要文件：
NeedFile:
	$(MAKE) MBR	
	$(MAKE) Bootsect1
	$(MAKE) Bootsect2
#===============================

#=========2级行为===============
#对会被应用的命令单独创建 只调用三级行为
#生成对应盘块
Bootsect1:
	$(MAKE) Bootsect1.bin
	
Bootsect2:
	$(MAKE) Bootsect2.bin

MBR:
	$(MAKE) MBR.bin

Clean_disk:
	$(MAKE) Clean_disk.bin
#===============================


#=========3级行为===============
#生成对应的镜像文件 并不一定都会被调用
# 镜像文件生成
%.bin:%.asm
	$(NASM) $^ -o $@
#===============================



	


	









