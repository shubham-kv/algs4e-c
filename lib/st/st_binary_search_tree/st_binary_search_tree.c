#include <errno.h>
#include <stdlib.h>

#include "common_macros.h"
#include "queue_linked.h"
#include "st_binary_search_tree.h"

struct BinarySearchTreeNode;
typedef struct BinarySearchTreeNode *BSTNode;

struct BinarySearchTreeNode {
  struct BinarySearchTreeNode *left;
  struct BinarySearchTreeNode *right;
  int n;
  BSTKey key;
  BSTVal val;
};

struct BinarySearchTree {
  ComparatorFn keyComparator;
  struct BinarySearchTreeNode *root;
};

struct BinarySearchTreeKeysIterator {
  struct BinarySearchTree *bst;
  Queue keys;
  QueueIterator keysIterator;
};

static int _nodeSize(BSTNode node);
static BSTKey _nodeKey(BSTNode node);
static bool _less(BST st, BSTKey keyA, BSTKey keyB);

static BSTNode min(BSTNode node);
static BSTNode max(BSTNode node);
static BSTNode deleteMin(BSTNode node, const bool shouldFree);
static BSTNode deleteMax(BSTNode node, const bool shouldFree);

static int BST_Init(BST st, ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, EXIT_FAILURE);
  st->keyComparator = keyComparator;
  st->root = NULL;
  return EXIT_SUCCESS;
}

BST BST_Create(ComparatorFn keyComparator) {
  REQUIRE_TRUE(IS_NOT_NULL(keyComparator), EINVAL, NULL);

  BST st = calloc(1, sizeof(*st));
  REQUIRE_TRUE(IS_NOT_NULL(st), ENOMEM, NULL);

  const int code = BST_Init(st, keyComparator);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }

  return st;
}

static int _delete(BSTNode node) {
  if (IS_NULL(node)) {
    return EXIT_SUCCESS;
  }
  ENSURE_SUCCESS(_delete(node->left));
  ENSURE_SUCCESS(_delete(node->right));
  free(node), (node = NULL);
  return EXIT_SUCCESS;
}

static int BST_Clear(BST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(_delete(st->root));
  memset(st, 0, sizeof(*st));
  return EXIT_SUCCESS;
}

int BST_Delete(BST *st) {
  REQUIRE_TRUE(IS_NOT_NULL(st) && IS_NOT_NULL(*st), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(BST_Clear(*st));
  free(*st), (*st = NULL);
  return EXIT_SUCCESS;
}

static int put(BST st, BSTNode *out, BSTKey key, BSTVal val) {
  BSTNode node = *out;

  if (IS_NULL(node)) {
    node = calloc(1, sizeof(*node));
    REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);

    node->key = key;
    node->val = val;
    node->n = 1;
    *out = node;
    return EXIT_SUCCESS;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    ENSURE_SUCCESS(put(st, &node->left, key, val));
  } else if (cmp > 0) {
    ENSURE_SUCCESS(put(st, &node->right, key, val));
  } else {
    node->val = val;
  }

  node->n = _nodeSize(node->left) + 1 + _nodeSize(node->right);

  return EXIT_SUCCESS;
}

int BST_Put(BST st, BSTKey key, BSTVal val) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  if (IS_NULL(val)) {
    return BST_DeleteKey(st, key);
  }

  ENSURE_SUCCESS(put(st, &st->root, key, val));
  return EXIT_SUCCESS;
}

static BSTVal get(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return get(st, node->left, key);
  } else if (cmp > 0) {
    return get(st, node->right, key);
  } else {
    return node->val;
  }
}

int BST_Get(BST st, BSTKey key, BSTVal *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  *out = get(st, st->root, key);
  return EXIT_SUCCESS;
}

static BSTNode deleteKey(BST st, BSTNode node, BSTKey key) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, NULL);

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    node->left = deleteKey(st, node->left, key);
  } else if (cmp > 0) {
    node->right = deleteKey(st, node->right, key);
  } else {
    if (IS_NULL(node->left)) {
      BSTNode right = node->right;
      free(node), (node = NULL);
      return right;
    }
    if (IS_NULL(node->right)) {
      BSTNode left = node->left;
      free(node), (node = NULL);
      return left;
    }

    BSTNode successor = min(node->right);
    node->right = deleteMin(node->right, false);
    successor->left = node->left;
    successor->right = node->right;
    free(node), (node = NULL);
    node = successor;
  }

  node->n = _nodeSize(node->left) + 1 + _nodeSize(node->right);
  return node;
}

int BST_DeleteKey(BST st, BSTKey key) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);
  st->root = deleteKey(st, st->root, key);
  return EXIT_SUCCESS;
}

int BST_Contains(BST st, BSTKey key, bool *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  BSTVal val = NULL;
  ENSURE_SUCCESS(BST_Get(st, key, &val));
  *out = IS_NOT_NULL(val);
  return EXIT_SUCCESS;
}

inline int BST_Size(BST st) { return _nodeSize(st->root); }
inline bool BST_IsEmpty(BST st) { return BST_Size(st) <= 0; }

static BSTNode min(BSTNode node) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);
  return IS_NULL(node->left) ? node : min(node->left);
}

int BST_Min(BST st, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode smallest = min(st->root);
  *out = _nodeKey(smallest);
  return EXIT_SUCCESS;
}

static BSTNode max(BSTNode node) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);
  return IS_NULL(node->right) ? node : max(node->right);
}

int BST_Max(BST st, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode largest = max(st->root);
  *out = _nodeKey(largest);
  return EXIT_SUCCESS;
}

static BSTNode floor(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return floor(st, node->left, key);
  } else if (cmp > 0) {
    BSTNode temp = floor(st, node->right, key);
    return IS_NOT_NULL(temp) ? temp : node;
  } else {
    return node;
  }
}

int BST_Floor(BST st, BSTKey key, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode floorNode = floor(st, st->root, key);
  *out = _nodeKey(floorNode);
  return EXIT_SUCCESS;
}

static BSTNode ceil(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    BSTNode temp = ceil(st, node->left, key);
    return IS_NOT_NULL(temp) ? temp : node;
  } else if (cmp > 0) {
    return ceil(st, node->right, key);
  } else {
    return node;
  }
}

int BST_Ceiling(BST st, BSTKey key, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode ceilNode = ceil(st, st->root, key);
  *out = _nodeKey(ceilNode);
  return EXIT_SUCCESS;
}

int rank(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return 0;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return rank(st, node->left, key);
  } else if (cmp > 0) {
    return _nodeSize(node->left) + 1 + rank(st, node->right, key);
  } else {
    return _nodeSize(node->left);
  }
}

int BST_Rank(BST st, BSTKey key, int *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  *out = rank(st, st->root, key);
  return EXIT_SUCCESS;
}

static BSTNode select(BSTNode node, int rank) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int t = _nodeSize(node->left);

  if (rank < t) {
    return select(node->left, rank);
  } else if (rank > t) {
    return select(node->right, rank - (t + 1));
  } else {
    return node;
  }
}

int BST_Select(BST st, int rank, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);
  REQUIRE_TRUE(rank >= 0, ERANGE, EXIT_FAILURE);
  REQUIRE_TRUE(rank < _nodeSize(st->root), ERANGE, EXIT_FAILURE);

  BSTNode node = select(st->root, rank);
  *out = _nodeKey(node);
  return EXIT_SUCCESS;
}

static BSTNode deleteMin(BSTNode node, const bool shouldFree) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);

  if (IS_NULL(node->left)) {
    BSTNode right = node->right;
    if (shouldFree) {
      free(node), (node = NULL);
    }
    return right;
  }

  node->left = deleteMin(node->left, shouldFree);
  node->n = _nodeSize(node->left) + 1 + _nodeSize(node->right);

  return node;
}

int BST_DeleteMin(BST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);
  st->root = deleteMin(st->root, true);
  return EXIT_SUCCESS;
}

static BSTNode deleteMax(BSTNode node, const bool shouldFree) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);

  if (IS_NULL(node->right)) {
    BSTNode left = node->left;
    if (shouldFree) {
      free(node), (node = NULL);
    }
    return left;
  }

  node->right = deleteMax(node->right, shouldFree);
  node->n = _nodeSize(node->left) + 1 + _nodeSize(node->right);

  return node;
}

int BST_DeleteMax(BST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  st->root = deleteMax(st->root, true);
  return EXIT_SUCCESS;
}

int BST_SizeOfRange(BST st, BSTKey low, BSTKey high, int *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(low), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(high), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(!_less(st, high, low), EINVAL, EXIT_FAILURE);

  const int rankLow = rank(st, st->root, low);
  const int rankHigh = rank(st, st->root, high);
  *out = rankHigh - rankLow;

  bool containsHigh = false;
  ENSURE_SUCCESS(BST_Contains(st, high, &containsHigh));

  if (containsHigh) {
    (*out)++;
  }
  return EXIT_SUCCESS;
}

static int keysInRange(BSTKeysIter iterator, BSTNode node, BSTKey low,
                       BSTKey high) {
  if (IS_NULL(node)) {
    return EXIT_SUCCESS;
  }

  const int cmpLow = iterator->bst->keyComparator(low, node->key);
  const int cmpHigh = iterator->bst->keyComparator(node->key, high);

  if (cmpLow < 0) {
    ENSURE_SUCCESS(keysInRange(iterator, node->left, low, high));
  }
  if (cmpLow <= 0 && cmpHigh <= 0) {
    ENSURE_SUCCESS(Queue_Enqueue(iterator->keys, node->key));
  }
  if (cmpHigh < 0) {
    ENSURE_SUCCESS(keysInRange(iterator, node->right, low, high));
  }
  return EXIT_SUCCESS;
}

static int BSTKeysIter_Init(BSTKeysIter iterator, BST st, BSTKey low,
                            BSTKey high) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(low), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(high), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(!_less(st, high, low), EINVAL, EXIT_FAILURE);

  iterator->bst = st;
  iterator->keys = Queue_Create();
  if (IS_NULL(iterator->keys)) {
    return EXIT_FAILURE;
  }

  const int code = keysInRange(iterator, st->root, low, high);
  if (code != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  iterator->keysIterator = QueueIterator_Create(iterator->keys);
  if (IS_NULL(iterator->keysIterator)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

BSTKeysIter BSTKeysIter_Create(BST st) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, NULL);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, NULL);
  BSTKey minKey = min(st->root)->key;
  BSTKey maxKey = max(st->root)->key;
  return BSTKeysIter_CreateInRange(st, minKey, maxKey);
}

BSTKeysIter BSTKeysIter_CreateInRange(BST st, BSTKey low, BSTKey high) {
  BSTKeysIter iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  const int code = BSTKeysIter_Init(iterator, st, low, high);
  if (code != EXIT_SUCCESS) {
    return NULL;
  }
  return iterator;
}

static int BSTKeysIter_Clear(BSTKeysIter iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(QueueIterator_Delete(&iterator->keysIterator));
  ENSURE_SUCCESS(Queue_Delete(&iterator->keys));
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int BSTKeysIter_Delete(BSTKeysIter *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);
  ENSURE_SUCCESS(BSTKeysIter_Clear(*iterator));
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

bool BSTKeysIter_HasNext(BSTKeysIter iterator) {
  return QueueIterator_HasNext(iterator->keysIterator);
}

int BSTKeysIter_GetNext(BSTKeysIter iterator, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(QueueIterator_GetNext(iterator->keysIterator, out));
  return EXIT_SUCCESS;
}

static inline int _nodeSize(BSTNode node) {
  return IS_NOT_NULL(node) ? node->n : 0;
}

static inline BSTKey _nodeKey(BSTNode node) {
  return IS_NOT_NULL(node) ? node->key : NULL;
}

static inline bool _less(BST st, BSTKey keyA, BSTKey keyB) {
  return st->keyComparator(keyA, keyB) < 0;
}
