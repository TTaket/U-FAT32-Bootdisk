#include <stdint.h>
#include "fat32.h"
#include "system_h.h"

//确认fat32 系统位置
extern const uint32_t DBR_pos ;//DBR的lba起始号
extern uint32_t fsinfo_pos ;//fsinfo扇区的起始号
extern uint32_t FAT1_pos ;//FAT1的起始号
extern uint32_t FAT2_pos ;//FAT2的起始号
extern uint32_t DATA_pos ;//数据区的起始号

//辅助参数
extern const uint32_t file_info_len ;//目录项长度为32
extern const uint32_t end_clu ;//FFFFFF
extern const uint32_t fat_clunum ;//每张fat表能存入的簇信息个数
extern uint32_t clunum_fileinfo ;//每一个簇能存的记录项的数量
extern uint32_t Maxclunum;// 最多有多少个簇 也就是簇的上限是多少
extern uint32_t clunum_byte ;//每一个簇的总字节数量

extern uint32_t clunum_available ;//剩余的簇数
extern uint32_t clunum_next ;//下一个簇的位置
extern uint32_t MAX_BUFFER ;//


//全局变量
extern struct DBR_info_Struct DBR_info;
extern struct fsinfo_info_Struct fsinfo_info;

extern char global_path[PATH_SIZ];


void init(){
    int tmp = 0;
    tmp = DBR_pos;
    tmp = FAT1_pos;
    tmp = fsinfo_pos;
    tmp = FAT2_pos;
    tmp = DATA_pos;
    tmp = file_info_len;
    tmp = end_clu;
    tmp = fat_clunum;
    tmp = clunum_fileinfo;
    tmp = Maxclunum;
    tmp = clunum_byte;
    tmp = clunum_available;
    tmp = clunum_next;
    tmp = MAX_BUFFER;
    struct DBR_info_Struct tmp1 = DBR_info;
    struct fsinfo_info_Struct tmp2 = fsinfo_info;
    char * tmpchar = global_path;

}