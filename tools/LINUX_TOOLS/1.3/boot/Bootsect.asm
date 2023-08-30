BOOTSECT_BASE_ADDR EQU 0X9000         	;操作系统应该被加载到的地址
SYSTEM_BASE_ADDR EQU 0x1000


;保护模式和GDT 相关
;首先对 TYPE属性进行定义 
DA_32       EQU 4000H             	;32位
DA_DR		EQU	90h	; 存在的只读数据段类型值
DA_DRW		EQU	92h	; 存在的可读写数据段属性值
DA_DRWA		EQU	93h	; 存在的可读写数据段属性值
DA_C		EQU	98h	; 存在的只执行代码段属性值
DA_CR		EQU	9Ah	; 存在的可执行可读代码段属性值
DA_CCO		EQU	9Ch	; 存在的只执行一致代码段属性值
DA_CCOR		EQU	9Eh	; 存在的可执行可读一致代码段属性值
;保护模式的段定义暂时不是特别了解 目前仅进入保护模式的演示

; 宏 ------------------------------------------------------------------------------------------------------
;
; 描述符
; 传入参数： 传入基址 长度 属性
;        基址 32位
;        界限 20位
;        属性 给出16位
;属性中间是镂空的 放置着段界限的高四位
%macro DESCRIPTOR 3
	dw	%2 & 0FFFFh				; 段界限 1 低16位				(2 字节)
	dw	%1 & 0FFFFh				; 段基址 1 低16位			(2 字节)
	db	(%1 >> 16) & 0FFh			; 段基址 2 中间8位				(1 字节) 
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	; 属性 + 段界限 高8位  属性从中间被切开(2 字节)
	db	(%1 >> 24) & 0FFh			; 段基址 高8位				(1 字节)
%endmacro
    
    
[SECTION .BOOTSECT]
[BITS 16]
SECTION BOOTSECT VSTART=BOOTSECT_BASE_ADDR
    ORG 0X9000
    CALL CLEAR
    CALL SHOW_HELLO
    ;引导更多扇区进入内存
    pop AX;
    MOV CX , 16;
    SHL AX , CL;
    pop AX;
    ;EAX是lba逻辑扇区号
    ADD EAX , 10;自身占了一个扇区 把除自身外其他的扇区拷贝到system指定位置
    MOV BX ,SYSTEM_BASE_ADDR;BX 放入扇区拷贝到的内存地址
    MOV CX ,10;cx 放入读取扇区的数量 读取多个扇区
    CALL READ_DISK;
;进入到保护模式 其中有退出保护模式
    JMP DWORD ENTER_PM;

;返回到了16位实模式
BACK_ENTRY_SEGMENT:
    MOV AX, CX;
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0x7c00;

    ;关闭A20
    IN AL, 92H
    AND AL, 11111101B ;关闭第二十根
    OUT 92H, AL

    ;STI

    CALL CLEAR
    CALL SHOW_TUCHU
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
SHOW_TUCHU:
    MOV AX,EXIT_PM_INFO
    MOV BP,AX; 导入字符串
    MOV  CX,EXIT_PM_INFO_Len
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
EXIT_PM_INFO: db "COME BACK 16"
EXIT_PM_INFO_Len equ $ - EXIT_PM_INFO

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

    RET



[SECTION .GDT]
;GDT
PM_GDT          : DESCRIPTOR 0, 0, 0
PM_DESC_CODE32  : DESCRIPTOR 0, SEGCODE32_LEN -1, DA_C+DA_32
PM_DESC_DATA    : DESCRIPTOR 0, DATALEN-1, DA_DRW
PM_DESC_STACK   : DESCRIPTOR 0, TOP_OF_STACK, DA_DRWA+DA_32
PM_DESC_VIDEO   : DESCRIPTOR 0B8000H, 0FFFFH, DA_DRW
;刷新其余段寄存器
UPDATE_DESC     : DESCRIPTOR 0, 0xFFFF,DA_DRW
;段界限必须设置为 0xffff 刷新cs区的段界限
PM_DESC_CODE32_CLOSE  : DESCRIPTOR 0, 0xFFFF, DA_C
;段基址大多数填的都是0 因为后续有初始化 段界限现在就确定了下来 属性也确定了
GDTLEN  EQU $ - PM_GDT
GDTPTR  DW GDTLEN - 1
        DD 0                  	; GDT 基地址 初始化的时候补充GDT的基址32位


;GDT 选择子
;段选择子在GDT表内的偏移
SELECTOER_CODE32    EQU     PM_DESC_CODE32 - PM_GDT
SELECTOER_DATA      EQU     PM_DESC_DATA - PM_GDT
SELECTOER_STACK     EQU     PM_DESC_STACK - PM_GDT
SELECTOER_VIDEO     EQU     PM_DESC_VIDEO - PM_GDT
SELECTOER_CODE32_CLOSE EQU  PM_DESC_CODE32_CLOSE - PM_GDT
SELECTOER_UPDATE_DESC  EQU  UPDATE_DESC - PM_GDT


[SECTION .PM]
ENTER_PM:
[BITS 16]
    MOV AX, CS
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0100H

    ;初始化32位的代码段
    XOR EAX, EAX
    MOV AX, CS
    SHL EAX, 4
    ADD EAX, PM_CODE
    MOV WORD[PM_DESC_CODE32+2], AX
    SHR EAX, 16
    MOV BYTE [PM_DESC_CODE32+4], AL
    MOV BYTE [PM_DESC_CODE32+7], AH

    ;初始化32位的回到实模式
    XOR EAX, EAX
    MOV AX, CS
    SHL EAX, 4
    ADD EAX, PM_CODE_CLOSE
    MOV WORD[PM_DESC_CODE32_CLOSE+2], AX
    SHR EAX, 16
    MOV BYTE [PM_DESC_CODE32_CLOSE+4], AL
    MOV BYTE [PM_DESC_CODE32_CLOSE+7], AH


    ;初始化32位的数据段
    XOR EAX, EAX
    MOV AX, DS
    SHL EAX, 4
    ADD EAX, PM_DATA1
    MOV WORD[PM_DESC_DATA+2], AX
    SHR EAX, 16
    MOV BYTE [PM_DESC_DATA+4], AL
    MOV BYTE [PM_DESC_DATA+7], AH

    ;初始化32位的stack段
    XOR EAX, EAX
    MOV AX, DS
    SHL EAX, 4
    ADD EAX, PM_STACK
    MOV WORD[PM_DESC_STACK + 2], AX
    SHR EAX, 16
    MOV BYTE [PM_DESC_STACK + 4], AL
    MOV BYTE [PM_DESC_STACK + 7], AH

    ;加载GDTR
    XOR EAX, EAX
    MOV AX, DS
    SHL EAX, 4
    ADD EAX, PM_GDT
    MOV DWORD [GDTPTR + 2], EAX
    LGDT [GDTPTR]

    ;A20
    CLI

    IN AL, 92H
    OR AL, 00000010B
    OUT 92H, AL

    ;切换到保护模式
    MOV EAX, CR0
    OR EAX, 1
    MOV CR0, EAX

    ;STI 为什么开中断会闪烁 不断重启？
    JMP DWORD SELECTOER_CODE32: 0

[SECTION .DATA1]
ALIGN 32
PM_DATA1:
[BITS 32]
PMMESSAGE : DB "POTECT MODE", 0	;
OFFSETPMESSAGE  EQU PMMESSAGE - $$
       DATALEN  EQU $- PM_DATA1



;全局的堆栈段
[SECTION .STACK]

ALIGN 32
PM_STACK:
[BITS 32]
    TIMES 512 DB 0
    TOP_OF_STACK  EQU $ - PM_STACK -1
;开辟512字节的堆栈	


[SECTION .PM_CODE]        	;32位的代码段

PM_CODE:
[BITS 32]
MOV AX, SELECTOER_DATA 	;通过数据段的选择子放入ds寄存器，就可以用段+偏移进行寻址
MOV DS, AX


MOV AX, SELECTOER_VIDEO
MOV GS, AX

MOV AX, SELECTOER_STACK
MOV SS, AX
MOV ESP, TOP_OF_STACK

MOV AH, 0CH
XOR ESI, ESI
XOR EDI, EDI
MOV ESI, OFFSETPMESSAGE
MOV EDI, (80*10 +0) *2
CLD

.1:
LODSB
TEST AL, AL
JZ .2
MOV [GS: EDI], AX
ADD EDI, 2
JMP .1

.2:                   	;显示完毕
;JMP $
JMP DWORD SELECTOER_CODE32_CLOSE: 0
SEGCODE32_LEN  EQU $ - PM_CODE

;从32位模式下跳出来 所以应该是32位的段
;32位 对齐的16位
;从保护模式跳回实模式需要特殊的段选择子
[SECTION .OUT_PM]
PM_CODE_CLOSE:
ALIGN   32
[BITS 16]
    mov ax, SELECTOER_UPDATE_DESC
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax


    ;切换到实模式
    MOV EAX, CR0
    AND EAX, 0
    MOV CR0, EAX


jmp 0:BACK_ENTRY_SEGMENT;
SEGCODE32_CLOSELEN EQU $ - PM_CODE_CLOSE







;保护没有溢出 共5个扇区
times 5120-($-$$) db 0