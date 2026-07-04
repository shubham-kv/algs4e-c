#include <stdint.h>

#include "hashers.h"

uint32_t Hash_Int(const void *key, const int tableSize) {
  uint32_t hash = 2166136261u;
  hash ^= *(int *)key;
  hash *= 16777619;
  return hash % tableSize;
}

uint32_t Hash_Str(const void *key, const int tableSize) {
  const char *str = (const char *)key;
  uint32_t hash = 2166136261u;

  for (; *str; str++) {
    hash ^= (unsigned char)*str;
    hash *= 16777619;
  }
  return hash % tableSize;
}

uint32_t Hash_Char(const void *key, const int tableSize) {
  uint32_t hash = 2166136261u;
  hash ^= *(char *)key;
  hash *= 16777619;
  return hash % tableSize;
}
