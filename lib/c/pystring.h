#ifndef PYSTRING_LIB_H
#define PYSTRING_LIB_H

#include <stddef.h>

typedef struct String_s String;

struct String_s {
    size_t          length;
    unsigned short  flags;
    void*           buffer;
};

#define cstr(ps) ((char*)(ps).buffer)
#define cstrc(ps) ((const char*)(ps).buffer)

typedef String* String__new__(void *type_info, void *init);
typedef String* String__remove__(String *instance);
typedef String* String__init__(String *instance, void *init, ...);
typedef String* String_clone(String *inst);
typedef String* String_cat(String *inst, String *another);

typedef Slice_s Slice;
typedef int Index;
typedef Index Cursor;

struct Slice_s {
    Index       from;
    Index       to;
    String*     sliced;
};

typedef Slice String_slice(String *instance, Slice slice);

typedef Slice String_split(String *instance, Slice slice);
typedef Slice String_join(String *instance, Slice slice);
typedef Slice String_format(String *instance, Slice slice);

#endif

