#include "stringio_h.h"
#include "system_h.h"
extern uint32_t cmd_ls(char *p);
extern uint32_t cmd_cd(char *p);
extern void cmd_mkdir(char *p);
extern void cmd_touch(char *p);
extern void clearScreen();
extern void poweroff();

char global_path[PATH_SIZ] = {0};

void startup(){
    char * title = "hello , welcome to ljy_os";
    char * subtitle = "designed by ljy";
    char * hint = "loading system..";
    char * nt = "Press enter..\r\n";
    printInPos(title , strlen(title) , 5 ,5);
    printInPos(subtitle , strlen(subtitle) , 6 ,5);
    printInPos(hint , strlen(hint) , 7 , 5);
    printInPos(nt , strlen(nt) , 8 , 5);
	// tmp_global_path_char[0]=(char)'.';
	// tmp_global_path_char[1]=(char)'/';
	// tmp_global_path_char[2]=0;
    // strncpy(global_path ,tmp_global_path_char, 2);
	//初始化文件系统
	// init_DBRinfo();
	// init_fsinfo_info();
	//初始化根路径
	global_path[0] = '/';
	global_path[1] = 0;
}

void show_p(){
    char * p_str = "LJY_OS :";
    char * p_end = "#";
	char * tmp_path = global_path;
    print(p_str);
    print(tmp_path);
    print(p_end);
}

// help me
void showHelp(){
	char * help = "shell for OS version 1.4 x86 PC\r\n"
	"This is the shell command!\r\n"
	"Use `help` to see the list \r\n"
	"\r\n"
	"\rcls - clear the ternimal \r\n"\
	"\r\n";
	print(help);
}

void shell(){
	clearScreen();
	showHelp();
	char cmdStr[BUF_SIZ + 1] = {0};
	char cmdFirstWord[BUF_SIZ + 1] = {0};
    char cmdSecondWord[BUF_SIZ + 1] = {0};
	char tmp_path[BUF_SIZ + 1] = {0};
	//char global_path[PATH_SIZ] = {0};
	enum{help,clear,power_off,ls,cd,touch,mkdir};
	char*command[] = {"help","cls","poweroff","ls","cd","touch","mkdir"};
	while(1){
		show_p();
		readBuf(cmdStr,BUF_SIZ);
		getFirstWord(cmdStr,cmdFirstWord);
		if(strncmp(cmdFirstWord,command[clear],min( strlen(command[clear]) ,strlen(cmdFirstWord) )) == 0){
			clearScreen();
		}
		else if(strncmp(cmdFirstWord,command[help] ,min( strlen(command[help]) ,strlen(cmdFirstWord) )) == 0){
			showHelp();
		}
		else if(strncmp(cmdFirstWord,command[power_off],min( strlen(command[power_off]) ,strlen(cmdFirstWord) )) == 0){
        	poweroff();
		}
        else if(strncmp(cmdFirstWord,command[ls],min( strlen(command[ls]) ,strlen(cmdFirstWord) )) == 0){
            getFirstWord(cmdStr + strlen(cmdFirstWord) + 1 ,cmdSecondWord);
            cmd_ls(cmdSecondWord);
        }
        else if(strncmp(cmdFirstWord,command[cd],min( strlen(command[cd]) ,strlen(cmdFirstWord) )) == 0){
            getFirstWord(cmdStr + strlen(cmdFirstWord) + 1 ,cmdSecondWord);
            cmd_cd(cmdSecondWord);
        }
		else if(strncmp(cmdFirstWord,command[touch],min( strlen(command[touch]) ,strlen(cmdFirstWord) )) == 0){
            getFirstWord(cmdStr + strlen(cmdFirstWord) + 1 ,cmdSecondWord);
            cmd_touch(cmdSecondWord);
        }
        else if(strncmp(cmdFirstWord,command[mkdir],min( strlen(command[mkdir]) ,strlen(cmdFirstWord) )) == 0){
            getFirstWord(cmdStr + strlen(cmdFirstWord) + 1 ,cmdSecondWord);
            cmd_mkdir(cmdSecondWord);
        }
		else{
			if(cmdFirstWord[0] != 0){
				char*errMsg = "command not found\r\n";
				print(cmdFirstWord);
				print(errMsg);
			}
		}
	}
}