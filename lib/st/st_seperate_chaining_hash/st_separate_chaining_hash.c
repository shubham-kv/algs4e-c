#include <stdint.h>
#include <stdlib.h>

#include "common_macros.h"
#include "comparators.h"
#include "st_separate_chaining_hash.h"

struct SeparateChainingHashSTNode;
typedef struct SeparateChainingHashSTNode *SCHSTNode;

struct SeparateChainingHashSTNode {
  struct SeparateChainingHashSTNode *next;
  SCHSTKey key;
  SCHSTVal val;
};

struct SeparateChainingHashST {
  ComparatorFn keyComparator;
  HashFn keyHasher;
  SCHSTNode *nodes;
  size_t tableSize;
  uint32_t n;
};

struct SeparateChainingHashSTKeysIterator {
  SCHSTKey *keys;
  int n;
  int i;
};

static bool _equals(SCHST st, SCHSTKey a, SCHSTKey b);

static int SCHST_Init(SCHST st, ComparatorFn keyComparator, HashFn keyHasher,
                      uint32_t tableSize) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(keyHasher), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(tableSize > 0, EINVAL, EXIT_FAILURE);

  st->keyComparator = keyComparator;
  st->keyHasher = keyHasher;

  SCHSTNode *nodes = calloc(tableSize, sizeof(*nodes));
  REQUIRE_TRUE(IS_NOT_NULL(nodes), ENOMEM, EXIT_FAILURE);

  st->nodes = nodes;
  st->tableSize = tableSize;
  st->n = 0;

  return EXIT_SUCCESS;
}

SCHST SCHST_Create(ComparatorFn keyComparator, HashFn keyHasher,
                   uint32_t tableSize) {
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(keyHasher), EINVAL, NULL);

  SCHST st = calloc(1, sizeof(*st));
  REQUIRE_TRUE(IS_NOT_NULL(st), ENOMEM, NULL);

  const int code = SCHST_Init(st, keyComparator, keyHasher, tableSize);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }

  return st;
}

static int SCHST_Clear(SCHST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);

  for (int i = 0; i < st->tableSize; i++) {
    if (IS_NOT_NULL(st->nodes[i])) {
      SCHSTNode cur = st->nodes[i], next = NULL;
      st->nodes[i] = NULL;

      for (; IS_NOT_NULL(cur); cur = next) {
        next = cur->next;
        free(cur), (cur = NULL);
      }
    }
  }

  free(st->nodes), (st->nodes = NULL);
  memset(st, 0, sizeof(*st));
  return EXIT_SUCCESS;
}

int SCHST_Delete(SCHST *st) {
  REQUIRE_TRUE(IS_NOT_NULL(st) && IS_NOT_NULL(*st), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(SCHST_Clear(*st));
  free(*st), (*st = NULL);
  return EXIT_SUCCESS;
}

int SCHST_Put(SCHST st, SCHSTKey key, SCHSTVal val) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  if (IS_NULL(val)) {
    return SCHST_DeleteKey(st, key);
  }

  const int hash = st->keyHasher(key, st->tableSize);
  SCHSTNode prev = NULL, cur = st->nodes[hash];

  for (; IS_NOT_NULL(cur); prev = cur, cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      cur->val = val;
      return EXIT_SUCCESS;
    }
  }

  SCHSTNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);

  node->next = NULL;
  node->key = key;
  node->val = val;

  if (IS_NULL(st->nodes[hash])) {
    st->nodes[hash] = node;
  } else {
    prev->next = node;
  }
  st->n++;

  return EXIT_SUCCESS;
}

int SCHST_Get(SCHST st, SCHSTKey key, SCHSTVal *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  const int hash = st->keyHasher(key, st->tableSize);
  SCHSTNode cur = st->nodes[hash];

  for (; IS_NOT_NULL(cur); cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      if (IS_NOT_NULL(out)) {
        *out = cur->val;
      }
      return EXIT_SUCCESS;
    }
  }

  return EXIT_SUCCESS;
}

int SCHST_DeleteKey(SCHST st, SCHSTKey key) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  const int hash = st->keyHasher(key, st->tableSize);
  SCHSTNode prev = NULL, cur = st->nodes[hash];

  for (; IS_NOT_NULL(cur); prev = cur, cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      if (IS_NOT_NULL(prev)) {
        prev->next = cur->next;
      } else {
        st->nodes[hash] = cur->next;
      }

      free(cur), cur = NULL;
      st->n--;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_SUCCESS;
}

int SCHST_Contains(SCHST st, SCHSTKey key, bool *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  SCHSTVal val = NULL;
  ENSURE_SUCCESS(SCHST_Get(st, key, &val));
  *out = IS_NOT_NULL(val);
  return EXIT_SUCCESS;
}

inline int SCHST_Size(SCHST st) { return st->n; }

inline bool SCHST_IsEmpty(SCHST st) { return SCHST_Size(st) <= 0; }

static int SCHSTKeysIter_Init(SCHSTKeysIter iterator, SCHST st) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);

  iterator->keys = NULL;
  iterator->i = 0;
  iterator->n = 0;

  if (st->n > 0) {
    SCHSTKey *keys = calloc(st->n, sizeof(*keys));
    REQUIRE_TRUE(IS_NOT_NULL(keys), ENOMEM, EXIT_FAILURE);

    for (int i = 0; i < st->tableSize; i++) {
      if (IS_NOT_NULL(st->nodes[i])) {
        SCHSTNode cur = st->nodes[i];

        for (; IS_NOT_NULL(cur); cur = cur->next) {
          keys[iterator->n++] = cur->key;
        }
      }
    }

    iterator->keys = keys;
  }

  return EXIT_SUCCESS;
}

SCHSTKeysIter SCHSTKeysIter_Create(SCHST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);

  SCHSTKeysIter iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  const int code = SCHSTKeysIter_Init(iterator, st);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }

  return iterator;
}

static int SCHSTKeysIter_Clear(SCHSTKeysIter iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  if (IS_NOT_NULL(iterator->keys)) {
    free(iterator->keys), (iterator->keys = NULL);
  }
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int SCHSTKeysIter_Delete(SCHSTKeysIter *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);
  ENSURE_SUCCESS(SCHSTKeysIter_Clear(*iterator));
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool SCHSTKeysIter_HasNext(SCHSTKeysIter iterator) {
  return iterator->i < iterator->n;
}

int SCHSTKeysIter_GetNext(SCHSTKeysIter iterator, SCHSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  *out = iterator->keys[iterator->i++];
  return EXIT_SUCCESS;
}

static bool _equals(SCHST st, SCHSTKey a, SCHSTKey b) {
  return st->keyComparator(a, b) == 0;
}
