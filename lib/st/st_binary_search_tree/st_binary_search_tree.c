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
