#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "st_sequential_search.h"

#define BUFFER_LENGTH 31

static void _printHelp(FILE *stream, const char *programName);
static int _stringComparator(const void *a, const void *b);

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build frequency_counter
 *
 * ### Run
 * % echo "hello hello foo bar baz bar bar bin foo" > input.txt
 * % ./build/bin/frequency_counter 2 < input.txt
 *
 */
int main(int argc, char const *argv[]) {
  if (argc != 2) {
    _printHelp(stderr, argv[0]);
    return EXIT_FAILURE;
  }

  char *end;
  const int minWordLen = (int)strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    _printHelp(stderr, argv[0]);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_LENGTH];
  memset(buffer, 0, BUFFER_LENGTH);

  SSST wordCountST = SSST_Create(_stringComparator);
  RETURN_IF_NULL(wordCountST, EXIT_FAILURE);

  // Put the word (above a given threshold) and it's count into the ST
  while (fscanf(stdin, "%30s", buffer) != EOF) {
    const int wordLen = strlen(buffer);

    if (wordLen < minWordLen) {
      continue;
    }

    bool contains = false;
    ENSURE_CALL_SUCCESS(SSST_Contains(wordCountST, buffer, &contains));

    if (contains) {
      SSSTVal count;
      ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, buffer, &count));
      (*(int *)count)++;
      continue;
    }

    char *word = calloc(wordLen + 1, sizeof(*word));
    RETURN_IF_NULL(word, EXIT_FAILURE);

    int *count = calloc(1, sizeof(*count));
    RETURN_IF_NULL(count, EXIT_FAILURE);

    strncpy(word, buffer, wordLen);
    *count = 1;
    ENSURE_CALL_SUCCESS(SSST_Put(wordCountST, word, count));
  }

  const char *mostFrequentWord = NULL;
  int mostFrequentWordCount = 0;

  // Iterate through the keys to find the most frequent word
  SSSTKeysIter iterator = SSSTKeysIter_Create(wordCountST);
  RETURN_IF_NULL(iterator, EXIT_FAILURE);

  while (SSSTKeysIter_HasNext(iterator)) {
    SSSTKey word;
    SSSTVal wordCount;
    ENSURE_CALL_SUCCESS(SSSTKeysIter_GetNext(iterator, &word));
    ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, word, &wordCount));

    if (*(int *)wordCount > mostFrequentWordCount) {
      mostFrequentWord = (const char *)word;
      mostFrequentWordCount = *(int *)wordCount;
    }
  }
  ENSURE_CALL_SUCCESS(SSSTKeysIter_Delete(&iterator));

  // Print the most frequent word & it's count
  if (IS_NOT_NULL(mostFrequentWord)) {
    fprintf(stdout, "%s %d\n", mostFrequentWord, mostFrequentWordCount);
  }

  // Cleanup
  iterator = SSSTKeysIter_Create(wordCountST);
  RETURN_IF_NULL(iterator, EXIT_FAILURE);

  while (SSSTKeysIter_HasNext(iterator)) {
    SSSTKey word;
    SSSTVal wordCount;
    ENSURE_CALL_SUCCESS(SSSTKeysIter_GetNext(iterator, &word));
    ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, word, &wordCount));
    free((char *)word), (word = NULL);
    free((int *)wordCount), (wordCount = NULL);
  }
  ENSURE_CALL_SUCCESS(SSSTKeysIter_Delete(&iterator));
  ENSURE_CALL_SUCCESS(SSST_Delete(&wordCountST));

  return EXIT_SUCCESS;
}

static void _printHelp(FILE *stream, const char *programName) {
  char *thresholdLengthArgName = "threshold";

  fprintf(stream, "\n");
  fprintf(stream,
          "Finds the word no shorter than <%s> which occurs most frequently in "
          "a given text.\n",
          thresholdLengthArgName);
  fprintf(stream, "\n");
  fprintf(stream, "Usage: %s <%s>\n", programName, thresholdLengthArgName);
  fprintf(stream, "\n");
  fprintf(stream, "Arguments:\n");
  fprintf(stream, "%-15s %s\n", thresholdLengthArgName,
          "Integer specifying the minimum threshold length of word to count");
  fprintf(stream, "\n");
}

static int _stringComparator(const void *_a, const void *_b) {
  const char *a = (const char *)_a;
  const char *b = (const char *)_b;
  return strcmp(a, b);
}
