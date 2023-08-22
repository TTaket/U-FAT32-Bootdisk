ORG 07C00H ;指示位置
MOV AX , CS;
MOV DS , AX;
MOV ES , AX;
CALL SHOW;
JMP $;

SHOW:
    MOV AX,MESSAGE
    MOV BP,AX; 导入字符串
    MOV CX,len_MESSAGE
    MOV AX,01301H;
    MOV BX,000cH;
    MOV DL,0H;
    INT 10H;
    RET;


MESSAGE: DB "HELLO, THIS IS OS WORLD"
len_MESSAGE  equ  $ - MESSAGE 
TIMES 510 - ($ - $$)  DB 0

DW 0xAA55