

#include "strlist.h"
#ifndef FILE_H_INC_
#define FILE_H_INC_



#define GETEXT(pstr,str) do { pstr = str + strlen(str); while((*pstr) != '.' && pstr!=str) pstr--;  }while(0);

int fw_getpath(char *fpath,slist *plist) ;
char ** strsplitch(char *string , char chr,int *cnt);
int fw_ren_files(char * fpath, char *ext2);
#endif