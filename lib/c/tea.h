#ifndef TEA_LIB_H
#define TEA_LIB_H

typedef struct _Key     Key_t;
typedef struct _Blocks  Blocks_t;   
typedef struct _String  String_t;

struct Tea_s {
	int rounds;
	Key *key;
	Blocks *blocks;
	char *padding;
};


typedef struct _Memory Memory_t;
typedef Memory_s {
};

typedef void* (*memory_alloc_t)(Memory_t *self, size_t size);

#endif
