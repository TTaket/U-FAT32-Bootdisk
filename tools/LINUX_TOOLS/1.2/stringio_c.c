
#include "./stringio_h.h"


// Calculate string length
uint16_t strlen(char *str){
    uint16_t count = 0;
    while(str[count++] != 0);
    return count - 1;
}

void print(char *s){
    uint16_t len = strlen(s);
    for(int i = 0;i<len;i++){
        putchar(s[i]);
    }
}

void readBuf(char * buffer , uint16_t maxlen){
    int i = 0;
    char tmp;
    while(1){
        tmp = getch();
        if(!(tmp == '\n' || tmp == '\b' || (tmp >=32 && tmp <= 127))){
            continue;
        }
        if(i<maxlen){//没有达到可读上限制
            if(tmp == '\n'){
                break;
            }else if(tmp == '\b'){
                if(i>0){
                    putchar('\b');//光标回退
                    putchar(' ');//覆盖
                    putchar('\b');//光标回退
                    i--;
                }
            }else{
                buffer[i] = tmp;
                i++;
                putchar(tmp);
            }
        }else{
            if(i>0 && (tmp == '\b')){
                putchar('\b');//光标回退
                putchar(' ');//覆盖
                putchar('\b');//光标回退
                i--;
            }else if(tmp == '\n'){
                break;
            }
        }    
    }
    buffer[++i] = 0;
    putchar('\r');
    putchar('\n');
}
