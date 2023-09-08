#pragma once
#include <stdint.h>
extern void printInPos(char*msg,uint16_t len,uint8_t row,uint8_t col); 
extern void putchar(char c);         // Output one character
extern char getch();                // Get keyboard input

uint32_t strlen(char *str);
void print(char *s);
void printint(uint32_t num);
void readBuf(char * buffer , uint32_t maxlen);
//字符串复制
uint32_t strncpy(char *dest, char *src ,uint32_t n);
//字符串对比 -1 小于 = 等于 1 大于
uint32_t strncmp(char *dest, char *src ,uint32_t n);
//字符串拼接 s1 要 s2 的 n个
uint32_t strncat(char *s1, char *s2 , char * aim , uint32_t n);
//小端序的 16位整数变 10进制整数
uint32_t atoi_16_small(char * pos , uint32_t num);
//小端序的 10进制整数变字符串
void itoa_10_small(char * array , uint32_t len , uint32_t num);
//清空
void bzero(char * s , uint32_t len);
//找到第一个单词
void getFirstWord(char*str,uint32_t strlen , char*buf);
//获取一个路径的父路径
uint32_t fatherpath(char * path , uint32_t pathlen , char * farpath);
//获取名字
uint32_t getname(char * path , uint32_t pathlen , char * name);
//打印换行回车
void println(char * s );
void printintln(uint32_t num);
void print_n(char * s , uint32_t n);