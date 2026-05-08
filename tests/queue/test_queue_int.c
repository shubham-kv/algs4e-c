#include <criterion/criterion.h>
#include "queue_int.h"

static IntQueue queue;
static const int SUCCESS_RETURN_CODE = 0;
static const int ERR_RETURN_CODE = -1;

static void setup() {
  queue = IntQueue_Create();
  cr_assert(queue != NULL);
}

static void teardown() {
  IntQueue_Delete(&queue);
}


Test(
    int_queue_enqueue,
    enqueues_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Peek(queue, NULL), ERR_RETURN_CODE);

  IntQueue_Enqueue(queue,  -1);
  IntQueue_Enqueue(queue,   0);
  IntQueue_Enqueue(queue,  +1);

  IntQueueItem item;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 3);
  cr_assert_eq(IntQueue_Peek(queue, &item), SUCCESS_RETURN_CODE);
  cr_assert_eq(item, -1);

  IntQueueItem expected, actual;
  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);

  expected = -1;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  expected = 0;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  expected = 1;
  cr_assert(IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
               expected, actual);

  cr_assert(!IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, NULL), ERR_RETURN_CODE);

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
  cr_assert_eq(IntQueue_Peek(queue, NULL), ERR_RETURN_CODE);

  const int integersSize = INT16_MAX * 2;
  IntQueueItem integers[integersSize];

  for (int i = 0, val = INT16_MIN; i < integersSize; i++, val++) {
    integers[i] = val;
    cr_assert_eq(IntQueue_Enqueue(queue, val), SUCCESS_RETURN_CODE);
  }

  IntQueueItem item;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), integersSize);
  cr_assert_eq(IntQueue_Peek(queue, &item), SUCCESS_RETURN_CODE);
  cr_assert_eq(item, integers[0]);

  IntQueueItem expected, actual;
  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);

  for (int i = 0; i < integersSize; i++) {
    expected = integers[i];
    cr_assert(IntQueueIter_HasNext(iterator));
    cr_assert_eq(IntQueueIter_GetNext(iterator, &actual), SUCCESS_RETURN_CODE);
    cr_assert_eq(expected, actual, "Expected next integer to be %lld, got %lld",
                 expected, actual);
  }

  cr_assert(!IntQueueIter_HasNext(iterator));
  cr_assert_eq(IntQueueIter_GetNext(iterator, NULL), ERR_RETURN_CODE);

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
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), ERR_RETURN_CODE);

  IntQueue_Enqueue(queue,  -1);
  IntQueue_Enqueue(queue,   0);
  IntQueue_Enqueue(queue,  +1);

  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 3);

  IntQueueItem actual, expected;

  expected = -1;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  expected = 0;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  expected = 1;
  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Dequeue(queue, &actual), SUCCESS_RETURN_CODE);
  cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
               expected, actual);

  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), ERR_RETURN_CODE);
}


Test(
    int_queue_dequeue,
    dequeues_large_number_of_items,
    .init = setup,
    .fini = teardown
) {
  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), ERR_RETURN_CODE);

  const int integersSize = INT16_MAX;
  IntQueueItem integers[integersSize];

  for (int i = 0, val = INT16_MIN; i < integersSize; i++, val++) {
    integers[i] = val;
    cr_assert_eq(IntQueue_Enqueue(queue, val), SUCCESS_RETURN_CODE);
  }

  cr_assert(!IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), integersSize);

  IntQueueItem expected, actual;

  for (int i = 0; i < integersSize; i++) {
    expected = integers[i];
    cr_assert(!IntQueue_IsEmpty(queue));
    cr_assert_eq(IntQueue_Dequeue(queue, &actual), SUCCESS_RETURN_CODE);
    cr_assert_eq(expected, actual, "Expected %lld to be dequeued, got %lld",
                 expected, actual);
  }

  cr_assert(IntQueue_IsEmpty(queue));
  cr_assert_eq(IntQueue_Size(queue), 0);
  cr_assert_eq(IntQueue_Dequeue(queue, NULL), ERR_RETURN_CODE);
}

