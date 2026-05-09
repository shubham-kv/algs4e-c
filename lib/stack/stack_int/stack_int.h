#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __int_stack_h
#define __int_stack_h

struct IntegerStackNode;
struct IntegerStack;
struct IntegerStackIterator;

typedef int64_t IntStackItem;
typedef struct IntegerStackNode *IntStackNode;
typedef struct IntegerStack *IntStack;
typedef struct IntegerStackIterator *IntStackIterator;

#define STACK IntStack
#define ITEM IntStackItem
#define ITER IntStackIterator

struct IntegerStack {
  struct IntegerStackNode *top;
  size_t size;
};

STACK IntStack_Create();
  int IntStack_Init(STACK stack);
  int IntStack_Clear(STACK stack);
  int IntStack_Delete(STACK *stack);
  int IntStack_Push(STACK stack, ITEM item);
  int IntStack_Pop(STACK stack, ITEM *out);
  int IntStack_Peek(STACK stack, ITEM *out);
  int IntStack_Size(STACK stack);
 bool IntStack_IsEmpty(STACK stack);

struct IntegerStackIterator {
  struct IntegerStackNode *cur;
};

ITER IntStackIter_Create(STACK stack);
 int IntStackIter_Init(ITER iterator, STACK stack);
 int IntStackIter_Clear(ITER iterator);
 int IntStackIter_Delete(ITER *iterator);
bool IntStackIter_HasNext(ITER iterator);
 int IntStackIter_GetNext(ITER iterator, ITEM *out);

#undef ITER
#undef ITEM
#undef STACK

#endif // __int_stack_h
