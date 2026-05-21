#include <stdbool.h>

#include "comparators.h"

#ifndef __binary_search_st_h
#define __binary_search_st_h

struct BinarySearchST;
struct BinarySearchSTKeysIterator;

typedef const void *BSSTKey;
typedef const void *BSSTVal;
typedef struct BinarySearchST *BSST;
typedef struct BinarySearchSTKeysIterator *BSSTKeysIter;

#define ST BSST
#define KEY BSSTKey
#define VAL BSSTVal
#define ITER BSSTKeysIter

  ST BSST_Create(ComparatorFn keyComparator);
 int BSST_Delete(ST *st);
 int BSST_Put(ST st, KEY key, VAL val);
 int BSST_Get(ST st, KEY key, VAL *out);
 int BSST_DeleteKey(ST st, KEY key);
bool BSST_Contains(ST st, KEY key);
 int BSST_Size(ST st);
bool BSST_IsEmpty(ST st);

 int BSST_Min(ST st, KEY *out);
 int BSST_Max(ST st, KEY *out);
 int BSST_Floor(ST st, KEY key, KEY *out);
 int BSST_Ceiling(ST st, KEY key, KEY *out);
 int BSST_Rank(ST st, KEY key, int *out);
 int BSST_Select(ST st, int rank, KEY *out);
 int BSST_DeleteMin(ST st);
 int BSST_DeleteMax(ST st);
 int BSST_SizeOfRange(ST st, KEY low, KEY high);

ITER BSSTKeysIter_Create(ST st);
ITER BSSTKeysIter_CreateInRange(ST st, KEY low, KEY high);
 int BSSTKeysIter_Delete(ITER *iterator);
bool BSSTKeysIter_HasNext(ITER iterator);
 int BSSTKeysIter_GetNext(ITER iterator, KEY *out);

#undef ST
#undef KEY
#undef VAL
#undef ITER

#endif // __binary_search_st_h
