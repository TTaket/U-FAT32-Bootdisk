;Bootsect相关
BOOTSECT_BASE_ADDR EQU 0x9000;BOOTSECT 放入内存的地址

;分区表相关
DISK_PARTITIONED_TABLE_BASE_ADDR EQU 0x7DBE
DISK_PARTITIONED_TABLE_BASE_ADDR0 EQU 0x7DBE
DISK_PARTITIONED_TABLE_BASE_ADDR1 EQU 0x7DCE
DISK_PARTITIONED_TABLE_BASE_ADDR2 EQU 0x7DDE
DISK_PARTITIONED_TABLE_BASE_ADDR3 EQU 0x7DEE
DISK_PARTITIONED_TABLE_UNIT_LEN EQU 16

;硬盘相关
;硬盘数据应该由系统调用获得 把硬盘数据提供给操作
;这里可以直接写死 系统调用获得是操作系统的事情 MBR只负责引导
Cylinder_NUM EQU 3788
HEADS_NUM    EQU 255
SECTOR_NUM   EQU 63


;杂项
TEXT_SHOW_POS EQU 0B800H ;显示适配器 文字模式存放地址
MBR_BASE_ADDR EQU 0x7c00 ;MBR基址



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

    ; MOV BYTE [GS: 0X00], '*'
    ; MOV BYTE [GS: 0X01], 0XA4 ; A表示绿色背景闪烁，4表示前景色为红色

    ; MOV BYTE [GS: 0X02], '*'
    ; MOV BYTE [GS: 0X03], 0XA4

    MOV BYTE [GS: 0X04], 'M'
    MOV BYTE [GS: 0X05], 0XA4

    MOV BYTE [GS: 0X06], 'B'
    MOV BYTE [GS: 0X07], 0XA4

    MOV BYTE [GS: 0X08], 'R'
    MOV BYTE [GS: 0X09], 0XA4

    ; MOV BYTE [GS: 0X0a], '*'
    ; MOV BYTE [GS: 0X0b], 0XA4

    ; MOV BYTE [GS: 0X0c], '*'
    ; MOV BYTE [GS: 0X0d], 0XA4


    ;读取硬盘
    ;跳转逻辑更改：寻找活动分区并且跳转到活动分区
    CALL TO_FIND_ACT;
    CALL GET_LBA_START_SECTOR;
    MOV CX , 16;
    ROR AX , CL; 
    push AX;高16位
    ROL AX , 16;
    push AX;低16位
    ;用这种方式 把逻辑扇区传递给下一个模块
    
    MOV BX ,BOOTSECT_BASE_ADDR;BX 放入扇区拷贝到的内存地址
    MOV CX ,10;cx 放入读取扇区的数量 读取多个扇区

    CALL READ_DISK;
    JMP BOOTSECT_BASE_ADDR;
;========================================================
;   名称：
;       Delay
;   功能：
;       延时
;========================================================    

DELAY:
    MOV CX , 0xFF;
    .0:
    MOV AX , 0xEFFF;
    .1:
        DEC Ax;
        jnz .1
    DEC CX;
    jnz .0
    ret

;========================================================
;   名称：
;       FIND_ACT
;   功能：
;       找到活动分区 并且加载到PART_ID 里面
;   入参：
;       无
;   修改：
;       AX BX CX BP SS
;   返回值：
;========================================================
TO_FIND_ACT:
FIND_ACT_INIT:
    MOV CX , 0;
FIND_ACT:
    MOV AX , CX;
    MOV BL , DISK_PARTITIONED_TABLE_UNIT_LEN;
    MUL BL
    MOV BP , DPT0;
    ADD BP , AX;
    MOV AX , [BP];
    CMP AL , 80H
    JZ FIND_ACT_END
    INC CX;
    CMP CX , 4
    JZ DEAD_COMPUTER
    JMP FIND_ACT
FIND_ACT_END:
    MOV [PART_ID] ,CX ;把活动分区装载进来
    MOV [ACTPART_ADDR] , BP ;地址也同时记录
    RET


;========================================================
;   名称：
;       GET_LBA_START_SECTOR
;   功能：
;       活动分区的起始LBA放入到EAX里面
;   入参：
;       无
;   修改：
;       AX BX CX
;   返回值：
;       EAX LBA起始扇区
;========================================================
GET_LBA_START_SECTOR:
    MOV DI , 0H;清零
    MOV BP , [ACTPART_ADDR]
    MOV DL , [BP + 2]
    AND DX , 00111111B
    ADD DI , DX;扇区号
    MOV CL , SECTOR_NUM;
    MOV AL , [BP + 1]
    MUL CL ;
    ADD DI , AX;磁头加入
    MOV AL , SECTOR_NUM
    MOV CL , HEADS_NUM
    MUL CL; AX存的是HEADS_NUM * SECTOR_NUM
    MOV DH , 0H;
    MOV DL , [BP + 2];
    MOV CX , 2;
    SHL DX , CL;
    MOV DL , [BP + 3];DX存的是柱面号
    MOV CX , DX;
    MUL CX; DX存的是高16位 ax低16位
    MOV BX , AX;
    MOV AX , DX;
    MOV CX , 10H;
    SHL AX , CL;
    MOV AX , BX;
    MOV ECX , 0;
    MOV CX , DI;
    ADD EAX,ECX; 添加柱面号
    RET;

;========================================================
;   名称：
;       DEAD_COMPUTER
;   功能：
;       死机
;   入参：
;   修改：
;   返回值：
;========================================================
DEAD_COMPUTER:
    JMP $;死循环 后续可以补充错误码

;========================================================
;   名称：
;       READ_DISK
;   功能：
;       把指定位置开始的指定个扇区读入指定内存位置
;   入参：
;       EAX: LBA的逻辑扇区编号
;       BX : 拷贝到本地的内存地址
;       CX : 读入扇区的数量
;   修改：
;       AX BX CX DX SI DI BP
;   返回值：
;========================================================
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

    CALL .SHOW_SECTOR_CNT
    RET


;========================================================
;   名称：
;       .SHOW_SECTOR_CNT
;   功能：
;       打印读取的扇区的个数
;   入参：
;       SI: 参数个数
;   修改：
;       AX BX CX DX SI DI BP
;   返回值：
;========================================================
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

        ; MOV  BP,.INFO;
        ; MOV  CX,.INFO_LEN;
        ; MOV  AX,01301H;
        ; MOV  BX,000cH;
        ; MOV  DX,0100H;
        ; INT  10H;

        MOV  BP,.SECTOR_CNT;
        MOV  CX,5;
        MOV  AX,01301H;
        MOV  BX,000cH;
        MOV  DX,0200H;
        INT  10H;
        RET;


        ;.INFO: db "We Have Read Times : "
        ;.INFO_LEN equ $ - .INFO
        .SECTOR_CNT db "00000";
        PART_ID db "0"
        ACTPART_ADDR db "00"

; 打印字符 把al 打印到屏幕上 
; ; 输入：AL = 要打印的字节
;     ; 输出：无

;     push ax
;     push bx
;     push cx
;     push dx

;     mov bx, ax          ; 备份AX到BX
;     shr al, 4            ; 取高4位
;     call HexToASCII     ; 转换为ASCII字符
;     mov ah, 0x0E        ; AH = 0x0E，显示字符的BIOS功能号
;     mov bh, 0x00        ; 页号为0，通常是文本模式的页号
;     mov cx, 0x0001      ; 打印字符的次数，这里是1
;     mov dl, al          ; 要打印的字符
;     int 0x10            ; 调用BIOS中断显示字符

;     mov al, bl          ; 恢复备份的AL
;     and al, 0x0F        ; 取低4位
;     call HexToASCII     ; 转换为ASCII字符
;     mov dl, al          ; 要打印的字符
;     int 0x10            ; 调用BIOS中断显示字符

;     pop dx
;     pop cx
;     pop bx
;     pop ax
;     ret

; HexToASCII:
;     ; 输入：AL = 十六进制数字（0-15）
;     ; 输出：AL = ASCII字符

;     cmp al, 9
;     jbe .is_digit
;     add al, 7
; .is_digit:
;     add al, '0'
;     ret

;分区表
;第1字节 引导标志。若值为80H表示活动分区，若值为00H表示非活动分区
;第2、3、4字节 本分区的起始磁头号、扇区号、柱面号。
;   磁头号——第2字节；
;   扇区号——第3字节的低6位；
;   柱面号——为第3字节高2位+第4字节8位。
;第5字节 分区类型符。
;   00H——表示该分区未用（即没有指定）；
;   06H——FAT16基本分区；
;   0BH——FAT32基本分区；
;   05H——扩展分区；
;   07H——NTFS分区；
;   0FH——（LBA模式）扩展分区（83H为Linux分区等）。
;第6、7、8字节 本分区的结束磁头号、扇区号、柱面号。
;   磁头号——第6字节
;   扇区号——第7字节的低6位
;   柱面号——为第7字节高2位+第8字节8位。
;第9、10、11、12字节 逻辑起始扇区号
;第13、14、15、16字节 本分区的总扇区数。

;当前分区表
;    指定第一分区为主分区-活动分区 扇区数量100个 起始扇区是从逻辑扇区20开始
;    指定第二分区为主分区 扇区数量100个 起始扇区是从逻辑扇区120开始
times 446 -($-$$) db 0
DISK_PARTITIONED_TABLE0:
    DPT0 db 0x80,0x00,0x14,0x00,0x0B,0x01,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x24
DISK_PARTITIONED_TABLE1:
    DPT1 db 0x00,0x01,0x39,0x00,0x0B,0x03,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x24
DISK_PARTITIONED_TABLE2:
    DPT2 db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
DISK_PARTITIONED_TABLE3:
    DPT3 db 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

;结束标志符
times 510-($-$$) db 0
db 0x55,0xaa


80 
20 21 00 
0C 
FE FF FF 
00 08 00 00 
20 B9 A0 03