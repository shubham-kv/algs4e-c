#include <stdbool.h>
#include <stdlib.h>

#ifndef __fixed_capacity_stack_h
#define __fixed_capacity_stack_h

struct FixedCapacityStack;
struct FCStackIterator;

typedef const void *Item;
typedef struct FixedCapacityStack *Stack;
typedef struct FCStackIterator *StackIterator;

struct FixedCapacityStack {
  size_t capacity;
  size_t size;
  Item *items;
};

void Stack_Init(Stack stack, size_t capacity);
void Stack_Clear(Stack stack);

void Stack_Push(Stack stack, Item item);
Item Stack_Pop(Stack stack);
Item Stack_Peek(Stack stack);
 int Stack_Size(Stack stack);
bool Stack_IsEmpty(Stack stack);

struct FCStackIterator {
  Stack stack;
  size_t i;
};

void StackIterator_Init(StackIterator iterator, Stack stack);
void StackIterator_Clear(StackIterator iterator);
bool StackIterator_HasNext(StackIterator iterator);
Item StackIterator_GetNext(StackIterator iterator);

#endif // __fixed_capacity_stack_h
