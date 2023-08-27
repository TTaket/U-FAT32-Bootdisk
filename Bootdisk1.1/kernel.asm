;也就是说 我们的主体还是汇编 c函数只是为了更方便的实现自定义函数
; global - 我们对外提供这个
; extern - 我们需要向外面找到这个
EXTERN DISPLAY            ;FROM C
[BITS 16]
[SECTION .TEXT]

GLOBAL _start             ;声明全局入口
GLOBAL MYPRINT            ;声明全局函数 可以被外界找到
_start:                   ;这里是函数入口
CALL DISPLAY              ;ASM CALL


MYPRINT:
    MOV AX, 0XB800
    MOV ES, AX
    MOV BYTE [ES: 0X00], 'O'
    MOV BYTE [ES: 0X01], 0X07
    MOV BYTE [ES: 0X02], 'K'
    MOV BYTE [ES: 0X03], 0X06
    MOV BYTE [ES: 0X04], 'C'
    MOV BYTE [ES: 0X05], 0X07
    MOV BYTE [ES: 0X06], '&'
    MOV BYTE [ES: 0X07], 0X06
    MOV BYTE [ES: 0X08], 'A'
    MOV BYTE [ES: 0X09], 0X07
    MOV BYTE [ES: 0X0a], 'S'
    MOV BYTE [ES: 0X0b], 0X06
    MOV BYTE [ES: 0X0c], 'M'
    MOV BYTE [ES: 0X0d], 0X07
    RET
