#include "stack_int.h"
#include "common_macros.h"
#include <errno.h>
#include <stdlib.h>
#include <memory.h>

#define STACK IntStack
#define ITEM IntStackItem
#define ITER IntStackIterator
#define NODE struct IntegerStackNode *

struct IntegerStackNode {
  ITEM item;
  NODE next;
};

STACK IntStack_Create() {
  STACK stack = calloc(1, sizeof(*stack));
  if (IS_NULL(stack)) { errno = ENOMEM; return NULL; }

  IntStack_Init(stack);
  return stack;
}

void IntStack_Init(STACK stack) {
  if (IS_NULL(stack)) { return; }
  stack->top = NULL;
  stack->size = 0;
}

void IntStack_Clear(STACK stack) {
  if (IS_NULL(stack)) { return; }

  while (!IntStack_IsEmpty(stack)) {
    IntStack_Pop(stack, NULL);
  }
}

void IntStack_Delete(STACK *stack) {
  if (IS_NULL(stack) || IS_NULL(*stack)) { return; }

  IntStack_Clear(*stack);
  free(*stack), (*stack = NULL);
}

int IntStack_Push(STACK stack, ITEM data) {
  if (IS_NULL(stack)) { errno = EINVAL; return -1; }

  NODE node = calloc(1, sizeof(*node));
  if (IS_NULL(node)) { errno = ENOMEM; return -1; }

  node->item = data;
  node->next = stack->top;
  stack->top = node;
  stack->size++;

  return 0;
}

int IntStack_Pop(STACK stack, ITEM *out) {
  if (IS_NULL(stack)) { errno = EINVAL; return -1; }
  if (IS_NULL(stack->top)) { errno = ENODATA; return -1; }

  if (IS_NOT_NULL(out)) {
    *out = stack->top->item;
  }

  NODE next = stack->top->next;
  free(stack->top), (stack->top = NULL);
  stack->top = next;
  stack->size--;

  return 0;
}

int IntStack_Peek(STACK stack, ITEM *out) {
  if (IS_NULL(stack)) { errno = EINVAL; return -1; }
  if (IS_NULL(stack->top)) { errno = ENODATA; return -1; }

  if (IS_NOT_NULL(out)) {
    *out = stack->top->item;
  }
  return 0;
}

inline int IntStack_Size(STACK stack) {
  return stack->size;
}

inline bool IntStack_IsEmpty(STACK stack) {
  return IntStack_Size(stack) == 0;
}

ITER IntStackIter_Create(STACK stack) {
  if (IS_NULL(stack)) { errno = EINVAL; return NULL; }

  ITER iterator = calloc(1, sizeof(*iterator));
  if (IS_NULL(iterator)) { errno = ENOMEM; return NULL; }

  IntStackIter_Init(iterator, stack);
  return iterator;
}

inline void IntStackIter_Init(ITER iterator, STACK stack) {
  iterator->cur = stack->top;
}

inline void IntStackIter_Clear(ITER iterator) {
  memset(iterator, 0, sizeof(*iterator));
}

inline void IntStackIter_Delete(ITER *iterator) {
  IntStackIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
}

inline bool IntStackIter_HasNext(ITER iterator) {
  return IS_NULL(iterator->cur) ? false : true;
}

int IntStackIter_GetNext(ITER iterator, ITEM *out) {
  if (IS_NULL(iterator)) { errno = EINVAL; return -1; }
  if (IS_NULL(iterator->cur)) { errno = ENODATA; return -1; }

  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return 0;
}

#undef NODE
#undef ITER
#undef ITEM
#undef STACK
