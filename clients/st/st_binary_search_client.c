#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "comparators.h"
#include "st_binary_search.h"

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build st_binary_search_client
 *
 * ### Run
 * % ./build/bin/st_binary_search_client
 *
 */
int main(int argc, char const *argv[]) {
  const char keys[] = "searchexample";
  const int n = strlen(keys);

  int values[n];
  FILE *out = stdout;

  BSST st = BSST_Create(CMP_Char);
  if (IS_NULL(st)) {
    return EXIT_FAILURE;
  }

  for (int i = 0; i < n; i++) {
    values[i] = i;

    BSSTKey key = &keys[i];
    bool contains = false;

    ENSURE_CALL_SUCCESS(BSST_Contains(st, key, &contains));
    fprintf(out, "contains('%c') = %s\n", *(char *)key,
            contains ? "true" : "false");

    if (contains) {
      BSSTVal val;
      ENSURE_CALL_SUCCESS(BSST_Get(st, key, &val));
      fprintf(out, "get('%c') = %d\n", *(char *)key, *(int *)val);
    }

    BSSTVal val = &values[i];
    ENSURE_CALL_SUCCESS(BSST_Put(st, key, val));
    fprintf(out, "put('%c', %d)\n", *(char *)key, *(int *)val);
    fprintf(out, "\n");
  }

  fprintf(out, "isEmpty() = %s\n", BSST_IsEmpty(st) ? "true" : "false");
  fprintf(out, "size() = %d\n", BSST_Size(st));

  BSSTKeysIter iterator = BSSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while (BSSTKeysIter_HasNext(iterator)) {
    BSSTKey key;
    BSSTVal val;
    ENSURE_CALL_SUCCESS(BSSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BSST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  BSSTKey smallestKey;
  ENSURE_CALL_SUCCESS(BSST_Min(st, &smallestKey));
  fprintf(out, "min() = '%c'\n", *(char *)smallestKey);

  BSSTKey largestKey;
  ENSURE_CALL_SUCCESS(BSST_Max(st, &largestKey));
  fprintf(out, "max() = '%c'\n", *(char *)largestKey);

  BSSTKey returnedKey;
  char ch = 'n';
  ENSURE_CALL_SUCCESS(BSST_Floor(st, &ch, &returnedKey));
  fprintf(out, "floor('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 's';
  ENSURE_CALL_SUCCESS(BSST_Floor(st, &ch, &returnedKey));
  fprintf(out, "floor('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 'p';
  ENSURE_CALL_SUCCESS(BSST_Ceiling(st, &ch, &returnedKey));
  fprintf(out, "ceil('%c') = '%c'\n", ch, *(char *)returnedKey);

  ch = 't';
  ENSURE_CALL_SUCCESS(BSST_Ceiling(st, &ch, &returnedKey));
  fprintf(out, "ceil('%c') = '%c'\n", ch, *(char *)returnedKey);
  fprintf(out, "\n");

  int rank;
  ch = 'g';
  ENSURE_CALL_SUCCESS(BSST_Rank(st, &ch, &rank));
  fprintf(out, "rank('%c') = %d\n", ch, rank);

  ENSURE_CALL_SUCCESS(BSST_Select(st, rank, &returnedKey));
  fprintf(out, "select(%d) = '%c'\n", rank, *(char *)returnedKey);

  ch = 'p';
  ENSURE_CALL_SUCCESS(BSST_Rank(st, &ch, &rank));
  fprintf(out, "rank('%c') = %d\n", ch, rank);

  ENSURE_CALL_SUCCESS(BSST_Select(st, rank, &returnedKey));
  fprintf(out, "select(%d) = '%c'\n", rank, *(char *)returnedKey);
  fprintf(out, "\n");

  bool contains = false;
  ch = 'p';

  ENSURE_CALL_SUCCESS(BSST_DeleteKey(st, &ch));
  fprintf(out, "delete('%c')\n", ch);
  ENSURE_CALL_SUCCESS(BSST_Contains(st, &ch, &contains));
  fprintf(out, "contains('%c') = %s\n", ch, contains ? "true" : "false");

  ENSURE_CALL_SUCCESS(BSST_DeleteMin(st));
  fprintf(out, "deleteMin()\n");
  ENSURE_CALL_SUCCESS(BSST_Contains(st, smallestKey, &contains));
  fprintf(out, "contains('%c') = %s\n", *(char *)smallestKey,
          contains ? "true" : "false");

  ENSURE_CALL_SUCCESS(BSST_DeleteMax(st));
  fprintf(out, "deleteMax()\n");
  ENSURE_CALL_SUCCESS(BSST_Contains(st, largestKey, &contains));
  fprintf(out, "contains('%c') = %s\n", *(char *)largestKey,
          contains ? "true" : "false");
  fprintf(out, "\n");

  fprintf(out, "isEmpty() = %s\n", BSST_IsEmpty(st) ? "true" : "false");
  fprintf(out, "size() = %d\n", BSST_Size(st));

  iterator = BSSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while (BSSTKeysIter_HasNext(iterator)) {
    BSSTKey key;
    BSSTVal val;
    ENSURE_CALL_SUCCESS(BSSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BSST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  char low = 'd';
  char high = 'n';
  int size = 0;
  ENSURE_CALL_SUCCESS(BSST_SizeOfRange(st, &low, &high, &size));
  fprintf(out, "size('%c', '%c') = %d\n", low, high, size);

  iterator = BSSTKeysIter_CreateInRange(st, &low, &high);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys('%c', '%c')\n", low, high);

  while (BSSTKeysIter_HasNext(iterator)) {
    BSSTKey key;
    BSSTVal val;
    ENSURE_CALL_SUCCESS(BSSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(BSST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key,
            *(char *)key, *(int *)val);
  }
  ENSURE_CALL_SUCCESS(BSSTKeysIter_Delete(&iterator));
  fprintf(out, "\n");

  ENSURE_CALL_SUCCESS(BSST_Delete(&st));

  return EXIT_SUCCESS;
}
