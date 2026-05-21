#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "common_macros.h"
#include "st_binary_search.h"

#define INITIAL_CAPACITY 1

#define ENSURE_SUCCESS(_callExpr)                                              \
  do {                                                                         \
    const int code = _callExpr;                                                \
    if (code != EXIT_SUCCESS) {                                                \
      return code;                                                             \
    }                                                                          \
  } while (0)

struct BinarySearchST {
  ComparatorFn keyComparator;
  BSSTKey *keys;
  BSSTVal *values;
  int n;
  int capacity;
};

static int _resizeArrays(BSST st, const int newCapacity);
static bool _keysEqual(BSST st, BSSTKey keyA, BSSTKey keyB);

static int BSST_Init(BSST st, ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, EXIT_FAILURE);

  st->keyComparator = keyComparator;
  st->n = 0;
  st->capacity = INITIAL_CAPACITY;

  st->keys = calloc(st->capacity, sizeof(*st->keys));
  REQUIRE_TRUE(IS_NOT_NULL(st->keys), ENOMEM, EXIT_FAILURE);

  st->values = calloc(st->capacity, sizeof(*st->values));
  REQUIRE_TRUE(IS_NOT_NULL(st->values), ENOMEM, EXIT_FAILURE);

  return EXIT_SUCCESS;
}

BSST BSST_Create(ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, NULL);

  BSST st = calloc(1, sizeof(*st));
  REQUIRE_TRUE(IS_NOT_NULL(st), ENOMEM, NULL);

  const int code = BSST_Init(st, keyComparator);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }
  return st;
}

static int BSST_Clear(BSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);

  free(st->keys), (st->keys = NULL);
  free(st->values), (st->values = NULL);
  memset(st, 0, sizeof(*st));
  return EXIT_SUCCESS;
}

int BSST_Delete(BSST *st) {
  REQUIRE_TRUE(IS_NOT_NULL(st) && IS_NOT_NULL(*st), EINVAL, EXIT_FAILURE);

  BSST_Clear(*st);
  free(*st), (*st = NULL);
  return EXIT_SUCCESS;
}

int BSST_Put(BSST st, BSSTKey key, BSSTVal val) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  if (IS_NULL(val)) {
    return BSST_DeleteKey(st, key);
  }

  int rank = -1;
  ENSURE_SUCCESS(BSST_Rank(st, key, &rank));
  assert(rank >= 0);

  // If key already exists then update the value and return
  if (rank < st->n && _keysEqual(st, key, st->keys[rank])) {
    st->values[rank] = val;
    return EXIT_SUCCESS;
  }

  // Double the capacity if this table is full
  if (st->n >= st->capacity) {
    ENSURE_SUCCESS(_resizeArrays(st, st->capacity * 2));
  }

  // Shift entries from rank until n-1 (+1 for new entry) to right by 1 unit
  for (int j = st->n; j - 1 >= rank; j--) {
    st->keys[j] = st->keys[j - 1];
    st->values[j] = st->values[j - 1];
  }

  st->keys[rank] = key;
  st->values[rank] = val;
  st->n++;

  return EXIT_SUCCESS;
}

int BSST_Get(BSST st, BSSTKey key, BSSTVal *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  REQUIRE_TRUE(st->n > 0, ENODATA, EXIT_FAILURE);

  int rank = -1;
  ENSURE_SUCCESS(BSST_Rank(st, key, &rank));
  assert(rank >= 0);

  if (rank < st->n && _keysEqual(st, key, st->keys[rank])) {
    *out = st->values[rank];
    return EXIT_SUCCESS;
  }

  REQUIRE_TRUE(0, EINVAL, EXIT_FAILURE);
  return EXIT_FAILURE;
}

int BSST_DeleteKey(BSST st, BSSTKey key) { return EXIT_SUCCESS; }

bool BSST_Contains(BSST st, BSSTKey key);
int BSST_Size(BSST st);
bool BSST_IsEmpty(BSST st);

int BSST_Min(BSST st, BSSTKey *out);
int BSST_Max(BSST st, BSSTKey *out);
int BSST_Floor(BSST st, BSSTKey key, BSSTKey *out);
int BSST_Ceiling(BSST st, BSSTKey key, BSSTKey *out);

int BSST_Rank(BSST st, BSSTKey key, int *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  int low = 0, high = st->n - 1;

  while (low <= high) {
    const int mid = (low + high) / 2;
    const int cmp = st->keyComparator(key, st->keys[mid]);

    if (cmp < 0) {
      high = mid - 1;
    } else if (cmp > 0) {
      low = mid + 1;
    } else {
      *out = mid;
      return EXIT_SUCCESS;
    }
  }

  *out = low;
  return EXIT_SUCCESS;
}

int BSST_Select(BSST st, int rank, BSSTKey *out);
int BSST_DeleteMin(BSST st);
int BSST_DeleteMax(BSST st);
int BSST_SizeOfRange(BSST st, BSSTKey low, BSSTKey high);

BSSTKeysIter BSSTKeysIter_Create(BSST st);
BSSTKeysIter BSSTKeysIter_CreateInRange(BSST st, BSSTKey low, BSSTKey high);
int BSSTKeysIter_Delete(BSSTKeysIter *iterator);
bool BSSTKeysIter_HasNext(BSSTKeysIter iterator);
int BSSTKeysIter_GetNext(BSSTKeysIter iterator, BSSTKey *out);

static int _resizeArrays(BSST st, const int newCapacity) {
  st->capacity = newCapacity;

  st->keys = realloc(st->keys, newCapacity * sizeof(*st->keys));
  REQUIRE_TRUE(IS_NOT_NULL(st->keys), ENOMEM, EXIT_FAILURE);

  st->values = realloc(st->values, newCapacity * sizeof(*st->values));
  REQUIRE_TRUE(IS_NOT_NULL(st->values), ENOMEM, EXIT_FAILURE);

  return EXIT_SUCCESS;
}

static inline bool _keysEqual(BSST st, BSSTKey keyA, BSSTKey keyB) {
  return st->keyComparator(keyA, keyB) == 0;
}
