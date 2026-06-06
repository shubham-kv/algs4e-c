#include <stdbool.h>

#include "comparators.h"

#ifndef __binary_search_tree_h
#define __binary_search_tree_h

struct BinarySearchTree;
struct BinarySearchTreeKeysIterator;

typedef const void *BSTKey;
typedef const void *BSTVal;
typedef struct BinarySearchTree *BST;
typedef struct BinarySearchTreeKeysIterator *BSTKeysIter;

#define ST BST
#define KEY BSTKey
#define VAL BSTVal
#define ITER BSTKeysIter

  ST BST_Create(ComparatorFn keyComparator);
 int BST_Delete(ST *st);
 int BST_Put(ST st, KEY key, VAL val);
 int BST_Get(ST st, KEY key, VAL *out);
 int BST_DeleteKey(ST st, KEY key);
 int BST_Contains(ST st, KEY key, bool *out);
 int BST_Size(ST st);
bool BST_IsEmpty(ST st);

 int BST_Min(ST st, KEY *out);
 int BST_Max(ST st, KEY *out);
 int BST_Floor(ST st, KEY key, KEY *out);
 int BST_Ceiling(ST st, KEY key, KEY *out);
 int BST_Rank(ST st, KEY key, int *out);
 int BST_Select(ST st, int rank, KEY *out);
 int BST_DeleteMin(ST st);
 int BST_DeleteMax(ST st);
 int BST_SizeOfRange(ST st, KEY low, KEY high, int *out);

ITER BSTKeysIter_Create(ST st);
ITER BSTKeysIter_CreateInRange(ST st, KEY low, KEY high);
 int BSTKeysIter_Delete(ITER *iterator);
bool BSTKeysIter_HasNext(ITER iterator);
 int BSTKeysIter_GetNext(ITER iterator, KEY *out);

#undef ST
#undef KEY
#undef VAL
#undef ITER

#endif // __binary_search_tree_h
