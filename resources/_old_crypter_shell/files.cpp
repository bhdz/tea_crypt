


#include "stdafx.h"
#include "files.h"
#include "strlist.h"
#include "prs.h"
#include <malloc.h>



#define CHECK(p) (((!strcmp(p,".")) || (!strcmp(p,"..")))?1:0)


/*
Clears a char in a string
*/

#define OPT_REC 1
#define OPT_FOLDERS 2
#define OPT_NOPARENT 4
#define OPT_MARKFOLDERS 8


static int fw_getpath_rec(char *fpath,char **ext,int extn,DWORD opt,slist *plist) {
	HANDLE hFind ;
	WIN32_FIND_DATA  fdata;
	
	char spath[200];
//	char sfile[200];
	char next_path[MAX_PATH];
	char *pstr = NULL;
	strcpy(spath,fpath);
	strcat(spath,"*.*");
	hFind = FindFirstFile(spath, &fdata);
	if(hFind!=INVALID_HANDLE_VALUE)
	do{
		if(!CHECK(fdata.cFileName)){
			if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				wsprintf(next_path,"%s%s\\",fpath,fdata.cFileName);
				if(opt & OPT_FOLDERS) {
					if(opt & OPT_NOPARENT) strcpy(spath,fdata.cFileName);
					else wsprintf(spath,"%s%s",fpath,fdata.cFileName);
					if(opt & OPT_MARKFOLDERS) {
						strcat(spath,">");
					}

					sl_addstr(plist,spath,1);
				}
				if(opt & OPT_REC){
					fw_getpath_rec(next_path,ext,extn,opt,plist);
				}
			}else {
				
				FCHRL( fdata.cFileName,pstr, '.');
				
				char *pe = pstr;
				char pf[200];
				memset(pf,0,sizeof(pf));
				strncpy(pf,fdata.cFileName,pstr - fdata.cFileName + 1);

				
				for(int i=0; i<extn; i++){
		//			printf("\"%s\", \"%s\", \"%s\"\n",pf,pe,ext[i]);
					if(!strcmpc(ext[i],pf) || !strcmpc(ext[i],pe) ||
						!strcmpc(ext[i],fdata.cFileName)||
						!strcmpc(ext[i],".") ) {
						if(opt & OPT_NOPARENT) 
							strcpy(spath,fdata.cFileName);
						else 
							wsprintf(spath,"%s%s",fpath,fdata.cFileName);
						sl_addstr(plist,spath,0);
						break;
					}
			/*		else
					if(!strcmpc(ext[i],".")) {
						if(opt & OPT_NOPARENT) 
							strcpy(spath,fdata.cFileName);
						else 
							wsprintf(spath,"%s%s",fpath,fdata.cFileName);
						sl_addstr(plist,spath,0);
						break;
					}*/
					
				}
			}
		}
	}while(FindNextFile(hFind,&fdata));
	FindClose(hFind);

	return 0;
}



int fw_getpath(char *fpath,slist *plist) {

	char ext[200];
	char path[MAX_PATH];

	char sopt[50];
	DWORD opt=0;
	char *pstrb = NULL;
	char *pstre = NULL;
	char *sptr = NULL;

	char **pext = NULL;
	int extnum = 0;

	memset(sopt, 0,sizeof(sopt));
	memset(ext,0,sizeof(ext));
	memset(path,0,sizeof(ext));
	sl_init(plist);

	FCHRL(fpath,pstrb,'\\');
	if(*pstrb == '\\'){
		strcpy(ext,pstrb + 1);
		strncpy(path,fpath,pstrb - fpath + 1);
	}
	else {
		strcpy(ext,fpath);
		strcpy(path,".\\");
	}
	
	pstre = ext;
	pstrb = ext;

	FCHRL(ext, pstrb,'\\');
	FCHRR(pstrb,pstre,' ');
	if((*pstre)){
		*(pstre) = '\0';
		pstrb = pstre +1;
	}
	pstre = pstrb;

	while(true) {
		FCHRR(pstrb, pstre,'-');
		pstrb = pstre;
		if(!(*pstre))
			break;
		else{
			if(*(pstre + 1) == 'r') {
				opt|=OPT_REC;
			}else if(*(pstre + 1) == 'f'){
				opt|=OPT_FOLDERS;
			}else if(*(pstre + 1) == 'p'){
				opt|=OPT_NOPARENT;
			}else if(*(pstre + 1) == 'm'){
				opt|=OPT_MARKFOLDERS;
			}
			pstrb++;
		}
	}


	pstre = ext;
	pstrb = ext;
//	pext = (char**)malloc( sizeof(char**));

	pext = strsplitch(ext,'|',&extnum);
	int i=0;
	for(i=0; i<extnum; i++) {
		clear_chr(pext[i],' ');
		clear_chr(pext[i],'*');
		
	}

	fw_getpath_rec(path,pext,extnum,opt,plist);
	for( i=0; i<extnum;i++) {
		free(pext[ i]);
	}
	free(pext);

	return 0;
}
char ** strsplitch(char *string , char chr,int *cnt) {
	char **pext  = (char**)malloc( sizeof(char**));
	char * pstrb = string, *pstre = string;
	int len = 0;
	(*cnt) = 0;
	do {
		FCHRR(pstrb,pstre,chr);
		len = pstre - pstrb;
		*(pext + (*cnt)) = (char*)malloc(len + 2);
		memset(*(pext + (*cnt)),0,len+1);
		strncpy(*(pext + (*cnt))  ,pstrb,len);
		pstrb = pstre +1;
		pext = (char**)realloc(pext, _msize(pext) + sizeof(char*) );
		(*cnt) ++;
	}while((*pstre));
	return pext;
}

int fw_ren_files(char * fpath, char *ext2) {
	int time =0;
	slist list;
	snode *pnode = NULL;
	char *pstr = NULL;
	sl_init((&list));


	fw_getpath(fpath,&list);
	char newp[200];

	sl_for_each(&list,pnode) {
		strcpy(newp,pnode->string);
		FCHRR(newp,pstr,'.');
		*(pstr+1) = '\0';
		strcat(newp	, ext2);
//		printf("\t\"%s\" -> \"%s\"\n",pnode->string,newp);
		MoveFile(pnode->string, newp);
	}
	sl_destroy((&list));
	return time;
}