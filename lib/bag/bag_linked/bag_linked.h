#include <stdbool.h>

#ifndef __linked_bag_h
#define __linked_bag_h

struct LinkedBagNode;
struct LinkedBag;
struct LinkedBagIterator;

typedef const void *BagItem;
typedef struct LinkedBagNode *BagNode;
typedef struct LinkedBag *Bag;
typedef struct LinkedBagIterator *BagIterator;

#define BAG Bag
#define ITEM BagItem
#define ITER BagIterator

struct LinkedBag {
  struct LinkedBagNode *first;
  int size;
};

 BAG Bag_Create();
 int Bag_Init(BAG bag);
 int Bag_Clear(BAG bag);
 int Bag_Delete(BAG *bag);
 int Bag_Add(BAG bag, ITEM item);
 int Bag_Size(BAG bag);
bool Bag_IsEmpty(BAG bag);

struct LinkedBagIterator {
  struct LinkedBagNode *cur;
};

ITER BagIterator_Create(BAG bag);
 int BagIterator_Init(ITER iterator, BAG bag);
 int BagIterator_Clear(ITER iterator);
 int BagIterator_Delete(ITER *iterator);
bool BagIterator_HasNext(ITER iterator);
 int BagIterator_GetNext(ITER iterator, ITEM *out);

#undef BAG
#undef ITEM
#undef ITER

#endif // __linked_bag_h
