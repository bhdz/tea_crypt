#ifndef _LIST_INC
#define _LIST_INC


#if defined(__cplusplus)
extern "C" {
#endif

#define	LST_HEAD(name, type)					\
struct name {							\
	struct type *pfirst;		/* first element */	\
	struct type *plast;		/* last element */	\
}

#define	LST_NODE(type)						\
struct {							\
	struct type *pnext;		/* next element */	\
	struct type *pprev;		/* previous element */	\
}

#define	LST_FIRST(head)		((head)->pfirst)
#define	LST_LAST(head)		((head)->plast)
#define	LST_NEXT(elm, field)	((elm)->field.pnext)
#define	LST_PREV(elm, field)	((elm)->field.pprev)



#define	LST_INIT(head) do {				\
	(head)->pfirst = NULL;				\
	(head)->plast = NULL;				\
} while (0)



#define	LST_INS_AFTER(head, listelm, elm, field) do {		\
	(elm)->field.pnext = (listelm)->field.pnext;		\
	(elm)->field.pprev = (listelm);				\
	if ((listelm)->field.pnext == NULL)		\
		(head)->plast = (elm);				\
	else							\
		(listelm)->field.pnext->field.pprev = (elm);	\
	(listelm)->field.pnext = (elm);				\
} while (0)

#define	LST_INS_BEFORE(head, listelm, elm, field) do {		\
	(elm)->field.pnext = (listelm);				\
	(elm)->field.pprev = (listelm)->field.pprev;		\
	if ((listelm)->field.pprev == NULL)		\
		(head)->pfirst = (elm);				\
	else							\
		(listelm)->field.pprev->field.pnext = (elm);	\
	(listelm)->field.pprev = (elm);				\
} while (0)

#define	LST_INS_HEAD(head, elm, field) do {			\
	(elm)->field.pnext = (head)->pfirst;			\
	(elm)->field.pprev = NULL;			\
	if ((head)->plast == NULL)			\
		(head)->plast = (elm);				\
	else							\
		(head)->pfirst->field.pprev = (elm);		\
	(head)->pfirst = (elm);					\
} while (0)

#define	LST_INS_TAIL(head, elm, field) do {		\
	(elm)->field.pnext = NULL;			\
	(elm)->field.pprev = (head)->plast;			\
	if ((head)->pfirst == NULL)			\
		(head)->pfirst = (elm);				\
	else							\
		(head)->plast->field.pnext = (elm);		\
	(head)->plast = (elm);					\
} while (0)

#define	LST_REMOVE(head, elm, field) do {		\
	if ((elm)->field.pnext == NULL)		\
		(head)->plast = (elm)->field.pprev;		\
	else							\
		(elm)->field.pnext->field.pprev =		\
		    (elm)->field.pprev;				\
	if ((elm)->field.pprev == NULL)		\
		(head)->pfirst = (elm)->field.pnext;		\
	else							\
		(elm)->field.pprev->field.pnext =		\
		    (elm)->field.pnext;				\
} while (0)

#if defined(__cplusplus)
}
#endif

#endif