#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 
extern char global_path[PATH_SIZ];
void cmd_mkdir(char * p){
    //确定路径 
    
    char mkdirpath[PATH_SIZ] ={0};
    char name[PATH_SIZ]={0};
    
    char * tmpjuedui = "/";
    //判断是绝对路径还是相对路径
    if((strncmp(p , tmpjuedui , 1) ==0)){//绝对路径
        strncpy(mkdirpath , p ,strlen(p));
    }else{
        strncpy(mkdirpath , global_path ,strlen(global_path));
        strncpy(mkdirpath+strlen(global_path) , p+1 ,strlen(p)-1);
    }
    getname(mkdirpath , strlen(mkdirpath), name);

    //普通可读可写文件
    create_file(mkdirpath , strlen(mkdirpath) , name, strlen(name) ,0x10);
}