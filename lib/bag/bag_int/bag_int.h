#include <stdbool.h>
#include <stdint.h>

#ifndef __int_bag_h
#define __int_bag_h

struct IntegerBagNode;
struct IntegerBag;
struct IntegerBagIterator;

typedef int64_t IntBagItem;
typedef struct IntegerBagNode *IntBagNode;
typedef struct IntegerBag *IntBag;
typedef struct IntegerBagIterator *IntBagIterator;

#define BAG IntBag
#define ITEM IntBagItem
#define ITER IntBagIterator

struct IntegerBag {
  struct IntegerBagNode *first;
  int size;
};

 BAG IntBag_Create();
 int IntBag_Init(BAG bag);
 int IntBag_Clear(BAG bag);
 int IntBag_Delete(BAG *bag);
 int IntBag_Add(BAG bag, ITEM item);
 int IntBag_Size(BAG bag);
bool IntBag_IsEmpty(BAG bag);

struct IntegerBagIterator {
  struct IntegerBagNode *cur;
};

ITER IntBagIter_Create(BAG bag);
 int IntBagIter_Init(ITER iterator, BAG bag);
 int IntBagIter_Clear(ITER iterator);
 int IntBagIter_Delete(ITER *iterator);
bool IntBagIter_HasNext(ITER iterator);
 int IntBagIter_GetNext(ITER iterator, ITEM *out);

#undef BAG
#undef ITEM
#undef ITER

#endif // __int_bag_h
