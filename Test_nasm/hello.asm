section .data
	hello: db 'Hello nasm world!',10 
	helloLen: equ $-hello

section .text
	global _start

_start:
	mov eax,4; sys_write
	mov ebx,1; Fileid = 1  std_output
	mov ecx,hello; info
	mov edx,helloLen; len

	int 80h

	mov eax,1
	mov ebx,0
	int 80h
