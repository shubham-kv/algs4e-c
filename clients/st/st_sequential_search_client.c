#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "comparators.h"
#include "st_sequential_search.h"

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build st_sequential_search_client
 *
 * ### Run
 * % ./build/bin/st_sequential_search_client
 *
 */
int main(int argc, char const *argv[]) {
  const char keys[] = "searchexample";
  const int n = strlen(keys);

  int values[n];
  FILE *out = stdout;

  fprintf(out, "** SequentialSearchST **\n");
  fprintf(out, "========================\n");
  fprintf(out, "\n");

  SSST st = SSST_Create(CMP_Char);
  if (IS_NULL(st)) {
    return EXIT_FAILURE;
  }

  for (int i = 0; i < n; i++) {
    values[i] = i;

    SSSTKey key = &keys[i];
    SSSTVal val = &values[i];
    const bool contains = SSST_Contains(st, key);

    fprintf(out, "contains('%c') = %s\n", *(char *)key,
            BOOL_TO_STRING(contains));

    if (contains) {
      SSSTVal oldVal;
      ENSURE_CALL_SUCCESS(SSST_Get(st, key, &oldVal));

      fprintf(out, "get('%c') = %d\n", *(char *)key, *(int *)oldVal);
    }

    ENSURE_CALL_SUCCESS(SSST_Put(st, key, val));
    fprintf(out, "put('%c', %d)\n", *(char *)key, *(int *)val);
    fprintf(out, "\n");
  }

  fprintf(out, "isEmpty() = %s\n", BOOL_TO_STRING(SSST_IsEmpty(st)));
  fprintf(out, "size() = %d\n", SSST_Size(st));

  SSSTKeysIter iterator = SSSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while(SSSTKeysIter_HasNext(iterator)) {
    SSSTKey key;
    SSSTVal val;
    ENSURE_CALL_SUCCESS(SSSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(SSST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key, *(char *)key, *(int *)val);
  }
  fprintf(out, "\n");

  ENSURE_CALL_SUCCESS(SSSTKeysIter_Delete(&iterator));
  ENSURE_CALL_SUCCESS(SSST_Delete(&st));

  return EXIT_SUCCESS;
}
