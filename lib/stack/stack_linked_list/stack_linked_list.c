#include "stack_linked_list.h"
#include <common_macros.h>
#include <memory.h>
#include <stdlib.h>

struct StackNode {
  Item item;
  struct StackNode *next;
};
typedef struct StackNode *Node;

void Stack_Init(Stack stack) {
  stack->top = NULL;
  stack->size = 0;
}

void Stack_Clear(Stack stack) {
  while (!Stack_IsEmpty(stack)) {
    Stack_Pop(stack);
  }
}

void Stack_Push(Stack stack, Item item) {
  Node node = calloc(1, sizeof(*node));
  node->item = item;
  node->next = stack->top;
  stack->top = node;
  stack->size++;
}

Item Stack_Pop(Stack stack) {
  if (IS_NULL(stack->top)) {
    return NULL;
  }

  Item item = stack->top->item;
  Node next = stack->top->next;
  free(stack->top), (stack->top = NULL);

  stack->top = next;
  stack->size--;
  return item;
}

inline Item Stack_Peek(Stack stack) {
  return IS_NULL(stack->top) ? NULL : stack->top->item;
}

inline int Stack_Size(Stack stack) {
  return stack->size;
}

inline bool Stack_IsEmpty(Stack stack) {
  return Stack_Size(stack) == 0;
}

inline void StackIterator_Init(StackIterator iterator, Stack stack) {
  iterator->stack = stack, iterator->cur = iterator->stack->top;
}

inline void StackIterator_Clear(StackIterator iterator) {
  memset(iterator, 0, sizeof(*iterator));
}

inline bool StackIterator_HasNext(StackIterator iterator) {
  return iterator->cur != NULL;
}

Item StackIterator_GetNext(StackIterator iterator) {
  if (IS_NULL(iterator->cur)) {
    return NULL;
  }
  Item item = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return item;
}
