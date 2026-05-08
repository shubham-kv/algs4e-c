#include <stdbool.h>
#include <stdlib.h>

#ifndef __linked_list_stack_h
#define __linked_list_stack_h

struct StackNode;
struct LinkedListStack;
struct LLStackIterator;

typedef const void *Item;
typedef struct LinkedListStack *Stack;
typedef struct LLStackIterator *StackIterator;

struct LinkedListStack {
  struct StackNode *top;
  int size;
};

void Stack_Init(Stack stack);
void Stack_Clear(Stack stack);

void Stack_Push(Stack stack, Item item);
Item Stack_Pop(Stack stack);
Item Stack_Peek(Stack stack);
 int Stack_Size(Stack stack);
bool Stack_IsEmpty(Stack stack);

struct LLStackIterator {
  struct LinkedListStack *stack;
  struct StackNode *cur;
};

void StackIterator_Init(StackIterator iterator, Stack stack);
void StackIterator_Clear(StackIterator iterator);
bool StackIterator_HasNext(StackIterator iterator);
Item StackIterator_GetNext(StackIterator iterator);

#endif // __linked_list_stack_h
