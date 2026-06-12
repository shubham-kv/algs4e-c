#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "comparators.h"
#include "st_binary_search_tree.h"

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build st_binary_search_tree_client
 *
 * ### Run
 * % ./build/bin/st_binary_search_tree_client
 *
 */
int main(int argc, char const *argv[]) {
  const char keys[] = "searchexample";
  const int n = strlen(keys);

  int values[n];
  FILE *out = stdout;

  fprintf(out, "** BinarySearchTree **\n");
  fprintf(out, "======================\n");
  fprintf(out, "\n");

  BST st = BST_Create(CMP_Char);
  if (IS_NULL(st)) {
    return EXIT_FAILURE;
  }

  for (int i = 0; i < n; i++) {
    values[i] = i;

    const BSTKey key = &keys[i];
    const BSTVal val = &values[i];
    bool contains = false;

    ENSURE_CALL_SUCCESS(BST_Contains(st, key, &contains));
    fprintf(out, "contains('%c') = %s\n", *(char *)key,
            BOOL_TO_STRING(contains));

    if (contains) {
      BSTVal oldVal;
      ENSURE_CALL_SUCCESS(BST_Get(st, key, &oldVal));
      fprintf(out, "get('%c') = %d\n", *(char *)key, *(int *)oldVal);
    }

    ENSURE_CALL_SUCCESS(BST_Put(st, key, val));
    fprintf(out, "put('%c', %d)\n", *(char *)key, *(int *)val);
    fprintf(out, "\n");
  }

  fprintf(out, "isEmpty() = %s\n", BOOL_TO_STRING(BST_IsEmpty(st)));
  fprintf(out, "size() = %d\n", BST_Size(st));

  BSTKeysIter iterator = BSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while (BSTKeysIter_HasNext(iterator)) {
    BSTKey key;
    BSTVal val;
    ENSURE_CALL_SUCCESS(BSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  BSTKey smallestKey;
  ENSURE_CALL_SUCCESS(BST_Min(st, &smallestKey));
  fprintf(out, "min() = '%c'\n", *(char *)smallestKey);

  BSTKey largestKey;
  ENSURE_CALL_SUCCESS(BST_Max(st, &largestKey));
  fprintf(out, "max() = '%c'\n", *(char *)largestKey);

  BSTKey returnedKey;
  char ch = 'n';
  ENSURE_CALL_SUCCESS(BST_Floor(st, &ch, &returnedKey));
  fprintf(out, "floor('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 's';
  ENSURE_CALL_SUCCESS(BST_Floor(st, &ch, &returnedKey));
  fprintf(out, "floor('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 'p';
  ENSURE_CALL_SUCCESS(BST_Ceiling(st, &ch, &returnedKey));
  fprintf(out, "ceil('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 't';
  ENSURE_CALL_SUCCESS(BST_Ceiling(st, &ch, &returnedKey));
  fprintf(out, "ceil('%c') = '%c'\n", ch, *(char *)returnedKey);
  fprintf(out, "\n");

  int rank;
  ch = 'g';
  ENSURE_CALL_SUCCESS(BST_Rank(st, &ch, &rank));
  fprintf(out, "rank('%c') = %d\n", ch, rank);

  ENSURE_CALL_SUCCESS(BST_Select(st, rank, &returnedKey));
  fprintf(out, "select(%d) = '%c'\n", rank, *(char *)returnedKey);

  ch = 'p';
  ENSURE_CALL_SUCCESS(BST_Rank(st, &ch, &rank));
  fprintf(out, "rank('%c') = %d\n", ch, rank);

  ENSURE_CALL_SUCCESS(BST_Select(st, rank, &returnedKey));
  fprintf(out, "select(%d) = '%c'\n", rank, *(char *)returnedKey);
  fprintf(out, "\n");

  bool contains = false;
  ch = 'p';

  ENSURE_CALL_SUCCESS(BST_DeleteKey(st, &ch));
  fprintf(out, "delete('%c')\n", ch);
  ENSURE_CALL_SUCCESS(BST_Contains(st, &ch, &contains));
  fprintf(out, "contains('%c') = %s\n", ch, BOOL_TO_STRING(contains));

  ENSURE_CALL_SUCCESS(BST_DeleteMin(st));
  fprintf(out, "deleteMin()\n");
  ENSURE_CALL_SUCCESS(BST_Contains(st, smallestKey, &contains));
  fprintf(out, "contains('%c') = %s\n", *(char *)smallestKey,
          BOOL_TO_STRING(contains));

  ENSURE_CALL_SUCCESS(BST_DeleteMax(st));
  fprintf(out, "deleteMax()\n");
  ENSURE_CALL_SUCCESS(BST_Contains(st, largestKey, &contains));
  fprintf(out, "contains('%c') = %s\n", *(char *)largestKey,
          BOOL_TO_STRING(contains));
  fprintf(out, "\n");

  fprintf(out, "isEmpty() = %s\n", BOOL_TO_STRING(BST_IsEmpty(st)));
  fprintf(out, "size() = %d\n", BST_Size(st));

  iterator = BSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while (BSTKeysIter_HasNext(iterator)) {
    BSTKey key;
    BSTVal val;
    ENSURE_CALL_SUCCESS(BSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  char low = 'd';
  char high = 'n';
  int size = 0;
  ENSURE_CALL_SUCCESS(BST_SizeOfRange(st, &low, &high, &size));
  fprintf(out, "size('%c', '%c') = %d\n", low, high, size);

  iterator = BSTKeysIter_CreateInRange(st, &low, &high);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys('%c', '%c')\n", low, high);

  while (BSTKeysIter_HasNext(iterator)) {
    BSTKey key;
    BSTVal val;
    ENSURE_CALL_SUCCESS(BSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  ENSURE_CALL_SUCCESS(BST_Delete(&st));

  return EXIT_SUCCESS;
}
