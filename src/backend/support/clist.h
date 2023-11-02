#ifndef CLIST_H
#define CLIST_H

typedef struct CList
{
  void * (* add)         (struct CList *l, void *o);            /* Add object to the end of a list */
  void * (* insert)      (struct CList *l, void *o, int n);     /* Insert object at position 'n' */
  void * (* replace)     (struct CList *l, void *o, int n);     /* Replace object at position 'n' */
  void   (* remove)      (struct CList *l, int n);              /* Remove object at position 'n' */
  void * (* at)          (struct CList *l, int n);              /* Get object at position 'n' */
  int    (* realloc)     (struct CList *l, int n);              /* Reallocate list to 'size' items */
  int    (* count)       (struct CList *l);                     /* Get list size in items */
  void * (* firstMatch)  (struct CList *l, const void *o, size_t shift, size_t size, int string);
                                                                /* Returns object with first match of string or byte compare */
  void * (* lastMatch)   (struct CList *l, const void *o, size_t shift, size_t size, int string);
                                                                /* Returns object with last match of string or byte compare */
  int    (* index)       (struct CList *l);                     /* Get index of previous search match */
  int    (* swap)        (struct CList *l, int a, int b);       /* Swap, replace two items with index a b */
  int    (* allocSize)   (struct CList *l);                     /* Get allocated size in items */
  size_t (* itemSize)    (struct CList *l);                     /* Get item size in bytes */
  void   (* print)       (struct CList *l, size_t shift, int n, const char *type);   /* Print list data */
  void   (* clear)       (struct CList *l);                     /* Clear list */
  void   (* free)        (struct CList *l);                     /* Destroy struct CList and all data */
  void  *priv;           /* NOT FOR USE, private data */
} CList;

CList *CList_init(size_t objSize); /* Set list object size in bytes */

#endif /* CLIST_H */