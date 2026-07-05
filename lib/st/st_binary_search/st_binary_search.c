#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "common_macros.h"
#include "st_binary_search.h"

#define INITIAL_CAPACITY 1

struct BinarySearchST {
  ComparatorFn keyComparator;
  BSSTKey *keys;
  BSSTVal *values;
  int n;
  int capacity;
};

struct BinarySearchSTKeysIterator {
  BSST st;
  int i;
  int c;
  int n;
};

static int _resize(BSST st, const int newCapacity);
static bool _equals(BSST st, BSSTKey keyA, BSSTKey keyB);
static bool _less(BSST st, BSSTKey keyA, BSSTKey keyB);

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
  ENSURE_SUCCESS(BSST_Clear(*st));
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
  if (rank < st->n && _equals(st, key, st->keys[rank])) {
    st->values[rank] = val;
    return EXIT_SUCCESS;
  }

  // Double the capacity if this table is full
  if (st->n >= st->capacity) {
    ENSURE_SUCCESS(_resize(st, st->capacity * 2));
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

  if (rank < st->n && _equals(st, key, st->keys[rank])) {
    *out = st->values[rank];
  }

  return EXIT_SUCCESS;
}

int BSST_DeleteKey(BSST st, BSSTKey key) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  REQUIRE_TRUE(st->n > 0, ENODATA, EXIT_FAILURE);

  int rank = -1;
  ENSURE_SUCCESS(BSST_Rank(st, key, &rank));
  assert(rank >= 0);

  if (rank < st->n && _equals(st, key, st->keys[rank])) {
    for (int i = rank; i + 1 < st->n; i++) {
      st->keys[i] = st->keys[i + 1];
      st->values[i] = st->values[i + 1];
    }

    st->keys[st->n - 1] = NULL;
    st->values[st->n - 1] = NULL;
    st->n--;

    if (st->n > 0 && st->n < st->capacity / 4) {
      ENSURE_SUCCESS(_resize(st, st->capacity / 2));
    }

    return EXIT_SUCCESS;
  }

  REQUIRE_TRUE(0, EINVAL, EXIT_FAILURE);
  return EXIT_FAILURE;
}

int BSST_Contains(BSST st, BSSTKey key, bool *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  BSSTVal val = NULL;
  ENSURE_SUCCESS(BSST_Get(st, key, &val));
  *out = IS_NOT_NULL(val);
  return EXIT_SUCCESS;
}

inline int BSST_Size(BSST st) { return st->n; }
inline bool BSST_IsEmpty(BSST st) { return BSST_Size(st) <= 0; }

int BSST_Min(BSST st, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(st->n > 0, ENODATA, EXIT_FAILURE);
  *out = st->keys[0];
  return EXIT_SUCCESS;
}

int BSST_Max(BSST st, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(st->n > 0, ENODATA, EXIT_FAILURE);
  *out = st->keys[st->n - 1];
  return EXIT_SUCCESS;
}

int BSST_Floor(BSST st, BSSTKey key, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  int rank = -1;
  ENSURE_SUCCESS(BSST_Rank(st, key, &rank));
  assert(rank >= 0);

  if (rank < st->n && _equals(st, key, st->keys[rank])) {
    *out = st->keys[rank];
    return EXIT_SUCCESS;
  }

  return BSST_Select(st, rank - 1, out);
}

int BSST_Ceiling(BSST st, BSSTKey key, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  int rank = -1;
  ENSURE_SUCCESS(BSST_Rank(st, key, &rank));
  assert(rank >= 0);

  return BSST_Select(st, rank, out);
}

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

int BSST_Select(BSST st, int rank, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(rank >= 0, ERANGE, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  REQUIRE_TRUE(st->n > 0, ENODATA, EXIT_FAILURE);
  REQUIRE_TRUE(rank < st->n, ERANGE, EXIT_FAILURE);
  *out = st->keys[rank];
  return EXIT_SUCCESS;
}

int BSST_DeleteMin(BSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  BSSTKey min;
  ENSURE_SUCCESS(BSST_Min(st, &min));
  return BSST_DeleteKey(st, min);
}

int BSST_DeleteMax(BSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  BSSTKey max;
  ENSURE_SUCCESS(BSST_Max(st, &max));
  return BSST_DeleteKey(st, max);
}

int BSST_SizeOfRange(BSST st, BSSTKey low, BSSTKey high, int *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(low), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(high), EINVAL, EXIT_FAILURE);

  REQUIRE_TRUE(!_less(st, high, low), EINVAL, EXIT_FAILURE);

  int rankLow = -1;
  ENSURE_SUCCESS(BSST_Rank(st, low, &rankLow));
  assert(rankLow >= 0);

  int rankHigh = -1;
  ENSURE_SUCCESS(BSST_Rank(st, high, &rankHigh));
  assert(rankHigh >= 0);

  if (rankHigh < st->n && _equals(st, high, st->keys[rankHigh])) {
    *out = rankHigh - rankLow + 1;
  } else {
    *out = rankHigh - rankLow;
  }

  return EXIT_SUCCESS;
}

BSSTKeysIter BSSTKeysIter_Create(BSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);

  BSSTKeysIter iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  iterator->st = st;
  iterator->i = 0;
  iterator->c = 0;
  iterator->n = st->n;

  return iterator;
}

BSSTKeysIter BSSTKeysIter_CreateInRange(BSST st, BSSTKey low, BSSTKey high) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(low), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(high), EINVAL, NULL);
  REQUIRE_TRUE(!_less(st, high, low), EINVAL, NULL);

  BSSTKeysIter iterator = BSSTKeysIter_Create(st);
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  int rankLow = -1;
  int code = BSST_Rank(st, low, &rankLow);
  REQUIRE_TRUE(code == EXIT_SUCCESS, EINVAL, NULL);
  assert(rankLow >= 0);

  int rankHigh = -1;
  code = BSST_Rank(st, high, &rankHigh);
  REQUIRE_TRUE(code == EXIT_SUCCESS, EINVAL, NULL);
  assert(rankHigh >= 0);

  iterator->i = rankLow;

  if (rankHigh < st->n && _equals(st, high, st->keys[rankHigh])) {
    iterator->n = rankHigh - rankLow + 1;
  } else {
    iterator->n = rankHigh - rankLow;
  }

  return iterator;
}

int BSSTKeysIter_Delete(BSSTKeysIter *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

bool BSSTKeysIter_HasNext(BSSTKeysIter iterator) {
  return iterator->c < iterator->n;
}

int BSSTKeysIter_GetNext(BSSTKeysIter iterator, BSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(iterator->n > 0, ENODATA, EXIT_FAILURE);
  REQUIRE_TRUE(iterator->c < iterator->n, ENODATA, EXIT_FAILURE);

  *out = iterator->st->keys[iterator->i];
  iterator->i++;
  iterator->c++;
  return EXIT_SUCCESS;
}

// =====================================
// Utilities
// =====================================

static int _resize(BSST st, const int newCapacity) {
  st->capacity = newCapacity;

  st->keys = realloc(st->keys, newCapacity * sizeof(*st->keys));
  REQUIRE_TRUE(IS_NOT_NULL(st->keys), ENOMEM, EXIT_FAILURE);

  st->values = realloc(st->values, newCapacity * sizeof(*st->values));
  REQUIRE_TRUE(IS_NOT_NULL(st->values), ENOMEM, EXIT_FAILURE);

  return EXIT_SUCCESS;
}

static inline bool _equals(BSST st, BSSTKey keyA, BSSTKey keyB) {
  return st->keyComparator(keyA, keyB) == 0;
}

static inline bool _less(BSST st, BSSTKey keyA, BSSTKey keyB) {
  return st->keyComparator(keyA, keyB) < 0;
}
