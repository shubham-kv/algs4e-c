#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "common_macros.h"
#include "comparators.h"
#include "linked_node.h"
#include "st_sequential_search.h"

struct SequentialSearchSTNode;
typedef struct SequentialSearchSTNode *SSSTNode;

struct SequentialSearchST {
  ComparatorFn keyComparator;
  struct SequentialSearchSTNode *first;
  int n;
};

struct SequentialSearchSTNode {
  struct SequentialSearchSTNode *next;
  SSSTKey key;
  SSSTVal val;
};

struct SequentialSearchSTKeysIterator {
  struct SequentialSearchSTNode *cur;
};

static bool _equals(SSST st, SSSTKey a, SSSTKey b);

static int SSST_Init(SSST st, ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, EXIT_FAILURE);
  st->keyComparator = keyComparator;
  st->first = NULL;
  st->n = 0;
  return EXIT_SUCCESS;
}

SSST SSST_Create(ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, NULL);

  SSST st = calloc(1, sizeof(*st));
  REQUIRE_TRUE(IS_NOT_NULL(st), ENOMEM, NULL);

  const int code = SSST_Init(st, keyComparator);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }
  return st;
}

static int SSST_Clear(SSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);

  SSSTNode cur = st->first, next = NULL;
  st->first = NULL;

  for (; IS_NOT_NULL(cur); cur = next) {
    next = cur->next;
    free(cur), (cur = NULL);
  }

  memset(st, 0, sizeof(*st));
  return EXIT_SUCCESS;
}

int SSST_Delete(SSST *st) {
  REQUIRE_TRUE(IS_NOT_NULL(st) && IS_NOT_NULL(*st), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(SSST_Clear(*st));
  free(*st), (*st = NULL);
  return EXIT_SUCCESS;
}

int SSST_Put(SSST st, SSSTKey key, SSSTVal val) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  if (IS_NULL(val)) {
    return SSST_DeleteKey(st, key);
  }

  SSSTNode cur = st->first;
  for (; IS_NOT_NULL(cur); cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      cur->val = val;
      return EXIT_SUCCESS;
    }
  }

  SSSTNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);

  node->key = key;
  node->val = val;
  node->next = st->first;
  st->first = node;
  st->n++;

  return EXIT_SUCCESS;
}

int SSST_Get(SSST st, SSSTKey key, SSSTVal *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  SSSTNode cur = st->first;
  for (; IS_NOT_NULL(cur); cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      *out = cur->val;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_SUCCESS;
}

int SSST_DeleteKey(SSST st, SSSTKey key) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  SSSTNode prev = NULL, cur = st->first;
  for (; IS_NOT_NULL(cur); prev = cur, cur = cur->next) {
    if (_equals(st, cur->key, key)) {
      if (IS_NOT_NULL(prev)) {
        prev->next = cur->next;
      } else {
        st->first = st->first->next;
      }

      free(cur), (cur = NULL);
      st->n--;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_SUCCESS;
}

int SSST_Contains(SSST st, SSSTKey key, bool *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  SSSTVal val = NULL;
  ENSURE_SUCCESS(SSST_Get(st, key, &val));
  *out = IS_NOT_NULL(val);
  return EXIT_SUCCESS;
}

inline int SSST_Size(SSST st) { return st->n; }
inline bool SSST_IsEmpty(SSST st) { return SSST_Size(st) <= 0; }

SSSTKeysIter SSSTKeysIter_Create(SSST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);
  return (SSSTKeysIter)LinkedNodeIter_Create((LinkedNode)st->first);
}

inline int SSSTKeysIter_Delete(SSSTKeysIter *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool SSSTKeysIter_HasNext(SSSTKeysIter iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int SSSTKeysIter_GetNext(SSSTKeysIter iterator, SSSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  SSSTNode cur;
  const int code =
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur);

  if (code != EXIT_SUCCESS) {
    return code;
  }

  *out = cur->key;
  return EXIT_SUCCESS;
}

static bool _equals(SSST st, SSSTKey a, SSSTKey b) {
  return st->keyComparator(a, b) == 0;
}
