#include "stack_resizing_array.h"
#include <memory.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 8

void Stack_Init(Stack stack) {
  stack->capacity = INITIAL_CAPACITY;
  stack->size = 0;
  stack->items = calloc(stack->capacity, sizeof(*stack->items));
}

void Stack_Clear(Stack stack) {
  free(stack->items), (stack->items = NULL);
  memset(stack, 0, sizeof(*stack));
}

static void resize(Stack stack, size_t newCapacity) {
  stack->capacity = newCapacity;
  stack->items = realloc(stack->items, newCapacity * sizeof(*stack->items));
}

void Stack_Push(Stack stack, Item item) {
  if (stack->size < stack->capacity) {
    resize(stack, stack->capacity * 2);
  }
  stack->items[stack->size++] = item;
}

Item Stack_Pop(Stack stack) {
  if (!(stack->size > 0)) {
    return NULL;
  }

  Item top = stack->items[stack->size - 1];
  stack->items[stack->size - 1] = NULL;
  stack->size--;

  if (stack->size < stack->capacity / 4) {
    resize(stack, stack->capacity / 2);
  }

  return top;
}

inline Item Stack_Peek(Stack stack) {
  return !(stack->size > 0) ? NULL : stack->items[stack->size - 1];
}

inline int Stack_Size(Stack stack) {
  return stack->size;
}

inline bool Stack_IsEmpty(Stack stack) {
  return Stack_Size(stack) == 0;
}

inline void StackIterator_Init(StackIterator iterator, Stack stack) {
  iterator->stack = stack, iterator->i = stack->size - 1;
}

inline void StackIterator_Clear(StackIterator iterator) {
  memset(iterator, 0, sizeof(*iterator));
}

inline bool StackIterator_HasNext(StackIterator iterator) {
  return 0 <= iterator->i && iterator->i < iterator->stack->size;
}

inline Item StackIterator_GetNext(StackIterator iterator) {
  return StackIterator_HasNext(iterator) ? iterator->stack->items[iterator->i--]
                                         : NULL;
}

#undef INITIAL_CAPACITY
