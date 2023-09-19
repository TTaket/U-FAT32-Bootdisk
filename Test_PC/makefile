CHS:
	make Make_CHS
	make Run_CHS

Make_CHS:
	nasm -f bin mbr_chs.asm
	nasm loader.asm
	cat mbr_chs loader > os_chs.img


Run_CHS:
	sudo qemu-system-x86_64 -drive file=os_chs.img,format=raw


LBA:
	make Make_LBA
	make Run_LBA

Make_LBA:
	nasm -f bin mbr_lba28.asm
	nasm loader.asm
	cat mbr_lba28 loader > os_lba28.img


Run_LBA:
	sudo qemu-system-x86_64 -drive file=os_lba28.img,format=raw


Clean:
	rm *.img
	rm loader mbr_chs mbr_lba28