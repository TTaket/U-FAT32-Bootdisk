;也就是说 我们的主体还是汇编 c函数只是为了更方便的实现自定义函数
; global - 我们对外提供这个
; extern - 我们需要向外面找到这个
extern DISPLAY            ;FROM C
[BITS 16]
section .text
GLOBAL _start             ;声明全局入口
_start:                   ;这里是函数入口
	CALL DISPLAY              ;ASM CALL
	RET


