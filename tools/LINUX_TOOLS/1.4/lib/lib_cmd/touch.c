#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 
extern char global_path[PATH_SIZ];
void cmd_touch(char * p){
    //确定路径 
    
    char touchpath[PATH_SIZ] ={0};
    char name[PATH_SIZ]={0};
    
    char * tmpjuedui = "/";
    //判断是绝对路径还是相对路径
    if((strncmp(p , tmpjuedui , 1) ==0)){//绝对路径
        strncpy(touchpath , p ,strlen(p));
    }else{
        strncpy(touchpath , global_path ,strlen(global_path));
        strncpy(touchpath+strlen(global_path) , p+1 ,strlen(p)-1);
    }
    getname(touchpath , strlen(touchpath), name);

    //普通可读可写文件
    create_file(touchpath , strlen(touchpath) , name, strlen(name) ,0);
}