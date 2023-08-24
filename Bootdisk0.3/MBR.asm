LOADER_BASE_ADDR EQU 0x9000
LOADER_START_SECTOR EQU 0x1

TEXT_SHOW_POS EQU 0B800H
MBR_BASE_ADDR EQU 0x7c00

SECTION MBR VSTART=MBR_BASE_ADDR
    MOV AX , CS
    MOV DS , AX
    MOV ES , AX
    MOV SS , AX
    MOV FS , AX
    MOV SP , 0X7C00 ;初始化段寄存器 和 堆栈指针

    MOV AX , TEXT_SHOW_POS
    MOV GS , AX

    ;清屏
    MOV AX , 0x0600;
    MOV BX , 0x0700;
    MOV CX , 0;
    MOV DX , 184FH ; 因为VGA文本模式中，一行只能容纳80个字符,共25行。
			       ; 下标从0开始，所以0x18=24,0x4f=79
    INT 10H

    MOV BYTE [GS: 0X00], '*'
    MOV BYTE [GS: 0X01], 0XA4 ; A表示绿色背景闪烁，4表示前景色为红色

    MOV BYTE [GS: 0X02], '*'
    MOV BYTE [GS: 0X03], 0XA4

    MOV BYTE [GS: 0X04], 'M'
    MOV BYTE [GS: 0X05], 0XA4

    MOV BYTE [GS: 0X06], 'B'
    MOV BYTE [GS: 0X07], 0XA4

    MOV BYTE [GS: 0X08], 'R'
    MOV BYTE [GS: 0X09], 0XA4

    MOV BYTE [GS: 0X0a], '*'
    MOV BYTE [GS: 0X0b], 0XA4

    MOV BYTE [GS: 0X0c], '*'
    MOV BYTE [GS: 0X0d], 0XA4


    ;读取硬盘
    ;可以在这里改变为一个菜单 这样就支持多操作系统的选择
    MOV EAX ,LOADER_START_SECTOR;AX 放入LBA的逻辑扇区
    MOV BX ,LOADER_BASE_ADDR;BX 放入扇区拷贝到的内存地址
    MOV CX ,1;cx 放入读取扇区的数量

    CALL READ_DISK;
    JMP LOADER_BASE_ADDR;


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
        ; MOV DX , AX;
        ; MOV DI , BP;
        ; SHL ECX , 16;
        ; CALL .SHOW_COPYINFO
        ; SHR ECX , 16;
        ; MOV BP , DI;
        loop  read_info

    ;CALL .SHOW_SECTOR_CNT
    RET

    ; ;打印拷贝的信息
    ; .SHOW_COPYINFO:
    ;     CMP SI , 100
    ;     JNA .SHOW_COPYINFO.CONTINUE;
    ;     RET
    ;     .SHOW_COPYINFO.CONTINUE:


    ;     MOV BX , 0H;
    ;     .SHOW_COPYINFO.Lop:
    ;     MOV AX , DX;
    ;     MOV CL , 12;
    ;     SHR AX , CL;
    ;     CMP AL , 10;
    ;     JB .1
    ;     ADD AL, 7
    ;     .1:
    ;     ADD AL , '0'
    ;     MOV [.TMP_INFO + BX] , AL;
    ;     MOV CL , 4;
    ;     SHL DX , CL;
    ;     INC BX;
    ;     cmp BX , 5;
    ;     jnz .SHOW_COPYINFO.Lop

    ;     ;32/16 商16位 余数16位
    ;     MOV DX , 0;
    ;     MOV AX , SI;
    ;     MOV CX , 8;
    ;     DIV CX; AX 是商 DX 是 余数 -- DH放置行号 DL 放置列号
    ;     ADD AL , 3;假设商只存在于al里
    ;     MOV DH , AL; 行号
    ;     MOV AL , DL;
    ;     DEC AL;
    ;     MOV AH , 5;
    ;     MUL AH;
    ;     MOV DL , AL; 列号 6*5 = 30 < 255

    ;     MOV  BP,.TMP_INFO;
    ;     MOV  CX,5;
    ;     MOV  AX,01301H;
    ;     MOV  BX,000cH;
    ;     INT  10H;
    ;     RET
    ;     .TMP_INFO resq 5
    
    ;打印读取的块的个数
    .SHOW_SECTOR_CNT:
        MOV DI , 10000;被除数
        MOV BP , SI;
        MOV SI , 0;

        ;处理.SECTOR_CNT数组
        .SHOW_SECTOR_CNT.Lop:
        MOV DX , 0;
        MOV AX , BP;
        DIV DI;
        ADD [.SECTOR_CNT + SI] , AX;
        MOV BP , DX;

        MOV AX , DI;
        MOV DX , 0;
        MOV CX , 10;
        DIV CX;商位于AX中
        MOV DI , AX;

        INC SI;
        cmp SI , 5;
        jnz .SHOW_SECTOR_CNT.Lop

        MOV  BP,.INFO;
        MOV  CX,.INFO_LEN;
        MOV  AX,01301H;
        MOV  BX,000cH;
        MOV  DX,0100H;
        INT  10H;

        MOV  BP,.SECTOR_CNT;
        MOV  CX,5;
        MOV  AX,01301H;
        MOV  BX,000cH;
        MOV  DX,0200H;
        INT  10H;
        RET;
        .INFO: db "We Have Read Times : "
        .INFO_LEN equ $ - .INFO
        .SECTOR_CNT db "00000";
    


times 510-($-$$) db 0
db 0x55,0xaa