BOOTSECT_BASE_ADDR EQU 0X9000         	;操作系统应该被加载到的地址
SYSTEM_BASE_ADDR EQU 0x1000

SECTION BOOTSECT VSTART=BOOTSECT_BASE_ADDR
    ;CALL CLEAR
    MOV AX , CS;
    MOV DS , AX;
    MOV ES , AX;
    CALL CLEAR
    CALL SHOW_HELLO
    ;进入到保护模式 退出保护模式
    

    ;引导更多扇区进入内存
    pop AX;
    MOV CX , 16;
    SHL AX , CL;
    pop AX;
    ;EAX是lba逻辑扇区号
    ADD EAX , 1;自身占了一个扇区 把除自身外其他的扇区拷贝到system指定位置
    MOV BX ,SYSTEM_BASE_ADDR;BX 放入扇区拷贝到的内存地址
    MOV CX ,10;cx 放入读取扇区的数量 读取多个扇区

    CALL READ_DISK;
    JMP SYSTEM_BASE_ADDR;

SHOW_HELLO:
    MOV AX,WELCOME_INFO
    MOV BP,AX; 导入字符串
    MOV  CX,WELCOME_INFO_Len
    MOV  AX,01301H;
    MOV  BX,000cH;
    MOV  DX,0000H;
    INT  10H;
    RET;

CLEAR:
    MOV AX,0600H
    MOV BX,00700H
    MOV CX,0H;
    MOV DX,184FH
    INT 10H
    RET;

WELCOME_INFO: db "THIS IS Bootsect1"
WELCOME_INFO_Len equ $ - WELCOME_INFO

READ_DISK:
    ;因为EAX的值和CX 会经常发生变化所以先保护起来
    MOV ESI , EAX;
    MOV DI , CX;
    MOV BP , BX

    MOV DX ,0X1F2;读入个数
    MOV AL , CL;
    OUT DX , AL;


    MOV EAX , ESI;
    MOV DX , 0X1F3;放入低8位
    OUT DX , AL;

    MOV CL , 8;
    SHR EAX , CL;
    MOV DX , 0X1F4;放入中间8位
    OUT DX , AL;

    SHR EAX , CL;
    AND AL, 0X0F
    MOV DX , 0X1F5;放入高8位
    OUT DX , AL;

    SHR EAX , CL;
    AND AL , 0X0F;
    OR  AL , 0XE0;
    MOV DX , 0X1F6;准备好低四位和一些标志
    OUT DX , AL;

    MOV DX , 0X1F7;发出读写命令
    MOV AX , 0X20;
    OUT DX , AL;
    ;读取扇区:0x20
    ;写入扇区:0x30
    ;磁盘识别:0xEC

    Check_status:;检查磁盘状态
        NOP
        IN  AL , DX                 ;DX = 001F7H
        AND AL , 0X88			;第4位为1表示硬盘准备好数据传输，第7位为1表示硬盘忙
        CMP AL , 0X08
        JNZ Check_status

    ;准备完成开始读取
    ;计算循环次数 扇区个数在di里面 一次能读出两个字节 512/2 * di 是 16位乘8位 答案32位在DX : AX;
    MOV AX , DI;
    MOV EBX , 256;
    MUL BX;
    MOV cl , 10H;
    SHL EDX , cl;
    MOV ECX , EDX;
    ADD CX , AX;


    MOV SI , 0H;
    read_info:
        MOV DX , 001F0H;
        IN  AX , DX;
        MOV [BP] ,AX;
        ADD BP , 2
        INC SI;
        loop  read_info

    RET





times 510-($-$$) db 0
db 0x55,0xaa