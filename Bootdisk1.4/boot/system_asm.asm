[BITS 16]

extern shell;
extern startup;
extern init_DBRinfo;
extern init_fsinfo_info;


GLOBAL _start

_start:
    ; ;扩大堆栈范围
    ; MOV AX , 5000H;
    ; MOV SS , AX;
    ; MOV AX , 9000H;
    ; MOV SP , AX;
    call dword CLEAR;
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
