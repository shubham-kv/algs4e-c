#ifndef __comparators_h
#define __comparators_h

typedef int (*ComparatorFn)(const void *a, const void *b);

int CMP_Char(const void *a, const void *b);
int CMP_Int(const void *a, const void *b);

#endif // __comparators_h
