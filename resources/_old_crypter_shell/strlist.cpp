

#include "stdafx.h"
#include "strlist.h"
#include "prs.h"
#include "_list.h"




snode * sl_addstr(slist *plist,char *str,int pos) {	
	snode * psn = sl_create_node();

	psn->string = (char *)LocalAlloc(LPTR, strlen(str) +1);
	strcpy(psn->string,str);
	sl_addnode(plist,psn,pos);
	return psn;
}

int sl_extract(char *in,char *out,char chr) {
	char *pinb = in;
	char *pine = in;
	FCHRR(pinb,pine,chr);
	if(*pine == chr) {
		(*pine) = '\0';
		pine++;
		pinb = pine;
		FCHRR(pinb,pine,chr);
		if(*pine == chr){
			*pine = '\0';
			strcpy(out,pinb);
			return 1;
		}
	}
	return 0;
}

snode *sl_create_node() {
	snode *psn = (snode*)LocalAlloc(LPTR, sizeof(snode)) ;
	psn->string = NULL;
	psn->list_node.pnext = psn->list_node.pprev = NULL;
	return psn;
}


DWORD sl_calcmem( slist *plst) {
	DWORD mem = plst->cnt * sizeof(snode) + sizeof(slist);
	snode *pnode = NULL;
	sl_for_each(plst, pnode) {
		mem += strlen(pnode->string) +1;
	}
	return mem;
}
void sl_sort(slist *plist) {
	char *tmp = NULL;
	snode *pnode1=NULL, *pnode2 =NULL;
	sl_for_each(plist, pnode1) {
		sl_for_each(plist, pnode2) {
			if(pnode1 !=pnode2) {
				if(strcmp(pnode2->string, pnode1->string) > 0) {
					tmp = pnode1->string;
					pnode1->string = pnode2->string;
					pnode2->string = tmp;
				}
			}
		}
	}
}

