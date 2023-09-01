#include <stdint.h>
#include "fat32.h"
#include "system_h.h" 

extern char global_path[PATH_SIZ];
extern uint32_t MAX_BUFFER;
extern uint32_t file_info_len;


uint32_t cmd_ls(char * p){
    //确定路径 
    char * tmp1 ="1hao ";
    char * tmp2 ="2hao ";
    char * tmp3 ="\n";

    char * tmp_glo = global_path;
    char lspath[PATH_SIZ] ={0};
    char name[PATH_SIZ] ={0};
    char * tmpjuedui = "/";
    //判断是绝对路径还是相对路径
    if((strncmp(p , tmpjuedui , 1) ==0)){//绝对路径
        strncpy(lspath , p ,strlen(p));
    }else{
        strncpy(lspath , tmp_glo ,strlen(tmp_glo));
        strncpy(lspath+strlen(tmp_glo) , p+1 ,strlen(p)-1);
    }
    // print(lspath);
    // printint(strlen(lspath));
    if(strlen(lspath) == 1){
        // print(tmp1);
        uint32_t now_clu = 2;
        struct file_info_Struct finfo;
        uint32_t next_clu = now_clu;
        while(next_clu != ERR){
            now_clu = next_clu;
            // print(tmp1);
            next_clu = getnextclu(now_clu);
            //printint(next_clu);
            //println();
            
            //遍历这个簇里面的所有文件
            uint32_t off = clu_off(now_clu);
            // printint(off);
            // println();
            uint32_t filemax = off / file_info_len;
            // printint(filemax);
            // println();
            for(int i = 0;i<filemax ;i++){
                print(tmp1);
                getfile_info(now_clu , i , &finfo );
                print(tmp1);
                //删除文件跳过 
                if(finfo.name_3[0] == 0xE5){
                    continue;
                }
                print("\r\n");
                print(finfo.name_3);
            } 
        }    
        return 0;   
    }
    getname(lspath , strlen(lspath) ,  name);
    //确定目标路径是不是目录
    struct file_info_Struct finfo;
    uint32_t now_clu =  getclu_bypath(lspath , strlen(lspath));
    uint32_t off = getoffinclu_byname(now_clu , name , strlen(name));
    getfile_info(now_clu , off , &finfo);
    if((finfo.file_attr & 0x10 )== 1){
        //遍历这个目录的每一个簇
        struct file_info_Struct finfo;
        
        
        char namebuffer[MAX_BUFFER];
        char pathbuffer[MAX_BUFFER];
        char newpathbuffer[MAX_BUFFER];
        uint32_t now_clu =  getclu_bypath(lspath , strlen(lspath));
        uint32_t next_clu = now_clu;
        while(next_clu!= ERR){
            now_clu = next_clu;
            next_clu = getnextclu(now_clu);
            //遍历这个簇里面的所有文件
            uint32_t off = clu_off(now_clu);
            uint32_t filemax = off / file_info_len;
            for(int i = 0;i<filemax ;i++){
                getfile_info(now_clu ,i , &finfo );
                //删除文件跳过 
                if(finfo.name_3[0] == 0xE5){
                    continue;
                }
                print("\r\n");
                print(finfo.name_3);
            } 
        }    
        return 0;
    }else{
        return 1;
    }


    
}