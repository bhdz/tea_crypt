// crypt2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "crypt.h"
#include "strlist.h"
#include "files.h"
#include "prs.h"



#define CMD_FCODE 1
#define CMD_FDECODE 2
#define CMD_EXIT 3

char curpath[MAX_PATH];
int get_pwd(char *str,int memsz);
int GetCmd2(char *cmdline, slist *plist) {
	char *pstr = cmdline;
	char *pstrb = cmdline;
	int iret = 0;
	int stop = 0;
	char tmp[200];
	memset(tmp,0,sizeof(tmp));
	sl_destroy(plist);
	//// FIRST GET COMMAND
	while((*pstr) == ' ' && (*pstr) != '\0') 
		pstr ++;
	if(!(*pstr)|| (*pstr) == ' ') 
		return 0;
	pstrb = pstr;
	while((*pstr) != ' ' && (*pstr) != '\0') 
		pstr++;
	strncpy(tmp,pstrb,(int)(pstr - pstrb ));///Copy 1
	sl_addstr(plist,tmp,0);
	pstrb = pstr;
	iret = 1;
	
	do {
		while((*pstr) != '\"' && (*pstr)) 
			pstr ++;
		if(!(*pstr)) {
			stop = 1;
		}
		else {
			pstrb = ++pstr;
			while((*pstr) != '\"' && (*pstr) ) 
				pstr++;
			strncpy(tmp,pstrb,(int)(pstr - pstrb ));///Copy 1
			sl_addstr(plist,tmp,0);
			pstrb = ++pstr;
			iret++;
		}
	}while(!stop); 
	return iret;
}

int crs_set(HANDLE hstdOut,COORD crd){
	return SetConsoleCursorPosition(hstdOut,crd);;
}
COORD crs_get(HANDLE hstdOut) {
	CONSOLE_SCREEN_BUFFER_INFO pci;
	GetConsoleScreenBufferInfo(hstdOut,&pci);
	return pci.dwCursorPosition;
}
int crs_setcinf(HANDLE hstdOut,DWORD sz,BOOL vis) {
	CONSOLE_CURSOR_INFO pci;
	pci.bVisible = vis;
	pci.dwSize = sz;
	return SetConsoleCursorInfo(hstdOut,&pci);
}
int clrscr(HANDLE hstdOut) 
{ 

  HANDLE hndl = hstdOut;//GetStdHandle(STD_OUTPUT_HANDLE); 
  CONSOLE_SCREEN_BUFFER_INFO csbi; 
  GetConsoleScreenBufferInfo(hndl, &csbi); 
  DWORD written; 
  DWORD N = csbi.dwSize.X * csbi.dwCursorPosition.Y + 
            csbi.dwCursorPosition.X + 1; 
  COORD curhome = {0,0}; 

  FillConsoleOutputCharacter(hndl, ' ', N, curhome, &written); 
  csbi.srWindow.Bottom -= csbi.srWindow.Top; 
  csbi.srWindow.Top = 0; 
  SetConsoleWindowInfo(hndl, TRUE, &csbi.srWindow); 
  SetConsoleCursorPosition(hndl, curhome); 
  return 0; 
} 
#define PERCENT(I,MAX) ((double)((double)I/(double)MAX) * 100.0) 

int GetCmd(char *cmdline, char *cmd,char *prm1,char *prm2) {
	char *pstr = cmdline;
	char *pstrb = cmdline;
	int iret = 0;
	//// FIRST
	while((*pstr) == ' ' && (*pstr) != '\0') 
		pstr ++;
	if(!(*pstr)|| (*pstr) == ' ') 
		return 0;
	pstrb = pstr;
	while((*pstr) != ' ' && (*pstr) != '\0') 
		pstr++;
	strncpy(cmd,pstrb,(int)(pstr - pstrb ));///Copy 1
	pstrb = pstr;
	//// SECOND
	while((*pstr) != '\"' && (*pstr)) 
		pstr ++;
	if(!(*pstr)) 
		return 1;
	else {
		pstrb = ++pstr;
	}
	while((*pstr) != '\"' && (*pstr) ) 
		pstr++;
	strncpy(prm1,pstrb,(int)(pstr - pstrb  ));///Copy 2
	pstrb = ++pstr;
	//// THIRD
	while((*pstr) != '\"' && (*pstr)) 
		pstr ++;
	if(!(*pstr)) 
		return 2;
	else {
		pstrb = ++pstr;
	}
	while((*pstr) != '\"' && (*pstr) ) 
		pstr++;
	strncpy(prm2,pstrb,(int)(pstr - pstrb ));///Copy 3
	
	////
	return 3;
}

/*int TEAEncryptF(BYTE *val,DWORD vlen, BYTE *key) {
	unsigned char *ptr;
	for(ptr = val; (ptr + 8) <= (val + vlen); ptr+=8) {
		CODE(ptr,key);
	}	
	return (int)((val + vlen) - (ptr) );
}*/
/*
crypt
*/

/*
int main(int argc, char* argv[]){
	char str[] = "Test Strinfghfdghdfghdfghfdghfghfdgghdfghfhg no 1!!! ";
	char key[] = "key";
	char *out = NULL, *out2 = NULL;
	int slen = strlen(str);
	int klen = strlen(key);
	
	printf("str: \"%s\", key: \"%s\"\n",str,key);
	out = (char*) _code((BYTE*)str,slen,(BYTE*)key);
	printf("str: \"%s\"\n",out,key);
	slen = strlen(out);
	
	
	out2 = (char*)_decode((BYTE*)out,slen+1,(BYTE*)key,slen);
	printf("str: \"%s\", key: \"%s\"\n",out2,key);

	free(out2);
	free(out);
	
	//15 = ((i/8)*8) + (i %8)
	return 0;
}*/
int readbl = CBLOCK_STD;

/*
A simple shell
*/

void display_help() {
	printf("TEA encrypt engine v1.1.1.1\nUsage:\n\n");
	printf("exit\n\tDescription : Terminates the program\n\n");

	printf("code:\n"
"\tDescription: Encodes a path or a file\n"
"\tparameters: \"path or file [-switch]\" "
"\"password\"\n"
"\tswitches:\n\n"
"\t\"|\" - file name delimiter\n\n"
"\tex: 'code \".\\readme.txt|foo.txt\" \"pass\"'\n"
"\t\twill code every file that is \n"
"\t\tnamed \"readme.txt\" or \"foo.txt\" in the \n"
"\t\tcurrent path with \"pass\" password\n");
	_getch();
printf("\t\"-r\" - recursive action \n"
"\t\tthis switch allow the code func to \n"
"\t\tencode sub-directories\n\n"
"\tex: 'code \"readme.txt|foo.txt -r\" \"pass\"'\n"
"\t\twill code every file that is \n"
"\t\tnamed \"readme.txt\" or \"foo.txt\"  the \n"
"\t\tcurrent path and its sub-dirs\n"
"\t\twith \"pass\" password\n\n");
_getch();
printf("\t\"*\" - every file that matches extension\n\n"
"\tex:'code \".\\readme.txt|*.zip|install.* -r\"\n"
"\t\twill code every readme.txt every *.zip \n"
"\t\tevery instal.* in path \".\\\" with its sub\n"
"\t\tdirectories\n\ndecode: See above and substitute\n"
"\tcode with decode...\n"
);


}


int CheckFileExist(char *path){
	WIN32_FIND_DATA fnd;
	if(FindFirstFile(path,&fnd)==INVALID_HANDLE_VALUE)
		return 0;
	if(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return 1;
	else return 2;
}
HANDLE g_hstdOut = NULL;//GetStdHandle(STD_OUTPUT_HANDLE);

#define GET_MSECS(time) (time % 1000)
#define GET_SECS(time) ((time <60000)? time/1000: time % 60000)
#define  GET_MINS(time) ((time <3600000)? time / 60000:( time % 3600000) /60000)
#define GET_HOURS(time) (time / 3600000)

int TEACallback(unsigned int msg,char *fname,long param1,long param2){
static COORD cur_pos;
	unsigned int call_id = msg & 0xFFFF;
	unsigned int call_type = (msg >> 16);
	switch(call_id){
	case TEACALL_START:
		printf("\tCallback started: (0x%.8X)\n",TEACallback);
		break;
	case TEACALL_STOP:
		printf("\tCallback stopped: (0x%.8X)\n",TEACallback);
		break;
	case TEACALL_CFILE:
	case TEACALL_DFILE:
		{
			switch(call_type){
			case TEACALLT_BEGIN:
				crs_setcinf(g_hstdOut,1,0);
				printf("[%s] ",fname);
				cur_pos = crs_get(g_hstdOut);
				break;
			case TEACALLT_STAT:
				crs_set(g_hstdOut,cur_pos);
				printf("%.1f%%, %d/%d", PERCENT(param1,param2),param1,param2   );
				break;
			case TEACALLT_COMPLETE:
				crs_set(g_hstdOut,cur_pos);
				printf("%.1f%%, ", PERCENT(100,100));
				if(call_id == TEACALL_DFILE)
					printf("de");
				printf("coding time %.2d:%.2d:%.2d:%.4d \n",
					GET_HOURS(param1),GET_MINS(param1),GET_SECS(param1),GET_MSECS(param1));
				crs_setcinf(g_hstdOut,1,1);
				break;
			case TEACALLT_ERROR:
				crs_set(g_hstdOut,cur_pos);
				printf("ERROR : \"%s\"\n",tea_geterrstr(param1));
				//printf("ERROR : \n");
				crs_setcinf(g_hstdOut,1,1);
				break;
			}
		}
		break;
	case TEACALL_CPATH:
	case TEACALL_DPATH:
		{
			switch(call_type){
			case TEACALLT_BEGIN:
				printf("\"%s\"\n\n",fname);
				break;
			case TEACALLT_COMPLETE:
				printf("\nTotal time %.2d:%.2d:%.2d:%.4d \n",
					GET_HOURS(param1),GET_MINS(param1),GET_SECS(param1),GET_MSECS(param1));

				break;
			case TEACALLT_ERROR:
				printf("ERROR : \"%s\"\n",tea_geterrstr(param1));
				//printf("ERROR : \n");
				break;
			}
		}
		break;
	}
	return 1;//return 0 to break execution of the caller
}
int main(int argc, char* argv[])
{
	int exit= 0;
	char sline[200];
	char cmd[100];
	char param1[100];
	char param2[100];

	char **params;

	params = (char**)LocalAlloc(LPTR,sizeof(char*) * 20);
	for(int I=0; I<20; I++){
		params[I] = (char*)LocalAlloc(LPTR,MAX_PATH  + 40);
	}
	tea_pfcall_start(TEACallback);
	g_hstdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD Popt = 1;
	slist list;
	snode *pnode = NULL;
	sl_init((&list));
	GetCurrentDirectory(MAX_PATH,curpath);
	DWORD opstart=0, opend=0;
	int iret = 0;
	int fh = -1;
	int i=0;
	int r = 0;
//	printf("TEA File Encryptor : %s\n\n",VER_STR);
	printf("\tRBL is %d bytes\n",readbl);
	printf("\ttype \"help\" for help!\n");
//	printf("\tInit: Path Options are 0x%X\n",Popt);
//	printf("\ttype help for help!\n");
	do {
		printf(">");
		memset((void*)sline,0,sizeof(sline));
		gets(sline);
		memset((void*)cmd,0,sizeof(cmd));
		memset((void*)param1,0,sizeof(param1));
		memset((void*)param2,0,sizeof(param2));
		iret = GetCmd(sline,cmd,param1,param2);
		sl_destroy((&list));
		GetCmd2(sline,&list);
//		printf("\t\"%s\", \"%s\", \"%s\" : (%d)\n",cmd,param1,param2,iret);
		if(!iret ) {
		//	printf("\tERROR:ENTER A COMMAND!\n");
		}else {

			if(!strcmp(cmd,"fencode")) {///ENCODING
				if(iret != 3) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}else {
					if(strlen(param2) > 16) {
						printf("\tERROR: password must be %d\n!",16);
						
					}else {
						printf("\t%s(%s)\n",cmd,param1);
						opstart = GetTickCount();
						r = tea_fcode(param1,param2,readbl);
						if(r == -1) {
							printf("\tERROR: %s -> File allready encoded!\n",cmd);
						}else if(r == -3) {
							printf("\tERROR: %s -> Can't open %s\n",cmd,param1);
						}else {
							opend = GetTickCount();
							printf("\t%s -> Done! ( %f secs )\n",cmd,((float)opend - (float)opstart) / 1000.0);
						}

					}
				}
			}else if(!strcmp(cmd,"fdecode")) { ////DECODING

				if(iret != 3 ) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}else {
					if(strlen(param2) > 16) {
						printf("\tERROR: password must be %d\n!",16);
						
					}else {
						printf("\t%s(%s)\n",cmd,param1);
						
						opstart = GetTickCount();
						r = tea_fdecode(param1,param2);
						printf("ret = %d\n",r);
						if(r == -3) {
							printf("\tERROR: %s -> Can't open %s\n",cmd,param1);
						}
						else if(r ==-1) {
							printf("\tERROR: %s -> Bad Password!\n",cmd);
						}else if(r == -2) {
							printf("\tERROR: %s -> Bad Header!\n",cmd);
						}
						else if(r == 1){
							opend = GetTickCount();
							printf("\t%s -> Done! ( %f secs )\n",cmd,((float)opend - (float)opstart) / 1000.0);
						}
					}
				}
			}else if(!strcmp(cmd,"code")) {

				if(iret != 2 ) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}else {
					char pwd[17];
					int pwr = 0;
					memset(pwd,0,sizeof(pwd));
					do{
						printf("Enter password: ");
						pwr = get_pwd(pwd,sizeof(pwd));
						printf("\n");
						if(pwr == -1){
							printf("Error: Cannot exede %d chars !\n",sizeof(pwd)-1);
						}
						else if(pwr ==-2){
							printf("Canceling\n");
							break;
						}
					}while(pwr !=1);
					if(pwr == 1){
//						printf("%s\n",pwd);
						tea_pcode(param1,pwd,readbl);
					}
				}
			}else if(!strcmp(cmd,"path") ) {
				GetCurrentDirectory(MAX_PATH, curpath);
				printf("\"%s\"\n",curpath);

			}else if(!strcmp(cmd,"decode")) {
				if(iret != 2 ) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}else {
					char pwd[17];
					int pwr = 0;
					memset(pwd,0,sizeof(pwd));
					do{
						printf("Enter password: ");
						pwr = get_pwd(pwd,sizeof(pwd));
						printf("\n");
						if(pwr == -1){
							printf("Error: Cannot exede %d chars !\n",sizeof(pwd)-1);
						}
						else if(pwr ==-2){
							printf("Canceling\n");
							break;
						}
					}while(pwr !=1);
					if(pwr == 1){
				//		printf("%s\n",pwd);
						tea_pdecode(param1,pwd);
					}

				}
			}else if(!strcmp(cmd,"help")) {
				display_help();
			}else if(!strcmp(cmd,"search")) {
				if(iret < 2) {
					
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}
				else {
					printf("\tSearching %s...\n\n",param1);
					slist lsrch;
					snode *pnode = NULL;
					tea_psearch(param1,&lsrch);
					i=0;
					if(!lsrch.cnt) {
						printf("\t%s -> Nothing found in %s\n",cmd,param1);
					}else
					sl_for_each((&lsrch),pnode) {
						i++;
						printf("[%s]\n",pnode->string);
						if(!(i % 22)) {
							printf("Press any key to continue...\n\n");
							_getche();
						}
					}
					sl_destroy((&lsrch));
				}
			}
			else if(!strcmp(cmd,"pmap")) {
				if(iret !=2) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}
				else {
					slist lst ;
					snode *pnode = NULL;

					sl_init((&lst));
					fw_getpath(param1,&lst);
					printf("\t%d files mapped\n\ttotal memory for list %d\n",lst.cnt,(int)sl_calcmem(&lst)) ;
					printf("\tDo you want to veiw them?\n\tPress [Y]es or [N]o\n");
					char an= getch();

					i=0;

					if(an == 'y' || an == 'Y')
					sl_for_each((&lst),pnode) {
						printf("[%s]\n",pnode->string);
						
						if(!(i%22) && i){
							printf("Press any key to continue...\n\n");

							getch();
						}
						i++;
					}
					sl_destroy((&lst));

				}
			}else if(!strcmp(cmd,"ptest")){
				slist lst ;
				snode *pnode = NULL;
				int i=0;
				sl_init((&lst));
				fw_getpath(param1,&lst);
				sl_for_each((&lst),pnode) {
						printf("[%s]\n",pnode->string);
						
						if(!(i%22) && i){
							printf("Press any key to continue...\n");
							getch();
						}
						i++;
					}
				printf("\ntotal number of files : %d\n",lst.cnt);
				sl_destroy((&lst));

			}
			else if(!strcmp(cmd,"set")) {
				if(iret != 3) {
					printf("\tERROR:%s -> Not enough parameters!\n",cmd);
				}
				else {
					if(!strcmp(param1,"readblock")) {
						readbl = atoi(param2);
						readbl *=8;
						if(readbl < RBL_MIN || readbl > RBL_MAX)  {
							printf("\tERROR:%s -> Please enter a number between %d and %d!\n",cmd,RBL_MIN/8,RBL_MAX/8);
							readbl = CBLOCK_STD;
						}
						else {
							printf("\t%s -> Read Block is now %d bytes\n",cmd,readbl);

						}
					}
					else if(!strcmp(param1,"popt")) {
						Popt = atol(param2);
						printf("\tPath Options are 0x%X now\n",Popt);
					}
					else {
						printf("\tERROR: %s ->Bad Parameter %s !\n",cmd,param1);
					}
				}

			}
			else if(!strcmp(cmd,"exit")) {
				printf("\tTerminating ...\n");
				exit = 1;
			}
			else if(!strcmp(cmd,"split")) {
				char **strs = NULL;
				int cnt;

				strs = strsplitch(param1,param2[0],&cnt);
				for(int i=0; i<cnt; i++) {
					printf("\"%s\"\n",strs[i]);
					free(strs[i]);
				}
				free(strs);
			}
			else if(!strcmp(cmd,"ren")) {
				fw_ren_files(param1,param2);
			}
			else if(!strcmp(cmd,"cd")) {
				if(!SetCurrentDirectory(param1)) {
					printf("\tInvalid directory!\n");
				}
				else {
					GetCurrentDirectory(MAX_PATH, curpath);
					printf("%s>\n",curpath);
				}
			}
			else if(!strcmp(cmd,"cd..")) {
				SetCurrentDirectory("..");
				GetCurrentDirectory(MAX_PATH,curpath);
				printf("%s>\n",curpath);
			}
			else if(!strcmp(cmd,"lsort")) {
				slist lst;
				DWORD start=0,end = 0;
				snode *pnode = NULL;
				sl_init((&lst));
				fw_getpath(param1, &lst);
				start = GetTickCount();
				sl_sort(&lst);
				end = GetTickCount();
				printf("time: %d\n\n",end - start);
				printf("do you want listing? \n");
				if(getch() =='y')
				sl_for_each((&lst),pnode) {
						i++;
						printf("[%s]\n",pnode->string);
						if(!(i % 22) && i) {
							printf("Press any key to continue...\n\n");
							_getche();
						}
				}
				

				sl_destroy((&lst));
			}
			else { 
				
				printf("\tBAD COMMAND!\n");
			}
		}
		sl_destroy((&list));
	}while(!exit);
	sl_destroy((&list));
	for(I=0; I<20; I++){
		params[I] = (char*)LocalAlloc(LPTR,MAX_PATH  + 40);
	}
	return 0;
}


int get_pwd(char *str,int memsz)
{
	char *pstr = str;
	int ret = 1;
	char chr = '\0';
	while(1){
		chr=getch();
	//	printf("(%d)",chr);
		if(chr <32 || chr > 126){
			if(chr == 8) {
				if(pstr!=str){
					putch(8);
					putch(0);
					putch(8);
					pstr--;
				}
				continue;
			}
			else if(chr == 27){
				ret = -2;
			}
			break;
		}
		else if(pstr - str > memsz -2){
			ret = -1;
			break;//lenght is bigger
		}
		else *pstr = chr;
		
		putch('*');
		pstr++;
	}
	*pstr = '\0';
	return ret;
}

