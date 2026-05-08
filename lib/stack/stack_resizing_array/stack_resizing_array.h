#include <stdbool.h>
#include <stdlib.h>

#ifndef __resizing_array_stack_h
#define __resizing_array_stack_h

struct ResizingArrayStack;
struct RAStackIterator;

typedef const void *Item;
typedef struct ResizingArrayStack *Stack;
typedef struct RAStackIterator *StackIterator;

struct ResizingArrayStack {
  size_t capacity;
  size_t size;
  Item *items;
};

void Stack_Init(Stack stack);
void Stack_Clear(Stack stack);

void Stack_Push(Stack stack, Item item);
Item Stack_Pop(Stack stack);
Item Stack_Peek(Stack stack);
 int Stack_Size(Stack stack);
bool Stack_IsEmpty(Stack stack);

struct RAStackIterator {
  struct ResizingArrayStack *stack;
  size_t i;
};

void StackIterator_Init(StackIterator iterator, Stack stack);
void StackIterator_Clear(StackIterator iterator);
bool StackIterator_HasNext(StackIterator iterator);
Item StackIterator_GetNext(StackIterator iterator);

#endif // __resizing_array_stack_h
