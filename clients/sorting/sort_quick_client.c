#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "sort_quick.h"

int charCmp(const void *a, const void *b);
void printChars(char *str, size_t n);

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % ./build/bin/sort_quick_client
 *
 */
int main() {
  char str[] = "sortexample";
  size_t n = strlen(str);

  printf("** Quick Sort **\n");
  printf("================\n");

  printf("Before: ");
  printChars(str, n);
  printf("\n");

  ENSURE_CALL_SUCCESS(Sort_Quick(str, sizeof(char), n, charCmp));

  printf(" After: ");
  printChars(str, n);
  printf("\n");
  printf("\n");

  return EXIT_SUCCESS;
}

int charCmp(const void *a, const void *b) {
  char c1 = *(const char *)a;
  char c2 = *(const char *)b;
  return c1 < c2 ? -1 : c1 > c2 ? 1 : 0;
}

void printChars(char *str, size_t n) {
  printf("{ '%c'", str[0]);
  for (size_t i = 1; i < n; i++) {
    printf(", '%c'", str[i]);
  }
  printf(" }");
}
