

/*
TEA encrypt engine declarations and macroses

*/

#ifndef _TEA_CRYPT_ENGINE_
#define _TEA_CRYPT_ENGINE_
#include "strlist.h"
#define VER  (DWORD) (01010103)

#define VER_STR "TEA v1.1.1.3"
#define DELTA 0x9e3779b9

#pragma pack(1) //for MSVC++ 6.0 structure packing (prevents random padding)
/*
Main File Header
ver 3:
(Safe writing
*/

struct TEA_FILE_HDR {
	unsigned long key_hash; //hash of the key
	unsigned long cblock_sz; //cypher block size ( standart is 4096 bytes)
	unsigned char flags;//flags...
	unsigned char l8_cnt; //(8 byte alligment)
	char idsign[20]; //unique id string 
};
/*
Safe page header
*/
struct TEA_SWH {
	long pgno_cur;//current page number (this is for safe operations) -1 for none
	long pgno_last;//last page number
	
} ;

#pragma pack()

//flags{
#define TFL_SAFEWRITE 0x1
//}
/*Coding macros ...encode 2 byte variable with 4 byte key*/
#define CODE(lpv,lpk) do { \
	long y = ((long*)lpv)[0], z = ((long*)lpv)[1],sum = 0,n=32 ; \
	while(n-->0) {\
		sum+=DELTA;\
		y += (z<<4)+((long*)lpk)[0] ^ z+sum ^ (z>>5)+((long*)lpk)[1];\
		z += (y<<4)+((long*)lpk)[2] ^ y+sum ^ (y>>5)+((long*)lpk)[3]; \
	}\
	((long*)lpv)[0]=y;\
	((long*)lpv)[1]=z;\
}while(0);
/*
Decoding...
*/
#define DECODE(lpv,lpk) do {\
	long y = ((long*)lpv)[0], z = ((long*)lpv)[1],sum,n=32 ;\
	sum=DELTA<<5;\
	while (n-->0) {\
		z-= (y<<4)+((long*)lpk)[2] ^ y+sum ^ (y>>5)+((long*)lpk)[3]; \
		y-= (z<<4)+((long*)lpk)[0] ^ z+sum ^ (z>>5)+((long*)lpk)[1];\
		sum-=DELTA ;\
	}\
	((long*)lpv)[0]=y;\
	((long*)lpv)[1]=z;\
}while(0);


#define TFH_SIZE (sizeof(TEA_FILE_HDR))
#define ID_SIZE 20
#define ID_LOWCHAR ' '
#define ID_HIGHCHAR '}'
#define CBLOCK_STD 4096 //standart readblock size 
/*
(note: if you are using other value make sure it is % 8 (alligned)
otherwise tea_fcode will return error )
*/
#define DELTA 0x9e3779b9
#define RBL_MIN 8
#define RBL_MAX 80000

//
//#define HDRSIGN (DWORD)(*((DWORD*)"AABB" ))

//error codes{
#define TEAERR_CODED -5 //error allready coded
#define TEAERR_BADRBL -4 //bad readblock
#define TEAERR_FOPEN -3 //cannot open file for read/write
#define TEAERR_BADHDR -2 //bad tea header
#define TEAERR_BADPASS -1 //bad password
//}

//callback support{
#define TEACALL_START 0x0
#define TEACALL_STOP 0x1
#define TEACALL_CFILE 0x2
#define TEACALL_DFILE 0x3
#define TEACALL_CPATH 0x4
#define TEACALL_DPATH 0x5
//theese below are shifted << 16 in the message var
#define TEACALLT_BEGIN (0x1) //begin operation
#define TEACALLT_STAT (0x2) //status operation
#define TEACALLT_COMPLETE (0x3) //complete message (with timing usually)
#define TEACALLT_ERROR (0x4) //some error have occured param1 holds the errcode

typedef int (*TEA_CALLBACK) (unsigned int msg,char *fname,long param1,long param2);

int tea_pfcall_start(TEA_CALLBACK pf_call) ; //sets the callback pointer 
int tea_pfcall_stop(); //set the callback pointer to NULL
//}


DWORD Hashfunc2(const void *key,DWORD len);

//int tea_code(BYTE *val,DWORD vlen, BYTE *key, int klen,DWORD dwOpt);
//int tea_decode(BYTE *val,DWORD vlen, BYTE *key, int klen,DWORD dwOpt);
int tea_fcode(char *fpath,char *skey,int rbl) ;
int tea_fdecode(char *fpath,char *skey) ;
int tea_pdecode(char *path,char *key);
int tea_pcode(char *path,char *key,int readbl);
int tea_psearch(char *path,slist *out_files);
int tea_fgethdr(int fh,TEA_FILE_HDR *phdr);
char * tea_geterrstr(int err);
int tea_idverify(DWORD seed,char *idstr ) ;
void tea_idgen(DWORD seed,char *idstr);

//un-comment theese if you are using the old function names
/*
#define TEAPathSrch tea_psearch
#define TEADecodePath tea_pdecode
#define TEAEncodePath tea_pcode
#define TEAFileGetHdr tea_fgethdr
#define TEAFileDecode tea_fdecode
#define TEAFileEncode tea_fcode
#define TEAEncrypt tea_code
#define TEADecrypt tea_decode
*/

#endif