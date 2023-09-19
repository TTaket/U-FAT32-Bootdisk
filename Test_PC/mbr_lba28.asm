%include "var.asm"
org 0x7c00
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
mov fs, ax
mov sp, 0x7c00
mov ax, 0xb800
mov gs, ax

; clear screen
mov ax, 0x0600
mov bx, 0x0700
mov cx, 0
mov dx, 0x184f
int 0x10

; output
mov byte[gs:0x00], 'h'
mov byte[gs:0x02], 'i'


mov ax, LOADER_START_SECTOR ; 起始扇区lba地址, 0x1
mov bx, LOADER_BASE_ADDR      ; 写入的地址, 0x900
mov cx, 1                     ; 待读入的扇区数
call ReadDisk

jmp LOADER_BASE_ADDR          ; 进入0x900，即loader程序

%include "read_disk_lba28.asm"
; ReadDisk:
;     mov ah, 2h    ; int13h function 2
;     mov al, 1    ; we want to read 63 sectors
;     mov ch, 0     ; from cylinder number 0
;     mov cl, 2     ; the sector number 2 - second sector (starts from 1, not 0)
;     mov dh, 0     ; head number 0
; 	Mov dl , 80h
;     ; xor bx, bx    
;     ; mov es, bx    ; es should be 0
;     mov bx, LOADER_BASE_ADDR ; 512bytes from origin address 7c00h
;     int 13h
; 	ret



times 510 - ($-$$) db 0
db 0x55, 0xaa