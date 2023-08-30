#include <stdint.h>
extern void printInPos(char*msg,uint16_t len,uint8_t row,uint8_t col); 
extern void putchar(char c);         // Output one character
extern char getch();                // Get keyboard input

uint16_t strlen(char *str);
void print(char *s);
void readBuf(char * buffer , uint16_t maxlen);
