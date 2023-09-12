#include "brk_h.h"
//读取某一块某一偏移量开始的一些n个字节数量 - 不支持跨块读入
uint32_t read_nbyte(uint32_t lba , uint32_t offset ,uint32_t n , char* info){
    //临时缓冲区
    char tmp_buffer[513];
    //取出这一块
    bread(lba , tmp_buffer , 1);
    strncpy(info , tmp_buffer + offset , n);
    //默认
    return 0;
}


//写入某一块某一偏移量开始的一些n个字节数量 - 不支持跨块写
uint32_t write_nbyte(uint32_t lba , uint32_t offset ,uint32_t n , char* info){
    //临时缓冲区
    char tmp_buffer[513];
    char * over1="over1";
    char * over2="over2";
    //取出这一块
    bread(lba , tmp_buffer , 1);
    
    //print_n(tmp_buffer + offset , 32);

    strncpy(tmp_buffer + offset, info , n);
    
    println(over1);

    //print_n(tmp_buffer + offset , 32);
    
    bwrite(lba , tmp_buffer , 1);
    
    println(over2);
    // printintln(offset);
    //默认
    return 0;
}