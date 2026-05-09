#include <criterion/criterion.h>
#include <stdlib.h>
#include "queue_int.h"

static struct IntegerQueue _queue, *queue = &_queue;

static void setup() {
  IntQueue_Init(queue);
  cr_assert(queue != NULL);
}

static void teardown() {
  IntQueue_Clear(queue);
}


Test(
    int_queue_enqueue,
    enqueues_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Peek(queue, NULL), EXIT_FAILURE);

  IntQueue_Enqueue(queue,  -1);
  IntQueue_Enqueue(queue,   0);
  IntQueue_Enqueue(queue,  +1);

  IntQueueItem item;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 3);
  cr_assert_eq(IntQueue_Peek(queue, &item), EXIT_SUCCESS);
  cr_assert_eq(item, -1);

  IntQueueItem expected, actual;
  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);

  expected = -1;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  expected = 0;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  expected = 1;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  cr_assert(!IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, NULL), EXIT_FAILURE);

  IntQueueIter_Clear(iterator);
}


Test(
    int_queue_enqueue,
    enqueues_large_number_of_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Peek(queue, NULL), EXIT_FAILURE);

  const int integersSize = INT16_MAX * 2;
  IntQueueItem integers[integersSize];

  for (int i = 0, val = INT16_MIN; i < integersSize; i++, val++) {
    integers[i] = val;
    cr_assert_eq(IntQueue_Enqueue(queue, val), EXIT_SUCCESS);
  }

  IntQueueItem item;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), integersSize);
  cr_assert_eq(IntQueue_Peek(queue, &item), EXIT_SUCCESS);
  cr_assert_eq(item, integers[0]);

  IntQueueItem expected, actual;
  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);

  for (int i = 0; i < integersSize; i++) {
    expected = integers[i];
    cr_assert(IntQueueIter_HasNext(iterator));
    cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), EXIT_SUCCESS);
    cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
                 expected, actual);
  }

  cr_assert(!IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, NULL), EXIT_FAILURE);

  IntQueueIter_Clear(iterator);
}


Test(
    int_queue_dequeue,
    dequeues_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), EXIT_FAILURE);

  IntQueue_Enqueue(queue,  -1);
  IntQueue_Enqueue(queue,   0);
  IntQueue_Enqueue(queue,  +1);

  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 3);

  IntQueueItem actual, expected;

  expected = -1;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  expected = 0;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  expected = 1;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), EXIT_SUCCESS);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), EXIT_FAILURE);
}


Test(
    int_queue_dequeue,
    dequeues_large_number_of_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), EXIT_FAILURE);

  const int integersSize = INT16_MAX;
  IntQueueItem integers[integersSize];

  for (int i = 0, val = INT16_MIN; i < integersSize; i++, val++) {
    integers[i] = val;
    cr_assert_eq(IntQueue_Enqueue(queue, val), EXIT_SUCCESS);
  }

  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), integersSize);

  IntQueueItem expected, actual;

  for (int i = 0; i < integersSize; i++) {
    expected = integers[i];
    cr_assert(!IntQueue_IsEmpty(queue));
    cr_assert_eq(IntQueue_Dequeue(queue, &actual), EXIT_SUCCESS);
    cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
                 expected, actual);
  }

  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), EXIT_FAILURE);
}

