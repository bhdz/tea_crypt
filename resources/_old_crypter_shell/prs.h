
#ifndef PPRS
#define PPRS

#define FCHRL(str,pstr,ch) do {\
	pstr = str + strlen(str);\
	while(pstr != str && (*pstr )!=ch) {\
		pstr--;\
	}\
	if(*pstr!=ch) pstr = str;\
}while(0)
#define FCHRR(str,pstr,ch) do {\
	pstr = (char*)str;\
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

char * prs_chop_str(char *str, char chr,int *out_len) ;
char * prs_chop_str(char *str, char dlm[3],int *out_len) ;
int prs_parse_cmdline(char *cmdl,char *cmd,char **prms);

int strcmpc(const char *str1,const char *str2) ;
void clear_chr(char* str,char chr);

#endif