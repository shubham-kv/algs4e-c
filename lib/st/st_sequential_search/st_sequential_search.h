#include <stdbool.h>
#include "comparators.h"

#ifndef __sequential_search_st_h
#define __sequential_search_st_h

struct SequentialSearchST;
struct SequentialSearchSTKeysIterator;

typedef const void *SSSTKey;
typedef const void *SSSTVal;
typedef struct SequentialSearchST *SSST;
typedef struct SequentialSearchSTKeysIterator *SSSTKeysIter;

#define ST SSST
#define KEY SSSTKey
#define VAL SSSTVal
#define ITER SSSTKeysIter

  ST SSST_Create(ComparatorFn keyComparator);
 int SSST_Delete(ST *st);
 int SSST_Put(ST st, KEY key, VAL val);
 int SSST_Get(ST st, KEY key, VAL *out);
 int SSST_DeleteKey(ST st, KEY key);
 int SSST_Contains(ST st, KEY key, bool *out);
 int SSST_Size(ST st);
bool SSST_IsEmpty(ST st);

ITER SSSTKeysIter_Create(ST st);
 int SSSTKeysIter_Delete(ITER *iterator);
bool SSSTKeysIter_HasNext(ITER iterator);
 int SSSTKeysIter_GetNext(ITER iterator, KEY *out);

#undef ST
#undef KEY
#undef VAL
#undef ITER

#endif // __sequential_search_st_h
