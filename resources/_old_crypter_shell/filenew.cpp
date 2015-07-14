

#include "stdafx.h"

#include "strlist.h"



#define RECURSE 1


#define FCHRL(str,pstr,ch) do {\
	pstr = str + strlen(str);\
	while(pstr != str && (*pstr )!=ch) {\
		pstr--;\
	}\
}while(0)
#define FCHRR(str,pstr,ch) do {\
	pstr = str;\
	while((*pstr) && (*pstr) !=ch) {\
		pstr++;\
	}\
}while(0)

#define FCHRRNOT(str,pstr,ch) do {\
	pstr = str;\
	while((*pstr) && (*pstr) == ch) {\
		pstr++;\
	}\
}while(0)

#define FCHRLNOT(str,pstr,ch) do {\
	pstr = str + strlen(str);\
	while(pstr != str && (*pstr ) ==ch) {\
		pstr--;\
	}\
}while(0)

/*
Clears a char in a string
*/


/*void clear_chr(char* str,char chr) {
	char  *pstr = NULL;
	char  *pstrn = NULL;
	do {
		FCHRR(str,pstr,chr);
		if((*pstr)) {
			(*pstr) = '\0';
			pstrn = pstr +1;
			strcat(pstr ,pstrn);
		}
		
	}while((*pstr));
}
*/




#define CHECK(p) ((!strcmp(p,".")) || (!strcmp(p,"..")))?1:0
#define GETEXT(pstr,str) do { pstr = str + strlen(str); while((*pstr) != '.' && pstr!=str) pstr--;  }while(0);



int dfw_getpath_rec(char *fpath,slist * plext,DWORD opt) {
	HANDLE hFind ;
	WIN32_FIND_DATA  fdata;
	
	char spath[200];
	char sfile[200];
	char next_path[MAX_PATH];
	char *pstr = NULL;
	int br = 0; //DEBUG
	snode *pnode = NULL;
	
	strcpy(spath,fpath);
	strcat(spath,"*.*");
	///
/*	printf("\t>\"%s\"\n",spath);
//	getch();
	sl_for_each(plext,pnode) {
		printf("\t<->\"%s\"\n",pnode->string);
	}*/
	///
	hFind = FindFirstFile(spath, &fdata);
	if(hFind!=INVALID_HANDLE_VALUE)
	do{
		if(!CHECK(fdata.cFileName)){
			if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				wsprintf(next_path,"%s%s\\",fpath,fdata.cFileName);
				if(opt == RECURSE)
					dfw_getpath_rec(next_path,plext,opt);
			}else {
				GETEXT(pstr,fdata.cFileName);
				_strlwr(pstr);
				printf("\"%s%s\"\n",fpath,fdata.cFileName);
				//getch();
				//printf(" GETEXT = %s\n",pstr);
				sl_for_each(plext,pnode) {

				}
			}
		}
	}while(FindNextFile(hFind,&fdata));
	FindClose(hFind);

	return 0;
	//return 0;
}
int dfw_getpath(char *fpath) {
	char ext[200];
	char path[200];
	char tmp[50];
	char sopt[50];
	DWORD opt=0;
	slist lext;

	snode *pnode = NULL;
	
	char *pstrb = NULL;
	char *pstre = NULL;

	
	
	sl_init(&lext);
	
	memset(sopt, 0,sizeof(sopt));
	memset(ext,0,sizeof(ext));
	memset(path,0,sizeof(ext));
	FCHRL(fpath,pstrb,'\\');
	strcpy(ext,pstrb + 1);
	strncpy(path,fpath,pstrb - fpath + 1);

	pstre = ext;
	pstrb = ext;
//	printf("\"%s\" ,'%c'\n",pstrb,*pstrb);
	FCHRL(ext, pstrb,' ');
//	printf("\"%s\" ,'%c'\n",pstrb,*pstrb);
	pstre = pstrb;
	FCHRRNOT(pstre,pstrb, ' ');
//	printf("\"%s\" ,'%c'\n",pstrb,*pstrb);
	

	if((*pstrb)) {
		*(pstrb-1) = '\0';
		strcpy(sopt,pstrb);
		if(sopt[1] == 'r'){
			printf("REC \n"); //DEBUG
			opt = 1;
		}
	}
	pstre = ext;
	pstrb = ext;


	do {
		memset(tmp,0,sizeof(tmp));
		FCHRR(pstrb,pstre,'|');
		strncpy(tmp,pstrb,pstre - pstrb );
		pstrb = pstre +1;
		clear_chr(tmp, ' ');
		if(strlen(tmp))
			if(tmp[0] == '*'){
				_strlwr(tmp + 1);


				sl_addstr(&lext,tmp + 1);
			}
			else{
			sl_addstr(&lext,tmp );
			}
	}while((*pstre));
	///
	printf("\t>[%s]  opt = 0x%X\n\n",path,opt);
	sl_for_each((&lext),pnode) {
		printf("\t>\"%s\"\n",pnode->string);
	}

	getche();
	printf("%s, %d\n",path,opt );
	dfw_getpath_rec(path,&lext,opt);
	sl_destroy(&lext);
	return 0;
}


