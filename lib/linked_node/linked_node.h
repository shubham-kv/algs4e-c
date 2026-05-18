#include <stdbool.h>

#ifndef __linked_node_h
#define __linked_node_h

struct LinkedNode;
struct LinkedNodeIterator;

typedef struct LinkedNode *LinkedNode;
typedef struct LinkedNodeIterator *LinkedNodeIter;

struct LinkedNode {
  struct LinkedNode *next;
};

struct LinkedNodeIterator {
  struct LinkedNode *cur;
};

#define NODE LinkedNode
#define ITER LinkedNodeIter

ITER LinkedNodeIter_Create(NODE node);
 int LinkedNodeIter_Init(ITER iterator, NODE node);
 int LinkedNodeIter_Clear(ITER iterator);
 int LinkedNodeIter_Delete(ITER *iterator);
bool LinkedNodeIter_HasNext(ITER iterator);
 int LinkedNodeIter_GetNext(ITER iterator, NODE *out);

#undef ITER

#endif // __linked_node_h
