#pragma once
#include <stdint.h>
//对fat32系统进行抽象 



struct DBR_info_Struct{
    uint16_t Sector_Byte;//每扇区字节数
    uint8_t  Cluster_Sector;//每簇扇区数
    uint16_t Start_FAT;//fat表的起始位置
    uint8_t  Num_FAT;//fat表的个数
    uint32_t Start_DBR;//DBR的起始位置
    uint32_t Cluster_Num;//总共扇区数量
    uint32_t FAT_Size;//fat表的大小
    uint32_t Cluster_RootDir ;//根目录所在的簇号
};

struct fsinfo_info_Struct{
    uint32_t next_Cluster;//下一个可用簇的位置
    uint32_t available_Cluster;//剩余的空闲簇数
};

//文件目录项 
struct file_info_Struct{
    char name_1[9];//文件名
    char name_2[5];//文件后缀
    char name_3[13];//文件全名
    uint8_t  file_attr;//文件属性
    uint32_t cluster_pos;//起始簇号
    uint32_t file_size;//文件大小
};


//初始化DBR
void init_DBRinfo();
void init_fsinfo_info();

//扇区和簇相互转换
uint32_t SectorToCluster(uint32_t sec);
uint32_t ClusterToSector(uint32_t clu);

//更新下一个空闲的簇
uint32_t Update_clunum_next();
//更新剩余簇的数量
uint32_t Update_clunum_available(int change);

//查询 
//获得某个clu开始的第off个目录项的内容 返回-1表示有错误 正确返回0
//这个查询只能查询在这个给clu内的目录项 超出返回-1
uint32_t getfile_info(uint32_t clu , uint32_t off , struct file_info_Struct * finfo);
//根据名字找到起始簇 - 需要传入当前的clu
//错误返回-1 找到返回clu
//这是找到这个目录下是否有该名字 换言之如果这个目录大于一个簇 他会到下一个簇中去找
uint32_t getclu_byname(uint32_t now_clu , char * name , uint32_t n);
//按照绝对路径找到这个文件的簇
//按照路径找到 错误返回-1 找到返回clu
uint32_t getclu_bypath(char * path , uint32_t len);
//找到该名字的文件在在这个簇中的偏移量
uint32_t getoffinclu_byname(uint32_t now_clu , char * name , uint32_t n);
//在fat表里面查看这个簇上面的值是什么
//没有下一个簇返回-1 找到了返回clu
uint32_t getnextclu(uint32_t now_clu);
//把一个簇的内容读入到缓冲区里面 
uint32_t read_clu(char * buffer , uint32_t clu);
//返回一个簇剩余空间 
uint32_t clu_available_size(uint32_t clu);
//返回一个簇最后的偏移量(簇内)
uint32_t clu_off(uint32_t clu);


//新建 
//扩充簇
uint32_t expand_clu(uint32_t now_clu);
//新开辟簇
uint32_t create_clu();
//新建文件 提供路径 名字 后缀 属性
uint32_t create_file(char* path , uint32_t pathlen , char*name, uint32_t namelen ,uint8_t attr);


//删除
//删除文件
uint32_t del_file(char * path , uint32_t pathlen , char*name, uint32_t namelen);
//删除目录
uint32_t del_dir(char * path , uint32_t pathlen ,char*name, uint32_t namelen);
//清空一个簇
void del_clu(uint32_t clu);

//更改 
//把fat表内的第i个簇的值改变为x
void change_fat_clu(uint32_t clu , uint32_t x);

//辅助 
//计算簇中偏移 off 个字节所在的扇区
uint32_t get_sec_afteroff(uint32_t clu , uint32_t off);


