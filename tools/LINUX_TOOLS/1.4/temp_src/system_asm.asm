[BITS 16]

extern shell;
extern startup;
extern init;
extern init_DBRinfo;
extern init_fsinfo_info;

extern DBR_pos
extern fsinfo_pos
extern FAT1_pos
extern FAT2_pos
extern DATA_pos
extern file_info_len
extern end_clu
extern fat_clunum
extern clunum_fileinfo
extern Maxclunum
extern clunum_byte
extern clunum_available
extern clunum_next
extern MAX_BUFFER
extern DBR_info
extern fsinfo_info
extern global_path

GLOBAL _start

_start:
    call dword CLEAR;
    call dword init;
    call dword startup;
    call dword init_DBRinfo
    call dword init_fsinfo_info
Key_Board:
    MOV AH , 0;
    INT 16H;
    CMP AL , 0DH;
    JNZ Key_Board

    call dword shell;
    jmp Key_Board;

CLEAR:
    MOV AX,0600H
    MOV BX,00700H
    MOV CX,0H;
    MOV DX,184FH
    INT 10H
    RETF;
