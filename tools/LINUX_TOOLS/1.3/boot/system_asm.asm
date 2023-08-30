[BITS 16]

extern shell;
extern startup;
GLOBAL _start

_start:
    call CLEAR;
    call dword startup;
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
    RET;