#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq_max_heap.h"
#include "stack_linked.h"

int intCmp(const void *a, const void *b);

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % echo "10 20 35 100 85 67 35 18 22 21" > input.txt
 * % ./build/bin/pq_max_heap_client 3 < input.txt
 * 
 */
int main(int argc, const char *argv[]) {
  if (!(argc == 2)) {
    fprintf(stderr, "** Smallest k Integers **\n");
    fprintf(stderr, "Usage: %s <k>\n", argv[0]);
    fprintf(stderr, "%-10s %s", "k", "Positive integer, given k prints smallest k integers read from stdin");
    fprintf(stderr, "\n");
    return -1;
  }

  int k = -1;
  sscanf(argv[1], "%d", &k);

  if (k <= 0) {
    fprintf(stderr, "Expected positive integer for argument <k>, received %d\n", k);
    return -1;
  }

  printf("** Smallest k integers **\n");

  struct HeapMaxPQ _pq, *pq = &_pq;
  MaxPQ_Init(pq, intCmp);

  int buffer, i;

  for (
      i = 0;
      fscanf(stdin, "%d", &buffer) != EOF;
      i++
  ) {
    int *key = calloc(1, sizeof(int));
    *key = buffer;
    MaxPQ_Insert(pq, key);
    printf("insert(%d)\n", *key);

    if (MaxPQ_Size(pq) > k) {
      int *max = (int *) MaxPQ_DelMax(pq);
      printf("del_max() = %d\n", *max);
      free(max);
    }
  }
  printf("\n");

  printf("size() = %d\n", MaxPQ_Size(pq));

  struct LinkedStack _stack, *stack = &_stack;
  Stack_Init(stack);

  while (!MaxPQ_IsEmpty(pq)) {
    const int *max = (int *) MaxPQ_DelMax(pq);
    printf("del_max() = %d\n", *max);
    Stack_Push(stack, max);
  }
  printf("\n");

  printf("Smallest %d integers from total %d integers\n", k, i);

  struct LLStackIterator _iterator, *iterator = &_iterator;
  StackIterator_Init(iterator, stack);

  while (StackIterator_HasNext(iterator)) {
    const int *key = (int *) StackIterator_GetNext(iterator);
    printf("%d\n", *key);
  }
  printf("\n");

  StackIterator_Clear(iterator), (iterator = NULL);

  while (!Stack_IsEmpty(stack)) {
    free((int *) Stack_Pop(stack));
  }
  Stack_Clear(stack), (stack = NULL);
  MaxPQ_Clear(pq), (pq = NULL);

  return 0;
}

int intCmp(const void *a, const void *b) {
  int i = *(const int *)a;
  int j = *(const int *)b;
  return (i < j) ? -1 : (i > j) ? 1: 0;
}

