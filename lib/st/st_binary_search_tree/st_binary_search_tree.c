#include <errno.h>
#include <stdlib.h>

#include "common_macros.h"
#include "st_binary_search_tree.h"

#define ENSURE_SUCCESS(_callExpr)                                              \
  do {                                                                         \
    const int code = _callExpr;                                                \
    if (code != EXIT_SUCCESS) {                                                \
      return code;                                                             \
    }                                                                          \
  } while (0)

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

static int _nodeSize(BSTNode node);
static BSTKey _nodeKey(BSTNode node);

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

  BST_Init(st, keyComparator);
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
  BST_Clear(*st);
  free(*st), (*st = NULL);
  return EXIT_SUCCESS;
}

static int _put(BST st, BSTNode *out, BSTKey key, BSTVal val) {
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
    ENSURE_SUCCESS(_put(st, &node->left, key, val));
  } else if (cmp > 0) {
    ENSURE_SUCCESS(_put(st, &node->right, key, val));
  } else {
    node->val = val;
  }

  node->n = _nodeSize(node->left) + 1 + _nodeSize(node->right);

  return EXIT_SUCCESS;
}

int BST_Put(BST st, BSTKey key, BSTVal val) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);

  // if (IS_NULL(val)) {
  //   return BST_DeleteKey(st, key);
  // }

  ENSURE_SUCCESS(_put(st, &st->root, key, val));
  return EXIT_SUCCESS;
}

static BSTVal _get(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return _get(st, node->left, key);
  } else if (cmp > 0) {
    return _get(st, node->right, key);
  } else {
    return node->val;
  }
}

int BST_Get(BST st, BSTKey key, BSTVal *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  *out = _get(st, st->root, key);
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

static BSTNode _min(BSTNode node) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);
  return IS_NULL(node->left) ? node : _min(node->left);
}

int BST_Min(BST st, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode min = _min(st->root);
  *out = _nodeKey(min);
  return EXIT_SUCCESS;
}

static BSTNode _max(BSTNode node) {
  REQUIRE_TRUE(IS_NOT_NULL(node), EINVAL, NULL);
  return IS_NULL(node->right) ? node : _max(node->right);
}

int BST_Max(BST st, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode max = _max(st->root);
  *out = _nodeKey(max);
  return EXIT_SUCCESS;
}

static BSTNode _floor(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return _floor(st, node->left, key);
  } else if (cmp > 0) {
    BSTNode floor = _floor(st, node->right, key);
    return IS_NOT_NULL(floor) ? floor : node;
  } else {
    return node;
  }
}

int BST_Floor(BST st, BSTKey key, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode floorNode = _floor(st, st->root, key);
  *out = _nodeKey(floorNode);
  return EXIT_SUCCESS;
}

static BSTNode _ceil(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    BSTNode ceil = _ceil(st, node->left, key);
    return IS_NOT_NULL(ceil) ? ceil : node;
  } else if (cmp > 0) {
    return _ceil(st, node->right, key);
  } else {
    return node;
  }
}

int BST_Ceiling(BST st, BSTKey key, BSTKey *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(st->root), ENODATA, EXIT_FAILURE);

  BSTNode ceilNode = _ceil(st, st->root, key);
  *out = _nodeKey(ceilNode);
  return EXIT_SUCCESS;
}

int _rank(BST st, BSTNode node, BSTKey key) {
  if (IS_NULL(node)) {
    return 0;
  }

  const int cmp = st->keyComparator(key, node->key);

  if (cmp < 0) {
    return _rank(st, node->left, key);
  } else if (cmp > 0) {
    return _nodeSize(node->left) + 1 + _rank(st, node->right, key);
  } else {
    return _nodeSize(node->left);
  }
}

int BST_Rank(BST st, BSTKey key, int *out) {
  REQUIRE_TRUE(IS_NOT_NULL(st), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(key), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  *out = _rank(st, st->root, key);
  return EXIT_SUCCESS;
}

static BSTNode _select(BSTNode node, int rank) {
  if (IS_NULL(node)) {
    return NULL;
  }

  const int t = _nodeSize(node->left);

  if (t < rank) {
    return _select(node->right, rank - (t + 1));
  } else if (t > rank) {
    return _select(node->left, rank);
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

  BSTNode node = _select(st->root, rank);
  *out = _nodeKey(node);
  return EXIT_SUCCESS;
}

static inline int _nodeSize(BSTNode node) {
  return IS_NOT_NULL(node) ? node->n : 0;
}

static inline BSTKey _nodeKey(BSTNode node) {
  return IS_NOT_NULL(node) ? node->key : NULL;
}
