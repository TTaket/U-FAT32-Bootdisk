#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 

extern char global_path[];
extern uint32_t file_info_len;
extern DBR_info_Struct DBR_info;

uint32_t cmd_ls(char * p){
    char *welcome_cmd_ls = "call_cmd_ls\r\n";
    char *xd = "./\0";
    char *jd = "/\0";
    char *debug_1 ="DEBUG: ";
    char *debug_2 = "now_path: ";
    char *debug_3 = "now_clu: ";
    char *err_getclu = "ERR: no such file";
    char * err_notdir = "this file not dir";
    char * err_Volumelabel ="Volume label";
    char * err_off = "ERR: err_off";
    uint32_t dir_flag = 0;//指定的为目录
    uint32_t ls_clu = 0;//遍历的簇号
    //确定路径 
    char * tmp_glo = global_path;
    char lspath[PATH_SIZ] ={0};
    char name[PATH_SIZ] ={0};
    if(strlen(p) == 0){ //空字符串
        strncpy(p, xd , 3);
    }
    //判断是绝对路径还是相对路径
    if((strncmp(p , jd , 1) ==0)){//绝对路径
        strncpy(lspath , p ,strlen(p));
    }else{
        strncpy(lspath , tmp_glo ,strlen(tmp_glo));
        strncpy(lspath+strlen(tmp_glo) , p+2 ,strlen(p+2));
    }

    //是否指定打印的为目录
    if((strlen(lspath) >= 2) && (strncmp(lspath + strlen(lspath) -1  , jd , 1) ==0)){
        lspath[strlen(lspath) - 1] = (char)0;
        dir_flag = 1;
    }
    //不是根目录 - 获取簇号
    if(strlen(lspath) != 1){
        getname(lspath , strlen(lspath) ,  name);
        file_info_Struct finfo_tmp;
        uint32_t now_clu =  getclu_bypath(lspath , strlen(lspath));
        //没有对应的路径:
        if(now_clu == ERR){
            println(err_getclu);
            return 0;
        }
        uint32_t off = getoffinclu_byname(now_clu , name , strlen(name));
        //获取对应信息
        getfile_info(now_clu , off , &finfo_tmp);
        //如果是卷标
        if((finfo_tmp.file_attr & 0x08 ) != 0){
            println(err_Volumelabel);
            return 1;
        }
        //如果是文件的话
        if((finfo_tmp.file_attr & 0x10 ) != (char)0x10){
            if(dir_flag == 1){
                println(err_notdir);
                return 1;
            }
            println(finfo_tmp.name_3);   
            return 0;
        }
        ls_clu = finfo_tmp.cluster_pos;
    }else{
        ls_clu = DBR_info.Cluster_RootDir;
    }
    println(debug_1);
    print(debug_2);
    println(lspath);
    print(debug_3);
    printintln(ls_clu);

    //遍历这个目录的每一个簇
    file_info_Struct finfo;
    char namebuffer[MAX_BUFFER];
    char pathbuffer[MAX_BUFFER];
    char newpathbuffer[MAX_BUFFER];
    uint32_t now_clu = ls_clu;
    uint32_t next_clu = now_clu;
    while(next_clu!= ERR){
        now_clu = next_clu;
        next_clu = getnextclu(now_clu);
        // printintln(now_clu);
        //遍历这个簇里面的所有文件
        uint32_t off = clu_off(now_clu);
        if(off == ERR){
            println(err_off);
            return 1;
        }
        // printintln(off);
        uint32_t filemax = off / file_info_len;
        // printintln(filemax);
        for(int i = 1;i<=filemax ;i++){
            getfile_info(now_clu ,i , &finfo );
            //删除文件跳过 
            if(finfo.name_3[0] == (char)0xE5){
                continue;
            }
            //空文件跳过
            if(finfo.name_3[0] == (char)0x00){
                continue;
            }
            //卷标跳过
            if((finfo.file_attr & (char)0x08 ) != 0){
                continue;
            }
            //printintln(finfo.file_attr);
            println(finfo.name_3);
        } 
    }    
    return 0;


    
}