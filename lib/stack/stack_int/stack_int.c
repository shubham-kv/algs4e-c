#include "stack_int.h"
#include "common_macros.h"
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

struct IntegerStackNode {
  IntStackItem item;
  struct IntegerStackNode *next;
};

IntStack IntStack_Create() {
  IntStack stack = calloc(1, sizeof(*stack));
  REQUIRE_TRUE(IS_NOT_NULL(stack), ENOMEM, NULL);

  IntStack_Init(stack);
  return stack;
}

int IntStack_Init(IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);
  stack->top = NULL;
  stack->size = 0;
  return EXIT_SUCCESS;
}

int IntStack_Clear(IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);

  while (!IntStack_IsEmpty(stack)) {
    IntStack_Pop(stack, NULL);
  }
  memset(stack, 0, sizeof(*stack));
  return EXIT_SUCCESS;
}

int IntStack_Delete(IntStack *stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack) && IS_NOT_NULL(*stack), EINVAL, EXIT_FAILURE);

  IntStack_Clear(*stack);
  free(*stack), (*stack = NULL);
  return EXIT_SUCCESS;
}

int IntStack_Push(IntStack stack, IntStackItem item) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);

  IntStackNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);
  node->item = item;
  node->next = stack->top;
  stack->top = node;
  stack->size++;

  return EXIT_SUCCESS;
}

int IntStack_Pop(IntStack stack, IntStackItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(stack->top), ENODATA, EXIT_FAILURE);

  if (IS_NOT_NULL(out)) {
    *out = stack->top->item;
  }

  IntStackNode next = stack->top->next;
  free(stack->top), (stack->top = NULL);
  stack->top = next;
  stack->size--;

  return EXIT_SUCCESS;
}

int IntStack_Peek(IntStack stack, IntStackItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(stack->top), ENODATA, EXIT_FAILURE);

  if (IS_NOT_NULL(out)) {
    *out = stack->top->item;
  }
  return EXIT_SUCCESS;
}

inline int IntStack_Size(IntStack stack) { return stack->size; }

inline bool IntStack_IsEmpty(IntStack stack) {
  return IntStack_Size(stack) == 0;
}

IntStackIterator IntStackIter_Create(IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, NULL);

  IntStackIterator iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  IntStackIter_Init(iterator, stack);
  return iterator;
}

inline int IntStackIter_Init(IntStackIterator iterator, IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);
  iterator->cur = stack->top;
  return EXIT_SUCCESS;
}

inline int IntStackIter_Clear(IntStackIterator iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

inline int IntStackIter_Delete(IntStackIterator *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);
  IntStackIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool IntStackIter_HasNext(IntStackIterator iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int IntStackIter_GetNext(IntStackIterator iterator, IntStackItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(iterator->cur), ENODATA, EXIT_FAILURE);
  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return EXIT_SUCCESS;
}
