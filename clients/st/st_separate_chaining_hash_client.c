#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "comparators.h"
#include "hashers.h"
#include "st_separate_chaining_hash.h"

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build st_separate_chaining_hash_client
 *
 * ### Run
 * % ./build/bin/st_separate_chaining_hash_client
 *
 */
int main(int argc, char const *argv[]) {
  const char keys[] = "searchexample";
  const int n = strlen(keys);

  int values[n];
  FILE *out = stdout;

  fprintf(out, "** SeparateChainingHashST **\n");
  fprintf(out, "============================\n");
  fprintf(out, "\n");

  SCHST st = SCHST_Create(CMP_Char, Hash_Char, 97);
  if (IS_NULL(st)) {
    return EXIT_FAILURE;
  }

  for (int i = 0; i < n; i++) {
    values[i] = i;

    SCHSTKey key = &keys[i];
    SCHSTVal val = &values[i];
    bool contains = false;
    ENSURE_CALL_SUCCESS(SCHST_Contains(st, key, &contains));

    fprintf(out, "contains('%c') = %s\n", *(char *)key,
            BOOL_TO_STRING(contains));

    if (contains) {
      SCHSTVal oldVal;
      ENSURE_CALL_SUCCESS(SCHST_Get(st, key, &oldVal));
      fprintf(out, "get('%c') = %d\n", *(char *)key, *(int *)oldVal);
    }

    ENSURE_CALL_SUCCESS(SCHST_Put(st, key, val));
    fprintf(out, "put('%c', %d)\n", *(char *)key, *(int *)val);
    fprintf(out, "\n");
  }

  fprintf(out, "isEmpty() = %s\n", BOOL_TO_STRING(SCHST_IsEmpty(st)));
  fprintf(out, "size() = %d\n", SCHST_Size(st));

  SCHSTKeysIter iterator = SCHSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while(SCHSTKeysIter_HasNext(iterator)) {
    SCHSTKey key;
    SCHSTVal val;
    ENSURE_CALL_SUCCESS(SCHSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(SCHST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key, *(char *)key, *(int *)val);
  }
  fprintf(out, "\n");

  ENSURE_CALL_SUCCESS(SCHSTKeysIter_Delete(&iterator));

  bool contains = true;
  char ch = 'p';
  ENSURE_CALL_SUCCESS(SCHST_DeleteKey(st, &ch));
  fprintf(out, "delete('%c')\n", ch);
  ENSURE_CALL_SUCCESS(SCHST_Contains(st, &ch, &contains));
  fprintf(out, "contains('%c') = %s\n", ch, BOOL_TO_STRING(contains));

  contains = true;
  ch = 'l';
  ENSURE_CALL_SUCCESS(SCHST_DeleteKey(st, &ch));
  fprintf(out, "delete('%c')\n", ch);
  ENSURE_CALL_SUCCESS(SCHST_Contains(st, &ch, &contains));
  fprintf(out, "contains('%c') = %s\n", ch, BOOL_TO_STRING(contains));
  fprintf(out, "\n");

  fprintf(out, "isEmpty() = %s\n", BOOL_TO_STRING(SCHST_IsEmpty(st)));
  fprintf(out, "size() = %d\n", SCHST_Size(st));

  iterator = SCHSTKeysIter_Create(st);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  fprintf(out, "keys()\n");

  while(SCHSTKeysIter_HasNext(iterator)) {
    SCHSTKey key;
    SCHSTVal val;
    ENSURE_CALL_SUCCESS(SCHSTKeysIter_GetNext(iterator, &key));
    ENSURE_CALL_SUCCESS(SCHST_Get(st, key, &val));
    fprintf(out, "  next() = '%c', get('%c') = %2d\n", *(char *)key, *(char *)key, *(int *)val);
  }
  fprintf(out, "\n");

  ENSURE_CALL_SUCCESS(SCHSTKeysIter_Delete(&iterator));
  ENSURE_CALL_SUCCESS(SCHST_Delete(&st));

  return EXIT_SUCCESS;
}
