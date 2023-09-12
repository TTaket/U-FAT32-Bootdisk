#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 

extern char global_path[];
extern uint32_t global_clu;

uint32_t cmd_cd(char * p){
    //确定路径 
    char cdpath[PATH_SIZ] ={0};
    char name[PATH_SIZ] ={0};
    char * jd = "/";
    char * ts_1 ="./";
    char * ts_2 ="../";
    char *err_getclu = "ERR: no such file";
    char * err_file = "ERR: it is not dir";
    char * tmp_glo = global_path;
    
    //特殊判断1
    if((strlen(p)==2) && (strncmp(p , ts_1 ,2 ) == 0)){
        return 0;
    } 
    //特殊判断2 
    if((strlen(p)==3) && (strncmp(p , ts_2 ,3 ) == 0)){
        if(strlen(tmp_glo) == 1){//当前是根路径 打印错误
            println(err_file);
        }else{//返回到上一级
            file_info_Struct finfo_tmp;
            char tmp_fapath[PATH_SIZ];
            bzero(tmp_fapath , PATH_SIZ);
            
            getfile_info(global_clu , 2 , &finfo_tmp);
            global_clu = finfo_tmp.cluster_pos;
            fatherpath(tmp_glo ,strlen(tmp_glo) ,tmp_fapath);
            bzero(tmp_glo , strlen(tmp_glo));
            strncpy(tmp_glo , tmp_fapath , strlen(tmp_fapath));
            if(strlen(tmp_glo) == 0){//
                tmp_glo[0] = '/';
                tmp_glo[1] = 0;
            }
        }
        return 0;
    }

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
        global_clu = finfo_tmp.cluster_pos;
        return 0;
    }else{
        println(err_file);
        return 1;
    }
    
}