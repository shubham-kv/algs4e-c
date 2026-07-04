#include <stdint.h>

#ifndef __hashers_h
#define __hashers_h

typedef uint32_t (*HashFn)(const void *key, const int tableSize);

uint32_t Hash_Int(const void *key, const int tableSize);
uint32_t Hash_Str(const void *key, const int tableSize);
uint32_t Hash_Char(const void *key, const int tableSize);

#endif // __hashers_h
