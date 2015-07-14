


#include "stdafx.h"
#include "strlist.h"
#include "prs.h"



char * FMCHRRE(char *str,char *fs,char esc)  {
	char *pstr = str;
	char *pfs = fs;
	int len_fs = strlen(fs);
	while((*pstr)  ) {
		if(*(pstr -1)!=esc){
			while((*pfs)) {
				if(*pfs == *pstr){
					return pstr;
				}
				pfs++;
			}
			pfs = fs;
		}
		pstr++;
	}


	return NULL;
}


#define FCHRRSK(str,pstr,ch,sk) do {\
	pstr = (char*)str;\
	while((*pstr)  ) {\
		if(*(pstr -1)!=sk)\
			if((*pstr) ==ch )\
				break;\
		pstr++;\
	}\
}while(0)




char * prs_chop_str(char *str, char chr,int *out_len) {
	char *pstrb=str;
	char *pstre=pstrb;
	char *pstre2=NULL;
	FCHRRSK(str,pstrb,chr,'\\');
	
	FCHRRSK(pstrb+1,pstre,chr,'\\');
	if(*pstrb == chr && *pstre == chr && pstre > pstrb) {
		(*out_len) = pstre - pstrb-1;
		return pstrb+1;
	}
	return NULL;
}

char * prs_chop_str(char *str, char dlm[3],int *out_len) {
	char *pstrb=str;
	char *pstre=pstrb;
	char *pstre2=NULL;
	
	int skip = 1;
	FCHRRSK(str,pstrb,dlm[0],'\\');
	pstre = pstrb;
	if(*pstrb == dlm[0]){
		while(true){
			pstre++;
			pstre2 = pstre;
			pstre = FMCHRRE(pstre2,dlm,'\\');
			if(!pstre){
				return NULL;
			}else if(*pstre == dlm[0]){
				skip++;
			}else if(*pstre == dlm[1]){
				skip--;
				if(!skip) {
					(*out_len) = pstre - pstrb - 1;
					return pstrb + 1;
				}
			}else if(*pstre == '\0'){
				return NULL;
			}
			//printf("%c,",*pstre);
		}
	}
	return NULL;
}

#define FCHRRSK(str,pstr,ch,sk) do {\
	pstr = (char*)str;\
	while((*pstr)  ) {\
		if(*(pstr -1)!=sk)\
			if((*pstr) ==ch )\
				break;\
		pstr++;\
	}\
}while(0)

int prs_parse_cmdline(char *cmdl,char *cmd,char **prms){
	int ret = 0;
	int len = 0;
	char *pstrb,*pstre;
	FCHRR(cmdl,pstrb,' ');
	memset(cmd,0,100);
	strncpy(cmd,cmdl,pstrb - cmdl);
	pstre = pstrb;
	while(true) {
		pstre = prs_chop_str(pstrb,'\"',&len);
		if(!pstre)
			break;
		memset(prms[ret],0,100);
		strncpy(prms[ret],pstre,len);
		pstrb = pstre + len+1;
		ret++;
	}
	return ret;
}

int strcmpc(const char *str1,const char *str2) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if(len1 > len2)
		return -1;
	else if(len1 < len2)
		return 1;
	else {
		for(int i=0; i< len1; i++) {
			if(str1[i] > str2[i] && str1[i] != str2[i] + 32)
				return -1;
			else if(str2[i] > str1[i]  && str2[i] != str1[i] + 32 )
				return 1;
		}
	}
	return 0;

}

void clear_chr(char* str,char chr) {
	char  *pstr = NULL;
	char  *pstrn = NULL;
	do {
		FCHRR(str,pstr,chr);
		if((*pstr) ) {
			(*pstr) = '\0';
			pstrn = pstr +1;
			strcat(pstr ,pstrn);
		}
	}while((*pstr));
}
