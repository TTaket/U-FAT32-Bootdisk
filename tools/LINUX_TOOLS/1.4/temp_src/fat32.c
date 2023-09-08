#include "fat32.h"
#include "system_h.h"
#include <stdint.h>

//确认fat32 系统位置
uint32_t DBR_pos = 1024;//DBR的lba起始号
uint32_t fsinfo_pos = 0;//fsinfo扇区的起始号
uint32_t FAT1_pos = 0;//FAT1的起始号
uint32_t FAT2_pos = 0;//FAT2的起始号
uint32_t DATA_pos = 0;//数据区的起始号

//辅助参数
uint32_t file_info_len = 32;//目录项长度为32
uint32_t end_clu = 0x0FFFFFFF;//结束簇
uint32_t fat_clunum = 512/4;//每张fat表能存入的簇信息个数
uint32_t clunum_fileinfo = 0; //每一个簇能存的记录项的数量
uint32_t Maxclunum = 0;//最多有多少个簇 也就是簇的上限是多少
uint32_t clunum_byte = 0;//每一个簇的总字节数量

uint32_t clunum_available = 0; //剩余的簇数
uint32_t clunum_next = 0; //下一个簇的位置



//全局变量
DBR_info_Struct DBR_info;
fsinfo_info_Struct fsinfo_info;


void init_DBRinfo(){
    char DBR[512] = {0};
    // printInPos(tmp , strlen(tmp) , 10 ,5);
    bread(DBR_pos , DBR , 1);
    // printInPos(tmp , strlen(tmp) , 11 ,5);
    //DBR 读取磁盘验证 正确
    //print_n(DBR , 512);
    //初始化
    DBR_info.Sector_Byte = (uint16_t)atoi_16_small(DBR+ 0x0B, 2);
    DBR_info.Cluster_Sector = (uint8_t)atoi_16_small(DBR+ 0x0D, 1);
    DBR_info.Start_FAT = (uint16_t)atoi_16_small(DBR+ 0x0E, 2);
    DBR_info.Num_FAT=(uint8_t )atoi_16_small(DBR+ 0x10, 1);
    DBR_info.Start_DBR=(uint32_t)atoi_16_small(DBR+ 0x1c, 4);
    DBR_info.Cluster_Num=(uint32_t)atoi_16_small(DBR+ 0x20, 4);
    DBR_info.FAT_Size=(uint32_t)atoi_16_small(DBR+ 0x24, 4);
    DBR_info.Cluster_RootDir=(uint32_t)atoi_16_small(DBR+ 0x2c, 4);
    // printInPos(tmp , strlen(tmp) , 12 ,5);
    fsinfo_pos = DBR_info.Start_DBR + 1;//fsinfo扇区的起始号
    FAT1_pos = DBR_info.Start_DBR + DBR_info.Start_FAT;//FAT1的起始号
    FAT2_pos = FAT1_pos + DBR_info.FAT_Size;//FAT2的起始号
    DATA_pos = FAT2_pos + DBR_info.FAT_Size;//数据区的起始号
    clunum_fileinfo = (uint32_t)DBR_info.Cluster_Sector * (uint32_t)DBR_info.Sector_Byte /(uint32_t)file_info_len;
    clunum_byte = (uint32_t)DBR_info.Cluster_Sector * (uint32_t)DBR_info.Sector_Byte;
    Maxclunum = (uint32_t)DBR_info.Cluster_Num /  (uint32_t)DBR_info.Cluster_Sector ;//偏小一点 但是合理
    // printInPos(tmp , strlen(tmp) , 13 ,5);
    //---DBR数据验证 正确
    printintln((uint32_t)DBR_info.Sector_Byte);
    printintln((uint32_t)DBR_info.Cluster_Sector);
    printintln((uint32_t)DBR_info.Start_FAT);
    printintln((uint32_t)DBR_info.Num_FAT);
    printintln((uint32_t)DBR_info.Start_DBR);
    printintln((uint32_t)DBR_info.Cluster_Num);
    printintln((uint32_t)DBR_info.FAT_Size);
    printintln((uint32_t)DBR_info.Cluster_RootDir);
    //printintln((uint32_t)&DBR_info.Cluster_RootDir);

}

void init_fsinfo_info(){
    fsinfo_info_Struct tmp_fsinfo_info;
    // char * tmp = "bbbbbbbbbbb";
    // printInPos(tmp , strlen(tmp) , 14 ,5);
    char fsinfo[512]={0};
    bread(fsinfo_pos , fsinfo , 1);
    // printInPos(tmp , strlen(tmp) , 15 ,5);
    tmp_fsinfo_info.next_Cluster = (uint32_t)atoi_16_small(fsinfo+0x01EC , 4);
    tmp_fsinfo_info.available_Cluster = (uint32_t)atoi_16_small(fsinfo+0x01E8 , 4);
    fsinfo_info = tmp_fsinfo_info;
    clunum_available = fsinfo_info.available_Cluster;
    clunum_next = fsinfo_info.next_Cluster;
    // printInPos(tmp , strlen(tmp) , 16 ,5);
}

//扇区和簇相互转换
uint32_t SectorToCluster(uint32_t sec){
    uint32_t ret;
    //计算偏移的扇区
    sec = sec - DATA_pos;
    ret = sec/DBR_info.Cluster_Sector + DBR_info.Cluster_RootDir;
    return ret;
}
uint32_t ClusterToSector(uint32_t clu){
    int ret = 0;
    ret = ret + DATA_pos;
    ret = ret + (clu - DBR_info.Cluster_RootDir) * (uint32_t)DBR_info.Cluster_Sector;
}


//获得某个clu开始的第off个目录项的内容 //空的时候返回-1 错误 // 访问到下一个簇的时候返回 -1
uint32_t getfile_info(uint32_t clu , uint32_t off , file_info_Struct * finfo){
    //获取扇区值
    uint32_t lba = ClusterToSector(clu);
    if(off > clunum_fileinfo){
        return ERR;
    }
    //更改目录号为偏移量
    off = (off-1) *file_info_len;
    //临时缓冲区
    char tmp_finfo[file_info_len];
    bzero(tmp_finfo , file_info_len);
    //计算获取第几块合适
    uint32_t offbrk = (off / (uint32_t)DBR_info.Sector_Byte);
    //读取出来
    read_nbyte(offbrk + lba ,(off)%(uint32_t)DBR_info.Sector_Byte , file_info_len , tmp_finfo);
    //printintln(offbrk + lba);
    //print_n(tmp_finfo , file_info_len);
    //赋值
    //name 前8位是名字 之后的4位是后缀
    bzero(finfo->name_1 , 9);
    bzero(finfo->name_2 , 5);
    bzero(finfo->name_3 , 14);
    for(int i = 0;i<8;i++){
        finfo->name_1[i] = ((tmp_finfo[i] == (char)32)?(char)0:tmp_finfo[i]);
    }
    for(int i = 0;i<4;i++){
        finfo->name_2[i] = ((tmp_finfo[i+8] == (char)32)?(char)0:tmp_finfo[i+8]);
    }
    finfo->name_1[8] = (char)0;
    finfo->name_2[4] = (char)0;

    strncpy(finfo->name_3,finfo->name_1  ,strlen(finfo->name_1));
    if(strlen(finfo->name_2) != 0 ){
        finfo->name_3[strlen(finfo->name_3)] = (char)'.';
        strncpy(finfo->name_3 + strlen(finfo->name_3),finfo->name_2  ,strlen(finfo->name_2));
    }
    finfo->file_attr =(uint8_t)atoi_16_small(tmp_finfo+ 0x0B, 1);
    finfo->cluster_pos = (uint32_t) (uint16_t)atoi_16_small(tmp_finfo+ 0x14, 2);
    finfo->cluster_pos = finfo->cluster_pos * (uint32_t)65536;
    finfo->cluster_pos = finfo->cluster_pos + (uint32_t)(uint16_t)atoi_16_small(tmp_finfo+ 0x1A, 2);
    finfo->file_size =(uint32_t)atoi_16_small(tmp_finfo+ 0x1C, 4);
    return;
}

//找到这个文件的下一个簇
uint32_t getnextclu(uint32_t now_clu){
    char tmp_clunum[4];
    if(now_clu > Maxclunum){
        return ERR;
    }
    //计算偏移的块数
    //计算一张fat能存多少簇
    uint32_t offbrk = (now_clu+1)/fat_clunum + (((now_clu+1)%fat_clunum) == 0 ? 0 : 1) -1 ;
    read_nbyte(FAT1_pos + offbrk,now_clu%fat_clunum*4 , 4 , tmp_clunum);
    uint32_t ret = (uint32_t)atoi_16_small(tmp_clunum , 4);
    //printint(ret);
    //println();
    if(ret == end_clu){
        return ERR;
    }else{
        return ret;
    }
}

//返回文件名在这个簇中的偏移
uint32_t getoffinclu_byname(uint32_t now_clu , char * name , uint32_t n){
    uint32_t ret = 0;
    uint32_t offid = 0;
    file_info_Struct tmp_finfo;

    while((ret==0)){
        while(1){
            if(getfile_info(now_clu , offid , &tmp_finfo) == ERR){
                break;
            }else{
                if(strlen(name) == strlen(tmp_finfo.name_3)){
                    if(strncmp(tmp_finfo.name_3, name , strlen(name)) ==0 ){
                        ret = offid;
                        break;
                    }
                }
            }
            ++offid;
        }
        if(ret !=0){
            return ret;//找到了
        }else{
            return ERR;//没找到
        }
    }
}


//在当前目录下 返回文件名称对应的簇号 return -1 文件没找到 不存在
uint32_t getclu_byname(uint32_t now_clu , char * name , uint32_t name_len){
    file_info_Struct tmp_finfo;
    uint32_t offid = getoffinclu_byname(now_clu ,name , name_len);
    //printintln(offid);
    getfile_info(now_clu , offid , &tmp_finfo);
    //println(tmp_finfo.name_3);
    //如果是err 就返回err
    return tmp_finfo.cluster_pos;
}


//按照绝对路径找到这个文件目录项所在的簇
uint32_t getclu_bypath(char * path , uint32_t len){
    char path_name[20];
    char fapath[PATH_SIZ];
    uint32_t falen = 0;
    uint32_t offset = 0;
    uint32_t now_clu = DBR_info.Cluster_RootDir;
    if(len == 1){
        return now_clu;//根路径
    }
    falen = fatherpath(path , len , fapath);

    if(falen == 0){
        return now_clu;//根路径
    }
    //更改len
    while(offset < falen){
        offset++;//offset的位置一定是/
        uint32_t tmpnameoff = 0;
        while(offset<falen){
            if(fapath[offset] == '/'){
                break;
            }
            path_name[tmpnameoff] = fapath[offset];
            offset++;
            tmpnameoff++;
        }
        path_name[tmpnameoff] = (char)0;
        //println(path_name);
        now_clu = getclu_byname(now_clu , path_name , strlen(path_name ));
        if(now_clu == ERR){
            return ERR;
        }
    }
    return now_clu;
}

uint32_t Update_clunum_next(){
    uint32_t ret = 0;
    for( int i = 0;i<=Maxclunum;i++){
        if(getnextclu(i) == 0){
            ret = i;
            break;
        }
    }
    if(ret ==0){
        return ERR;
    }
    char clunum_next_arry[4];
    itoa_10_small(clunum_next_arry , 4 , ret);
    write_nbyte(fsinfo_pos, 0x01EC ,4 ,clunum_next_arry);
    clunum_next = ret;
    return ret;
}

uint32_t Update_clunum_available(int change ){
    clunum_available += change;
    char clunum_available_arry[4];
    itoa_10_small(clunum_available_arry , 4 , clunum_available);
    write_nbyte(fsinfo_pos, 0x01E8 ,4 ,clunum_available_arry);
    return clunum_available;
}

//把一个簇的内容读入到缓冲区里面
uint32_t read_clu(char * buffer , uint32_t clu){
    //堆栈区过小暂时禁用
    bread(ClusterToSector(clu) , buffer , (uint32_t)DBR_info.Cluster_Sector);
    // for(uint32_t i = 0;i<(uint32_t)DBR_info.Cluster_Sector;i++){
    //     bread(ClusterToSector(clu)+(uint32_t)i , buffer , 1);
    //     //printint(i);
    // }


    return 0;
}

//返回一个簇剩余空间 32的整数倍
uint32_t clu_available_size(uint32_t clu){
    uint32_t ret =0 ;
    char tmpbuffer[DBR_info.Sector_Byte];
    bzero(tmpbuffer , DBR_info.Sector_Byte);

    for(int i = DBR_info.Cluster_Sector-1;i>=0;i--){
        bread(ClusterToSector(clu) + i , tmpbuffer , 1);
        //print_n(tmpbuffer , 512);
        for(int j = DBR_info.Sector_Byte-1 ; j>=0;j--){
            if(tmpbuffer[j] == (char)0){
                ret++;
            }else{
                ret =ret/32*32;
                return ret;
            }
        }
    }
    ret =ret/32*32;
    return 0;
}
//返回一个簇最后的偏移量(簇内) 32的整数倍
uint32_t clu_off(uint32_t clu){
    int ret = clu_available_size(clu);
    if(ret ==0){
        return ERR;
    }else{
        return clunum_byte - ret;
    }
}

//新建
//扩充簇
uint32_t expand_clu(uint32_t now_clu){
    uint32_t TMP_clunum_next = clunum_next;
    //清空新的簇内容
    del_clu(TMP_clunum_next);
    change_fat_clu(now_clu , TMP_clunum_next);
    change_fat_clu(TMP_clunum_next , end_clu);
    //更新
    Update_clunum_next();
    Update_clunum_available(-1);
    return TMP_clunum_next;
}
//新开辟簇
uint32_t create_clu(){
    uint32_t TMP_clunum_next = clunum_next;
    //清空新的簇内容
    del_clu(TMP_clunum_next);
    change_fat_clu(clunum_next , end_clu);
    //更新
    Update_clunum_next();
    Update_clunum_available(-1);
    return TMP_clunum_next;
}

//新建文件 提供路径 名字 后缀 属性
uint32_t create_file(char* path , uint32_t pathlen , char*name, uint32_t namelen ,uint8_t attr){
    int clu = getclu_bypath(path, pathlen);
    //找到这个文件的最后一个簇
    while(1){
        uint32_t tmp = getnextclu(clu);
        if(tmp == ERR){
            break;
        }else{
            clu = tmp;
        }
    }

    int avail_siz = clu_available_size(clu);
    uint32_t off = 0;
    if(avail_siz <= file_info_len){//不足
        clu = expand_clu(clu);
        off = 0;
    }else{
        off = clu_off(clu);
    }
    char tmp_buffer[file_info_len];
    char tmp_file[file_info_len];
    char tmp_name1[8];
    char tmp_name2[4];
    int tmp_name1len , tmp_name2len;

    //清空
    bzero(tmp_file ,file_info_len );
    //处理名字部分
    for(int i =0;i<namelen;i++){
        if(name[i] == '.'){
            break;
        }else{
            tmp_name1[tmp_name1len++] = name[i] ;
        }
    }
    for(int i =tmp_name1len;i<=7;i++ ){
        tmp_name1[i] = 32;
    }

    strncpy(tmp_file ,tmp_name1 , 8 );
    //处理后缀部分
    for(int i =tmp_name1len+1;i<namelen;i++){
        tmp_name2[tmp_name2len++] = name[i] ;
    }
    for(int i =tmp_name2len;i<=3;i++ ){
        tmp_name2[tmp_name2len++] = 32;
    }
    strncpy(tmp_file+8 ,tmp_name2 , 4 );

    //处理属性
    itoa_10_small(tmp_buffer , 1 , attr );
    strncpy(tmp_file+0x0B ,tmp_buffer , 1);

    //文件簇号
    itoa_10_small(tmp_buffer , 4 , clu);
    strncpy(tmp_file+0x14 ,tmp_buffer , 2);
    strncpy(tmp_file+0x1A ,tmp_buffer+2 , 2);

    strncpy(tmp_file+0x1A ,tmp_buffer+2 , 2);

    write_nbyte(get_sec_afteroff(clu , off) , off%DBR_info.Sector_Byte , file_info_len ,tmp_buffer );
}

uint32_t get_sec_afteroff(uint32_t clu , uint32_t off){
    uint32_t sec = ClusterToSector(clu);//获得起始扇区
    uint32_t offbrk = off/DBR_info.Sector_Byte + (((off%DBR_info.Sector_Byte) == 0)? 0 : 1);
    return sec + offbrk;
}

//删除
//删除文件
uint32_t del_file(char * path , uint32_t pathlen , char*name, uint32_t namelen){
    //获得clu
    char farpath[pathlen];
    bzero(farpath , pathlen);
    uint32_t farlen = fatherpath(path , pathlen , farpath);
    uint32_t clu = getclu_bypath(farpath ,  farlen);
    //获得偏移量
    uint32_t off = getoffinclu_byname(clu , name , namelen );
    //获得扇区
    uint32_t sec = get_sec_afteroff(clu , off);
    //改变标记
    char tmpbuffer[2];
    tmpbuffer[1] = 0xE5;
    write_nbyte(sec , off%DBR_info.Sector_Byte ,1 , tmpbuffer);

    //找到这个文件的簇
    uint32_t file_clu = getclu_bypath(path , pathlen);

    uint32_t next_clu = file_clu;
    while(next_clu != ERR ){
        uint32_t now_clu = next_clu;
        next_clu = getnextclu(now_clu);
        change_fat_clu(now_clu , 0);
        //更新
        Update_clunum_available(+1);
    }
    Update_clunum_next();
}

//删除目录
uint32_t del_dir(char * path , uint32_t pathlen ,char*name, uint32_t namelen){
    //遍历这个目录的每一个簇
    file_info_Struct finfo;

    char namebuffer[MAX_BUFFER];
    char pathbuffer[MAX_BUFFER];
    char newpathbuffer[MAX_BUFFER];
    int now_clu =  getclu_bypath(path , pathlen);
    int next_clu = now_clu;
    while(next_clu!= ERR){
        now_clu = next_clu;
        next_clu = getnextclu(now_clu);
        //遍历这个簇里面的所有文件
        uint32_t off = clu_off(now_clu);
        uint32_t filemax = off / file_info_len;
        for(int i = 0;i<filemax ;i++){
            getfile_info(now_clu ,i , &finfo );
            strncpy(namebuffer ,finfo.name_3 , strlen(finfo.name_3 ) );
            uint32_t newpathlen = strncat(pathbuffer , namebuffer , newpathbuffer , strlen(finfo.name_3 ) +pathlen );
            if(finfo.file_attr & 0x10){//目录
                del_dir(newpathbuffer , newpathlen , finfo.name_3 , strlen(finfo.name_3 ));
            }else{
                del_file(newpathbuffer , newpathlen , finfo.name_3 , strlen(finfo.name_3 ));
            }
        }
        //更新
        change_fat_clu(now_clu , 0);
        Update_clunum_available(+1);
    }
    Update_clunum_next();
}
//改变fat表
void change_fat_clu(uint32_t clu , uint32_t x){
    char tmp_clunum[4];
    itoa_10_small(tmp_clunum , 4 , x);
    //计算偏移的块数
    //计算一张fat能存多少簇
    uint32_t offbrk = (clu+1)/fat_clunum + (((clu+1)%fat_clunum) == 0 ? 0 : 1) -1 ;
    write_nbyte(DBR_info.Start_FAT + offbrk,clu%fat_clunum*4 , 4 , tmp_clunum);
}

//清空一个簇
void del_clu(uint32_t clu){
    char tmp_clear[DBR_info.Sector_Byte];
    bzero(tmp_clear , DBR_info.Sector_Byte);
    uint32_t sec = ClusterToSector(clu);
    bwrite(sec , tmp_clear , DBR_info.Cluster_Sector);
}

