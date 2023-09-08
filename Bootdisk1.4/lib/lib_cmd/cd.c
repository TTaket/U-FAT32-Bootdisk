#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 

extern char global_path[];

uint32_t cmd_cd(char * p){
    //确定路径 
    char cdpath[PATH_SIZ] ={0};
    char name[PATH_SIZ] ={0};
    char * jd = "/";
    char *err_getclu = "ERR: no such file";
    char * err_file = "ERR: it is not dir";
    char * tmp_glo = global_path;
    
    //判断是绝对路径还是相对路径
    if((strncmp(p , jd , 1) ==0)){//绝对路径
        strncpy(cdpath , p ,strlen(p));
    }else{
        strncpy(cdpath , tmp_glo ,strlen(tmp_glo));
        if(strlen(tmp_glo) == 1){
            strncpy(cdpath+strlen(tmp_glo) , p+2 ,strlen(p+2));
        }else{
            strncpy(cdpath+strlen(tmp_glo) , p+1 ,strlen(p+1));
        }
    }

    //是否指定路径后有 '/'
    if((strlen(cdpath) >= 2) && (strncmp(cdpath + strlen(cdpath) -1  , jd , 1) ==0)){
        cdpath[strlen(cdpath) - 1] = (char)0;
    }

    //如果路径是根目录
    if(strlen(cdpath) == 1){
        tmp_glo[0] = (char)'/';
        tmp_glo[1] = (char)0;
        return 0;
    }

    //获取名字
    getname(cdpath , strlen(cdpath) ,  name);

    //确定目标路径是不是目录
    file_info_Struct finfo_tmp;
    uint32_t now_clu =  getclu_bypath(cdpath , strlen(cdpath));
    if(now_clu == ERR){
        println(err_getclu);
        return 0;
    }

    uint32_t off = getoffinclu_byname(now_clu , name , strlen(name));

    getfile_info(now_clu , off , &finfo_tmp);

    //判断是否是目录
    if((finfo_tmp.file_attr & 0x10 )== 0x10){
        bzero(tmp_glo , PATH_SIZ);
        strncpy(tmp_glo , cdpath , strlen(cdpath));
        return 0;
    }else{
        println(err_file);
        return 1;
    }
    
}