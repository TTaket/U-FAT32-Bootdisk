#pragma once
#include <stdint.h>
//在指定lba读取n个扇区到达指定位置
extern void bread(uint32_t lba_pos , char * aimpos, uint16_t num);

//向lba写入n个扇区
extern void bwrite(uint32_t lba_pos , char * aimpos, uint16_t num);

//读取某一块某一偏移量开始的一些n个字节
uint32_t read_nbyte(uint32_t lba , uint32_t offset ,uint32_t n , char* info);

//写入某一块某一偏移量开始的一些n个字节
uint32_t write_nbyte(uint32_t lba , uint32_t offset ,uint32_t n , char* info);