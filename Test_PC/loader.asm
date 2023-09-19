%include "var.asm"
org LOADER_BASE_ADDR

; output
mov byte [gs:0x00], 'L'
mov byte [gs:0x02], 'O'
mov byte [gs:0x04], 'A'
mov byte [gs:0x06], 'D'
mov byte [gs:0x08], 'E'
mov byte [gs:0x0A], 'R'

; 程序挂起
func:
  hlt
  jmp func

times 510-($-$$) db 0
db 0x55,0xAA
