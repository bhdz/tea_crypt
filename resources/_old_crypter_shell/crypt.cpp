
/*
TEA Encryption engine implementation
*/
#include "stdafx.h"
#include "crypt.h"
#include "strlist.h"
#include "files.h"
/*	
ORIGINAL C CODE is... :
void code(long* v, long* k)  {              
	unsigned long y=v[0],z=v[1], sum=0,n=32 ;

	while (n-->0) {
		sum += DELTA ;
		y += (z<<4)+k[0] ^ z+sum ^ (z>>5)+k[1] ;
		z += (y<<4)+k[2] ^ y+sum ^ (y>>5)+k[3] ; 
	} 
	v[0]=y;
	v[1]=z;
}

void decode(long* v,long* k)  {
	unsigned long n=32, sum, y=v[0], z=v[1];
	sum=DELTA<<5;

	while (n-->0) {
		z-= (y<<4)+k[2] ^ y+sum ^ (y>>5)+k[3] ; 
		y-= (z<<4)+k[0] ^ z+sum ^ (z>>5)+k[1] ;
		sum-=DELTA ;
	}
	v[0]=y ; 
	v[1]=z ;  
}*/

/*
this one here is the tea callback fptr
tea engine will call it whenever there is 
something in the main thread to update
like : 
file encoding percentage

the callback params are
unsigned int cl_type <> type  of the callback
char *fname <> the file name the caller is working on
long param1 <> specific to the caller parameter 1
long param2 <> specific to the caller parameter 2
*/
//typedef int (*TEA_CALLBACK) (unsigned int,char *,long,long);
static TEA_CALLBACK pfcallback = NULL;

int tea_pfcall_stop(){
	if(!pfcallback)
		return 0;
	pfcallback(TEACALL_STOP,NULL,0,0);
	pfcallback = NULL;
	return 1;
}
int tea_pfcall_start(TEA_CALLBACK pf_call) {
	if(pfcallback) {
		tea_pfcall_stop();
	}
	pfcallback = pf_call;
	pfcallback(TEACALL_START,NULL,0,0);
	return 0;
}

DWORD Hashfunc2(const void *key,DWORD len)
{
	const BYTE *e, *k;
	DWORD h;
	k = (BYTE*)key;
	e = k + len;
	for (h = 0; k < e; ++k) {
		h *= 16777619;
		h ^= *k;
	}
	return h;
}

/*
Encoding/Decoding a single buffer :

*/
//#define TEABufCode tea_bcode


//int set

int tea_bcode(char key[17],char *buf,int buflen)  {
	char *pstr = NULL;
	int slen = strlen(buf);
	int div = slen / 8;
	
	int out_len = (div + ((slen % 8) ? 1:0) ) * 8 +1;
	int rem = out_len - slen;
	if(out_len  > buflen)
		return -1;
	memset(buf + slen,0,rem);
	for(pstr = buf; pstr < buf + slen; pstr+=8) { 
		CODE(pstr,key);
	}
	buf[out_len-1] = ((char)rem);
	return out_len;
}
int tea_bdecode(char key[17],char *buf,int buflen) {
	char *pstr = NULL;
	int slen = strlen(buf);
	int rem = (int)buf[slen - 1];
	if(rem < 0 || rem > 8) 
		return -1;
	int out_len = 0;
	for(pstr = buf; pstr < buf + slen; pstr+=8) {
		DECODE(pstr, key);
	}
	return slen - rem;
}

int tea_code(BYTE *val,DWORD vlen, char *key) {
	unsigned char *ptr;
	for(ptr = val; (ptr + 8) <= (val + vlen); ptr+=8) {
		CODE(ptr,key);
	}	
	return (int)((val + vlen) - (ptr) );
}

int tea_decode(BYTE *val,DWORD vlen, char *key) {
	unsigned char *ptr;
	for(ptr = val; (ptr+8) <= (val + vlen); ptr+=8) {
		DECODE(ptr,key);
	}	
	return (int)((val + vlen) - (ptr) );
}




/*
#define TFH_SIZE (sizeof(TEA_FILE_HDR))
#define CBLOCK_STD 8000

*/
int tea_fcode_safe(char *fpath,char *skey,int rbl) {
	char *buf = NULL;
//	char *safe_page = NULL;//this is the safe page
	int file = -1;
	int bt_read=0,bt_write=0,ret=0,cret=0;
	TEA_FILE_HDR hdr;
	unsigned long op_start=0,op_time=0;
	long last_pagesz=0,npages=0,cur_page=0;
	long seek_pos=0;
	char key[17];
	memset(key,0,sizeof(key));
	strncpy(key,skey,16);
	if(pfcallback){
		op_start = GetTickCount();
		pfcallback(TEACALL_CFILE|(TEACALLT_BEGIN<<16),
			fpath,0,0);
	}
	if(rbl % 8 || rbl < RBL_MIN || rbl > RBL_MAX) 
	{
		ret = -4;
		goto exit;
	}
	file = _open(fpath,_O_BINARY|_O_RDWR);
	if(file == -1) {
		ret = -2;
		goto exit;
	}
	if(tea_fgethdr(file,&hdr)) 	{
		
		ret = -5;
		goto exit;
	}
	bt_fsz = _filelength(file);
	lseek(file, seek_pos , SEEK_SET);
	buf = (char*)malloc(rbl);
	hdr.l8_cnt = 0;
	do{
	}while(cur_page < npages);

}
int tea_fcode(char *fpath,char *skey,int rbl) {
	unsigned char *buf = NULL;
	int file = -1;
	TEA_FILE_HDR hdr;
	int bt_read=0;
	int bt_write=0;
	int bt_uncoded = 0;
	int ret = 1;
	int cret = 1;
	DWORD id = 0;
	DWORD op_start=0,op_time=0;
	long bt_coded=0,bt_fsz=0,i=0;///tea_callback
	char key[17];
	memset(key,0,sizeof(key));
	strncpy(key,skey,16);
	if(pfcallback){
		op_start = GetTickCount();
		pfcallback(TEACALL_CFILE|(TEACALLT_BEGIN<<16),
			fpath,0,0);
	}

	if(rbl % 8 || rbl < RBL_MIN || rbl > RBL_MAX) {
		ret = -4;
		goto exit;
	}
	file = _open(fpath,_O_BINARY|_O_RDWR);
	if(file == -1) {
		ret = -2;
		goto exit;
	}
	if(tea_fgethdr(file,&hdr)) 
	{
		ret = -5;
		goto exit;
	}
	bt_fsz = _filelength(file);
	lseek(file, 0 , SEEK_SET);
	buf = (BYTE*)malloc(rbl);
	hdr.l8_cnt = 0;
	do{
		memset((void*)buf,0,rbl);
		bt_read = _read(file,(void *)buf,rbl);

		if(bt_read % 8 ) { 
			hdr.l8_cnt = (BYTE)((((bt_read/8) + 1 ) * 8) - bt_read);
		//	bt_read += hdr.l8_cnt;
			bt_write = bt_read + hdr.l8_cnt;
		}
		else 
			bt_write = bt_read;
		///encode
		bt_uncoded = tea_code((BYTE*)buf,bt_write,key);
		_lseek(file, -bt_read,SEEK_CUR);
		bt_write = _write(file,(void*)buf,bt_write);
		
		///Every 300 iterations send update via tea_callback
		if(pfcallback){
			bt_coded+=bt_write;
			if(!( i % 300)) {
				cret = pfcallback(TEACALL_CFILE | (TEACALLT_STAT <<16),
					fpath,bt_coded,bt_fsz);
				if(!cret)
					goto exit;
			}
			i++;
		}
	} while(bt_read);

	hdr.cblock_sz =rbl;
	hdr.key_hash = Hashfunc2((const void *)key,sizeof(key));
	tea_idgen(hdr.key_hash,hdr.idsign);

	_lseek(file,0,SEEK_END);
	_write(file,(void*)&hdr,TFH_SIZE);
	if(pfcallback){
		op_time = GetTickCount() - op_start;
		pfcallback(TEACALL_CFILE|(TEACALLT_COMPLETE<<16),
			fpath,op_time,0);
	}
exit:
	if(ret < 0 && pfcallback) {
		pfcallback(TEACALL_CFILE|(TEACALLT_ERROR<<16),
			fpath,ret,0);
	}
	if(buf)
		free(buf);
	if(file!=-1)
		_close(file);
	return ret;
}

int tea_fdecode(char *fpath,char *skey) {
	
	int rbl = 0;
	unsigned char *buf = NULL;
	int file=-1;
	TEA_FILE_HDR hdr;
	int bt_read=0;
	int bt_write=0;
	int bt_uncoded = 0;
	int l8_trunc;
	int ret = 1;
	int cret = 1;
	long bt_coded=0,bt_fsz=0,i=0;///tea_callback
	DWORD op_start=0,op_time=0;
	char key[17];
	memset(key,0,sizeof(key));
	strcpy(key,skey);

	if(pfcallback){
		pfcallback(TEACALL_DFILE|(TEACALLT_BEGIN<<16),
			fpath,0,0);
		op_start = GetTickCount();
	}
	file = _open(fpath,_O_BINARY|_O_RDWR);
	if(file == -1) {
		ret =  -3;//TEAERR_FOPEN
		goto exit;
	}
	if(!tea_fgethdr(file,&hdr)) {
		_close(file);
		ret =  -2;//TEAERR_BADHDR
		goto exit;
	}

	if(Hashfunc2((const void*)key,sizeof(key)) != hdr.key_hash ){
		ret =  -1; //TEAERR_BADPASS
		goto exit;
	}
	bt_fsz = _filelength(file);
	rbl = hdr.cblock_sz;
	l8_trunc = (int)hdr.l8_cnt;

	_chsize(file,_filelength(file) - TFH_SIZE);
	_lseek(file, 0,SEEK_SET);
	buf = (BYTE*)malloc(rbl);
	
	do{
		memset((void*)buf,0,rbl);
		bt_read = _read(file,(void *)buf,rbl);

		///decode
		bt_uncoded = tea_decode((BYTE*)buf,bt_read,key);
		///
		_lseek(file, -bt_read,SEEK_CUR);
		bt_write = _write(file,(void*)buf,bt_read);

		if(pfcallback){
			bt_coded+=bt_write;
			if(!( i % 300) ){
				cret = pfcallback(TEACALL_DFILE | (TEACALLT_STAT << 16),
					fpath,bt_coded,bt_fsz);
				if(!cret)
					goto exit;
			}
			i++;
		}
	} while(bt_read);
	_chsize(file,_filelength(file) - (long)hdr.l8_cnt);
	free(buf);
	_close(file);
	if(pfcallback){
		op_time = GetTickCount() - op_start;
		pfcallback(TEACALL_DFILE|(TEACALLT_COMPLETE<<16),
			fpath,op_time,0);
	}
exit:
	if(ret < 0 && pfcallback) {
		pfcallback(TEACALL_DFILE|(TEACALLT_ERROR<<16),
			fpath,ret,0);
	}
	if(buf)
		free(buf);
	if(file !=-1 )
		_close(file);
	return ret;
}

/*
returns 0 if header is invalid
*/

int tea_fgethdr(int fh,TEA_FILE_HDR *phdr) {
	DWORD id = 0;

	memset((void*)phdr->idsign,0,sizeof(phdr->idsign));
	_lseek(fh, - ((long)TFH_SIZE),SEEK_END);
	_read(fh,(void*)phdr,TFH_SIZE);
	
	if(!tea_idverify(phdr->key_hash,phdr->idsign))
		return 0;
	return 1;
}

int tea_pcode(char *path,char skey[17],int readbl) {
	DWORD op_start=0,op_time=0;
	int ret = 1;
	slist list;
	char key[17];
	memset(key,0,sizeof(key));
	strcpy(key,skey);
	snode *pnode = NULL;
	sl_init((&list));
	if(pfcallback) {
		pfcallback(TEACALL_CPATH|(TEACALLT_BEGIN<<16),
			path,0,0);
		op_start = GetTickCount();
	}
	if(fw_getpath(path,&list) < 0){
		ret = -10;
		goto exit;
	}

	sl_for_each((&list),pnode) {
		tea_fcode(pnode->string,key,readbl);
	}
	if(pfcallback) {
		op_time = GetTickCount() - op_start;
		pfcallback(TEACALL_CPATH|(TEACALLT_COMPLETE<<16),
			path,op_time,0);
	}
//	printf("Total files %d encoded (%f secs) \n",list.cnt,((float)big_end - (float)big_start) / 1000.0);
exit:
	if(ret < 0 && pfcallback) {
		pfcallback(TEACALL_CPATH|(TEACALLT_ERROR<<16),
			path,ret,0);
	}
	sl_destroy((&list));
	return ret;
}

int tea_pdecode(char *path,char skey[17]) {
	DWORD op_start=0,op_time=0;
	int ret = 1;
	slist list;
	char key[17];
	memset(key,0,sizeof(key));
	strcpy(key,skey);
	snode *pnode = NULL;
	sl_init((&list));
	if(pfcallback) {
		pfcallback(TEACALL_DPATH|(TEACALLT_BEGIN<<16),
			path,0,0);
		op_start = GetTickCount();
	}
	if(fw_getpath(path,&list) < 0){
		ret = -10;
		goto exit;
	}

	sl_for_each((&list),pnode) {
		tea_fdecode(pnode->string,key);
	}
	if(pfcallback) {
		op_time = GetTickCount() - op_start;
		pfcallback(TEACALL_DPATH|(TEACALLT_COMPLETE<<16),
			path,op_time,0);
	}
//	printf("Total files %d encoded (%f secs) \n",list.cnt,((float)big_end - (float)big_start) / 1000.0);
exit:
	if(ret < 0 && pfcallback) {
		pfcallback(TEACALL_DPATH|(TEACALLT_ERROR<<16),
			path,ret,0);
	}
	sl_destroy((&list));
	return ret;
}


/*
Searching up a path for Encoded files
puts the filepaths into a list 
returns the count of the files
*/

int tea_psearch(char *path,slist *out_files) {
	int iret=0, fh;
	int i=0; //debug
	slist list_files;
	snode *pnode = NULL,*paddnode = NULL;
	TEA_FILE_HDR hdr;
	//Initing string lists
	sl_init((&list_files));
	sl_init((out_files));
	fw_getpath(path,&list_files);
	sl_for_each((&list_files),pnode) {

		fh = _open(pnode->string,_O_BINARY|_O_RDONLY);

		if(tea_fgethdr(fh,&hdr)) {
		//	slCreateStrAddL(pnode->string
			paddnode = pnode;
			pnode = pnode->list_node.pnext;
			
			sl_remnode((&list_files),paddnode);
			sl_addnode(out_files,paddnode,0);
			
		}
		_close(fh);
		i++;
		if(!pnode) 
			break;
		
	}
	sl_destroy((&list_files));
	return iret;
}
//#define TEAERR_CODED -5 //error allready coded
//#define TEAERR_BADRBL -4 //bad readblock
//#define TEAERR_FOPEN -3 //cannot open file for read/write
//#define TEAERR_BADHDR -2 //bad tea header
//#define TEAERR_BADPASS -1 //bad password
char * tea_geterrstr(int err) {
	if(err>=0)
		return "bad error code";
	if(err == TEAERR_BADPASS) {
		return "bad password";
	}else if(err == TEAERR_BADHDR) {
		return "bad tea header";
	}else if(err == TEAERR_FOPEN) {
		return "cannot open file for read/write";
	}else if(err == TEAERR_BADRBL) {
		return "bad readblock";
	}else if(err == TEAERR_CODED) {
		return "allready coded";
	}
	return "unknown error";
}


void tea_idgen(DWORD seed,char *idstr) {
	char *pstr = idstr;
	srand(seed);
	while(pstr < idstr + ID_SIZE-1) {
		(*pstr) = ID_LOWCHAR + (rand() % ( ID_HIGHCHAR - ID_LOWCHAR));
		pstr++;
	}
	*pstr = '\0';
}

int tea_idverify(DWORD seed,char *idstr ) {
	char *pstr = idstr;
	srand(seed);
	while(pstr < idstr + ID_SIZE-1) {
		if( (*pstr) != ID_LOWCHAR + (rand() % ( ID_HIGHCHAR - ID_LOWCHAR)) )
			return 0;
		pstr++;
	}
	return 1;
}
