;这个文件实现的是一些提供给c的库函数
GLOBAL printInPos
GLOBAL putchar
GLOBAL getch
[BITS 16]

;在指定位置打印字符串
;printInPos(char * msg,uint16_t len,uint8_t row,uint8_t col);
;采用在被调用端处理堆栈中的入参
printInPos:
    ;参数从右往左压入栈
    ;从左往右出栈
    ; MOV BP,AX; 导入字符串
    ; MOV  CX,WELCOME_INFO_Len
    ; MOV  AX,01301H;
    ; MOV  BX,000cH;
    ; MOV  DX,0000H;
    ; INT  10H;
    PUSHA;压入栈保护起来 16字节
    mov ebp , esp;利用esp来进行取参
    ;放入首地址
    add bp , 16 + 4 
    mov si , [bp];
    mov ax , cx; cx为0
    mov es , ax; es:bp放入首地址
    mov ds , ax; ds = es = cs 
    ;放入长度
    add bp , 4;
    mov cx , [bp];
    ;放入位置 dx
    add bp , 4;
    mov dh , [bp];
    add bp , 4;
    mov dl , [bp];
    ;放入bx
    mov bx , 0007H;
    ;放入ax
    mov ax , 01301H;
    ;放入bp
    mov bp , si;
    INT 10H;
    POPA;
    ;retf 16
    retf;c 的函数结构里面已经包含了入参的清除 不需要手动清除

    


;在光标位置打印字符串
;void  putchar (char*c);
 putchar:
    PUSHA                     ;Protect the scene
    MOV BP, SP                ;Save the top of the stack
    ADD BP, 16 + 4            ;BP = 第一个参数的地址
    MOV AL, [BP]              ;AL = Print characters
    MOV BH, 0                 ;BH = page number
    MOV AH, 0EH               ;function number
    INT 10H
    POPA
    RETF;


;读入一个字符
;char  getch();
;al 为返回值char
getch:
    MOV AH, 0                 ;Function number
    INT 16H
    MOV AH, 0                 ;Read characters, AL = characters
    RETF