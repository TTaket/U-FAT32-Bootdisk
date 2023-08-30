#include "./stringio_h.h"
#define BUF_SIZ 32

void startup(){
    char * title = "hello , welcome to ljy_os";
    char * subtitle = "designed by ljy";
    char * hint = "loading system..";
    char * nt = "Press enter..\r\n";
    printInPos(title , strlen(title) , 5 ,5);
    printInPos(subtitle , strlen(subtitle) , 6 ,5);
    printInPos(hint , strlen(hint) , 7 , 5);
    printInPos(nt , strlen(nt) , 8 , 5);
}

void show_p(){
    char * p_str = "LJY_OS >";
    print(p_str);
}

void show_help(){
    char*help_str = "help\r\n";
    print(help_str);
}

void shell(){   
    show_p(); 
    show_help();
}