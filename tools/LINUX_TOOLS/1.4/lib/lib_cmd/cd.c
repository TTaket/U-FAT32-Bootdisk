#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 

extern char global_path[PATH_SIZ];

uint32_t cmd_cd(char * p){
    //确定路径 
    char cdpath[PATH_SIZ] ={0};
    char name[PATH_SIZ] ={0};
    char * tmpjuedui = "/";
    //判断是绝对路径还是相对路径
    if((strncmp(p , tmpjuedui , 1) ==0)){//绝对路径
        strncpy(cdpath , p ,strlen(p));
    }else{
        strncpy(cdpath , global_path ,strlen(global_path));
        strncpy(cdpath+strlen(global_path) , p+1 ,strlen(p)-1);
    }
    getname(cdpath , strlen(cdpath) ,  name);
    //确定目标路径是不是目录
    file_info_Struct finfo;
    uint32_t now_clu =  getclu_bypath(cdpath , strlen(cdpath));
    uint32_t off = getoffinclu_byname(now_clu , name , strlen(name));
    getfile_info(now_clu , off , &finfo);
    if((finfo.file_attr & 0x10 )== 1){
        bzero(global_path , PATH_SIZ);
        strncpy(global_path , cdpath , strlen(cdpath));
        return 0;
    }else{
        return 1;
    }
    
}