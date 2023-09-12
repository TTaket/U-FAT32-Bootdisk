#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 
extern char global_path[];
void cmd_touch(char * p){
    
    //确定路径
    char touchpath[PATH_SIZ] ={0};
    char name[PATH_SIZ]={0};
    char * jd = "/";
    char * xd = "./";
    char *err_getclu = "ERR: no such file";
    char * err_file = "ERR: it is not dir";
    char * err_nofilename = "ERR: No Designate File Name";
    char * tmp_glo = global_path;
    char *debug_1 ="DEBUG: ";
    char *debug_2 = "now_path: ";
    char *debug_3 = "now_name: ";
    
    //如果没有字符串
    if(strlen(p) == 0){
        println(err_nofilename);
        return 1;
    }

    //如果直接输入文件名
    //判断是绝对路径还是相对路径
    if((p[0] != (char)'/') && (p[0]!=(char)'.')){//如果直接输入文件名
        strncpy(touchpath , tmp_glo ,strlen(tmp_glo));
        if(strlen(tmp_glo) != 1){
            touchpath[strlen(tmp_glo)] = (char)'/';
            strncpy(touchpath+strlen(tmp_glo)+1 , p ,strlen(p));
        }else{
            strncpy(touchpath+strlen(tmp_glo) , p ,strlen(p));
        }
    }else if((strncmp(p , jd , 1) ==0)){//绝对路径
        strncpy(touchpath , p ,strlen(p));
    }else{
        strncpy(touchpath , tmp_glo ,strlen(tmp_glo));
        if(strlen(tmp_glo) == 1){
            strncpy(touchpath+strlen(tmp_glo) , p+2 ,strlen(p+2));
        }else{
            strncpy(touchpath+strlen(tmp_glo) , p+1 ,strlen(p+1));
        }
    }

    //是否指定路径后有 '/'
    if((strlen(touchpath) >= 2) && (strncmp(touchpath + strlen(touchpath) -1  , jd , 1) ==0)){
        println(err_nofilename);
        return 0;
    }

    

    //如果路径是根目录
    if(strlen(touchpath) == 1){
        println(err_nofilename);
        return 0;
    }

    //获取名字
    getname(touchpath , strlen(touchpath) ,  name);

    println(debug_1);
    print(debug_2);
    println(touchpath);
    print(debug_3);
    println(name);

    //确定目标路径是不是目录
    file_info_Struct finfo_tmp;
    uint32_t now_clu =  getclu_bypath(touchpath , strlen(touchpath));
    if(now_clu == ERR){
        println(err_getclu);
        return 0;
    }

    create_file(touchpath , strlen(touchpath) , name, strlen(name) ,0);

}