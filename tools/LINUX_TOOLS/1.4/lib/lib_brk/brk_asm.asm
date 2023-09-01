GLOBAL bread
GLOBAL bwrite
[BITS 16]


;========================================================
;   名称：
;       bread
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
bread:
    PUSHA;
    mov ebp , esp;利用esp来进行取参
    ;放入首地址
    add ebp , 16 + 4 
    MOV EAX ,[ebp];
    add ebp , 4;
    MOV EBX , [ebp];
    add ebp , 4;
    MOV ECX , [ebp];


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



    read_info:
        MOV DX , 001F0H;
        IN  AX , DX;
        MOV [BP] ,AX;
        ADD BP , 2
        loop  read_info

    POPA;
    retf



;========================================================
;   名称：
;       WRITE_DISK
;   功能：
;       写入lba
;   入参：
;       EAX: LBA的逻辑扇区编号
;       BX : 拷贝到本地的内存地址
;       CX : 读入扇区的数量
;   修改：
;       AX BX CX DX SI DI BP
;   返回值：
;========================================================
bwrite:
    PUSHA;
    mov ebp , esp;利用esp来进行取参
    ;放入首地址
    add ebp , 16 + 4 
    MOV EAX , [ebp];
    add ebp , 4;
    MOV EBX , [ebp];
    add ebp , 4;
    MOV ECX , [ebp];


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
    MOV AX , 0x30;
    OUT DX , AL;
    ;读取扇区:0x20
    ;写入扇区:0x30
    ;磁盘识别:0xEC

    .Check_status:;检查磁盘状态
        NOP
        IN  AL , DX                 ;DX = 001F7H
        AND AL , 0X88			;第4位为1表示硬盘准备好数据传输，第7位为1表示硬盘忙
        CMP AL , 0X08
        JNZ .Check_status

    ;准备完成开始读取
    ;计算循环次数 扇区个数在di里面 一次能读出两个字节 512/2 * di 是 16位乘8位 答案32位在DX : AX;
    MOV AX , DI;
    MOV EBX , 256;
    MUL BX;
    MOV cl , 10H;
    SHL EDX , cl;
    MOV ECX , EDX;
    ADD CX , AX;

    write_info:
        MOV DX , 001F0H;
        MOV AX , [BP];
        OUT DX , AX;
        ADD BP , 2
        loop  write_info

    POPA;
    retf