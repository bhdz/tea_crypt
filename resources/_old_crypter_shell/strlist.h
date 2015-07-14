

/*
String list declarations...
also some other functions and macroses
*/


#ifndef _STR_LIST_INC_
#define _STR_LIST_INC_
#include "_list.h"
typedef struct _snode {
	struct s_n {
		_snode *pprev,*pnext;
	}list_node;
	char *string;
}snode;

typedef struct _slist {
	struct s_h {
		_snode *pfirst,*plast;
	}list_head;

	int cnt;
} slist;




#define sl_init(plst) do{\
	LST_INIT((&(((slist*)plst)->list_head))) ;\
	plst->cnt= 0;\
}while(0)

#define sl_addnode(plist,psn,pos) do{\
	if(!pos)\
	LST_INS_TAIL((&plist->list_head),psn,list_node);\
	else\
		LST_INS_HEAD((&plist->list_head),psn,list_node);\
	plist->cnt++;\
}while(0)

#define sl_remnode(plist,psn) do{\
	LST_REMOVE(&plist->list_head,psn,list_node);\
	return plist->cnt--;\
}while(0)

#define sl_remstr(plist,psn) do{\
	LST_REMOVE(&plist->list_head,psn,list_node);\
	LocalFree(psn->string);\
	LocalFree(psn);\
	psn = NULL;\
	plist->cnt--;\
}while(0)

#define sl_destroy(plist) do{\
	snode *pnode = plist->list_head.pfirst;\
	if(pnode){\
		do{\
			sl_remstr(plist,pnode);\
			pnode = plist->list_head.pfirst;\
		}while(pnode);\
		\
		sl_init(plist);\
	}\
}while(0)

snode *sl_create_node() ;

snode * sl_addstr(slist *plist,char *str,int pos) ;
DWORD sl_calcmem( slist *plst) ;
void sl_sort(slist *plist);
int sl_extract(char *in,char *out,char chr) ;
void clear_chr(char* str,char chr);
#define sl_is_empty(plist) ((plist->list_head.pfirst == NULL && plist->list_head.plast == NULL)?1:0)
#define sl_for_each(plist,pnode) for(pnode = ((slist*)plist)->list_head.pfirst; pnode ; pnode = pnode->list_node.pnext)




//#define sgetat(iat,list) ((STR_NODE*)(slGetAt(list,iat)))->string)
#endif