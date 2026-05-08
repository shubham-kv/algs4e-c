#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __int_stack_h
#define __int_stack_h

struct IntegerStackNode;
struct IntegerStack;
struct IntegerStackIterator;

typedef struct IntegerStack *IntStack;
typedef int64_t IntStackItem;
typedef struct IntegerStackIterator *IntStackIterator;

#define STACK IntStack
#define ITEM IntStackItem
#define ITER IntStackIterator
#define NODE struct IntegerStackNode *

struct IntegerStack {
  NODE top;
  size_t size;
};

STACK IntStack_Create();
 void IntStack_Init(STACK stack);
 void IntStack_Clear(STACK stack);
 void IntStack_Delete(STACK *stack);

  int IntStack_Push(STACK stack, ITEM item);
  int IntStack_Pop(STACK stack, ITEM *out);
  int IntStack_Peek(STACK stack, ITEM *out);
  int IntStack_Size(STACK stack);
 bool IntStack_IsEmpty(STACK stack);

struct IntegerStackIterator {
  NODE cur;
};

ITER IntStackIter_Create(STACK stack);
void IntStackIter_Init(ITER iterator, STACK stack);
void IntStackIter_Clear(ITER iterator);
void IntStackIter_Delete(ITER *iterator);
bool IntStackIter_HasNext(ITER iterator);
 int IntStackIter_GetNext(ITER iterator, ITEM *out);

#undef NODE
#undef ITER
#undef ITEM
#undef STACK

#endif // __int_stack_h
