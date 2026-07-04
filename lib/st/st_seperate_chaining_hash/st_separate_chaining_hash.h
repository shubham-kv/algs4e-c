#include <stdbool.h>
#include <stdint.h>

#include "comparators.h"
#include "hashers.h"

#ifndef __separate_chaining_hash_st
#define __separate_chaining_hash_st

struct SeparateChainingHashST;
struct SeparateChainingHashSTKeysIterator;

typedef const void *SCHSTKey;
typedef const void *SCHSTVal;
typedef struct SeparateChainingHashST *SCHST;
typedef struct SeparateChainingHashSTKeysIterator *SCHSTKeysIter;

#define ST SCHST
#define KEY SCHSTKey
#define VAL SCHSTVal
#define ITER SCHSTKeysIter

  ST SCHST_Create(ComparatorFn keyComparator, HashFn keyHasher, uint32_t tableSize);
 int SCHST_Delete(ST *st);
 int SCHST_Put(ST st, KEY key, VAL val);
 int SCHST_Get(ST st, KEY key, VAL *out);
 int SCHST_DeleteKey(ST st, KEY key);
 int SCHST_Contains(ST st, KEY key, bool *out);
 int SCHST_Size(ST st);
bool SCHST_IsEmpty(ST st);

ITER SCHSTKeysIter_Create(ST st);
 int SCHSTKeysIter_Delete(ITER *iterator);
bool SCHSTKeysIter_HasNext(ITER iterator);
 int SCHSTKeysIter_GetNext(ITER iterator, KEY *out);

#undef ST
#undef KEY
#undef VAL
#undef ITER

#endif // __separate_chaining_hash_st
