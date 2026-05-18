#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "common_macros.h"
#include "linked_node.h"
#include "stack_int.h"

struct IntegerStackNode {
  struct IntegerStackNode *next;
  IntStackItem item;
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

inline int IntStackIter_Init(IntStackIterator iterator, IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, EXIT_FAILURE);
  return LinkedNodeIter_Init((LinkedNodeIter)iterator, (LinkedNode)stack->top);
}

IntStackIterator IntStackIter_Create(IntStack stack) {
  REQUIRE_TRUE(IS_NOT_NULL(stack), EINVAL, NULL);
  return (IntStackIterator)LinkedNodeIter_Create((LinkedNode)stack->top);
}

inline int IntStackIter_Clear(IntStackIterator iterator) {
  return LinkedNodeIter_Clear((LinkedNodeIter)iterator);
}

inline int IntStackIter_Delete(IntStackIterator *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool IntStackIter_HasNext(IntStackIterator iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int IntStackIter_GetNext(IntStackIterator iterator, IntStackItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  IntStackNode cur;
  const int code =
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur);

  if (code != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  *out = cur->item;
  return EXIT_SUCCESS;
}
