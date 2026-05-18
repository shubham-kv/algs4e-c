#include "comparators.h"

int CMP_Char(const void *a, const void *b) {
  const char c1 = *(const char *)a;
  const char c2 = *(const char *)b;
  return c1 < c2 ? -1 : c1 > c2 ? 1 : 0;
}

int CMP_Int(const void *a, const void *b) {
  const int i = *(const int *)a;
  const int j = *(const int *)b;
  return (i < j) ? -1 : (i > j) ? 1: 0;
}
