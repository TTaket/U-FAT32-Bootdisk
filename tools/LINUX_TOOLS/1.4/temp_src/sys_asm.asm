global poweroff
global clearScreen
[BITS 16]

poweroff:
MOV AX, 5307H
MOV BX, 0001H
MOV CX, 0003H
INT 15H
RETF

;clear the screen
clearScreen:
PUSH AX
MOV AX, 0003H
INT 10H
POP AX
RETF