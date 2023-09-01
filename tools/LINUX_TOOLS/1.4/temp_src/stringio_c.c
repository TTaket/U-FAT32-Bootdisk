#include "stringio_h.h"
#include "system_h.h"


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


void readBuf(char*buffer,uint16_t maxLen){
	int i = 0;
	while(1){
		char tempc = getch();
		if(!(tempc == 0x0D || tempc == '\b' || (tempc >= 32 && tempc <= 127))){
			continue;
		}
		if(i > 0 && i < maxLen - 1){
			if(tempc == 0x0D){
				break;	
			}else if(tempc == '\b'){
				putchar('\b');
				putchar(' ');
				putchar('\b');
				i--;
			}else{
				 putchar(tempc);
				 buffer[i] = tempc;
				 i++;
			}
		}else if(i >= maxLen - 1){
			if(tempc == '\b'){
				putchar('\b');
				putchar(' ');
				putchar('\b');
				i--;
			}else if(tempc == 0x0D){
				break;
			}
		}else if(i <= 0){
			if(tempc == 0x0D)
				break;
			else if(tempc != '\b'){
				putchar(tempc);
				buffer[i] = tempc;
				i++;
			}
		}
	}
	putchar('\r');
	putchar('\n');
	buffer[i] = 0;
}
uint32_t atoi_16_small(char * pos , uint32_t len){
    uint32_t ret = 0; 
    for(int i = len-1;i>=0;i--){
        ret *= 256;
        ret += (uint8_t)pos[i];
    }
    return ret;
}

//小端序的 10进制整数变字符串
void itoa_10_small(char * array , uint32_t len , uint32_t num){
    for(int i = 0;i<len;i++){
        array[i] = num%256;
        num/=256;
    }
    array[len] = 0;
}

uint32_t strncpy(char *dest, char *src ,uint32_t n){
    uint32_t readnum = 0;
    for(readnum = 0;readnum <n;readnum++){
        dest[readnum ] = src[readnum ];
    }
    return readnum;
}

uint32_t strncmp(char *dest, char *src ,uint32_t n){
    int ret = 0;
    for(int i = 0 ;i<n;i++){
        if(dest[i] == src[i]){
            continue;
        }else if(dest[i] < src[i]){
            return ERR;
        }else{
            return 1;
        }
    }
    return 0;
}
void bzero(char * s , int len){
    for(int i = 0;i<len;i++){
        s[i]= 0;
    }
}

//字符串拼接 s1 要 s2 的 n个
uint32_t strncat(char *s1, char *s2 , char * aim , uint32_t n){
    uint32_t len1 = strlen(s1);
    uint32_t len2 = strlen(s2);
    for(int i = 0;i<len1;i++){
        aim[i] = s1[i];
    }
    for(int i = 0;i<min(n , len2);i++){
        aim[i+len1] = s2[i];
    }
    aim[len1 + min(n , len2)] = 0;
    return len1 + min(n , len2);
}

void getFirstWord(char*str,char*buf){
	int i = 0;
	while(str[i] && (str[i] != ' ') && (str[i] != '\n')){
		buf[i] = str[i];
		i++;
	}
	buf[i] = 0;
}

//获取父路径
uint32_t fatherpath(char * path , uint32_t pathlen , char * farpath){
    int offset =pathlen -1; 
    for(offset; offset>=0;offset--){
        if(path[pathlen -1] == '/'){
            break;
        }
    }
    if(offset <=0){
        return ERR;
    }
    strncpy(farpath , path , offset);
    return offset;
}

//获取名字
uint32_t getname(char * path , uint32_t pathlen , char * name){
    int offset =pathlen -1; 
    for(offset; offset>=0;offset--){
        if(path[pathlen -1] == '/'){
            break;
        }
    }
    if(offset <=0){
        return ERR;
    }
    strncpy(name , path + offset+1 , pathlen - offset -1);
    return pathlen - offset-1;
}

void printint(uint32_t num){
    char buffer[10];
    for(int i = 9;i>=0;i--){
        buffer[i] = num%10 + '0';
        num/=10;
    }
    buffer[10] = 0;
    print(buffer);
}
void println(){
    char * tmp ="\n";
    print(tmp);
}

void print_n(char * s , uint32_t n){
    char buffer[3];
    char * tmp1 = " ";
    char * tmp2 = "\n\r";
    for(int i = 0;i<n;i++){
        buffer[0] = (uint8_t)s[i]/16;
        buffer[1] = (uint8_t)s[i]%16;
        buffer[2] = 0;
        if(buffer[0] >= 10){
            buffer[0] = buffer[0] - 10 + 'a';
        }else{
            buffer[0]+= '0';
        }
        if(buffer[1] >= 10){
            buffer[1] = buffer[1] - 10 + 'a';
        }else{
            buffer[1]+= '0';
        }
        print(buffer);
        print(tmp1);
    }
}