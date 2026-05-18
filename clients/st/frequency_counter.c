#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "st_sequential_search.h"
#include "common_macros.h"

#define BUFFER_LENGTH 64

void printHelp(FILE *stream, const char *programName) {
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

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printHelp(stderr, argv[0]);
    return EXIT_FAILURE;
  }

  char *end;
  const int threshold = (int) strtol(argv[1], &end, 10);
  if (end == argv[1]) {
    printHelp(stderr, argv[0]);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_LENGTH];
  memset(buffer, 0, BUFFER_LENGTH);

  SSST wordCountST = SSST_Create();
  REQUIRE_TRUE(IS_NOT_NULL(wordCountST), ENOMEM, EXIT_FAILURE);

  // Put the word (above a given threshold) and it's count into the ST
  while (fscanf(stdin, "%63s", buffer) != EOF) {
    const int wordLen = strlen(buffer);

    if (wordLen >= threshold) {
      char *word = calloc(wordLen + 1, sizeof(*word));
      REQUIRE_TRUE(IS_NOT_NULL(word), ENOMEM, EXIT_FAILURE);
      strncpy(word, buffer, wordLen);

      if (!SSST_Contains(wordCountST, word)) {
        int *count = calloc(1, sizeof(*count));
        REQUIRE_TRUE(IS_NOT_NULL(count), ENOMEM, EXIT_FAILURE);
        *count = 1;
        ENSURE_CALL_SUCCESS(SSST_Put(wordCountST, word, count));
      } else {
        SSSTVal countInST;
        ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, word, &countInST));
        int *count = (int *)countInST;
        (*count)++;
      }
    }
    memset(buffer, 0, BUFFER_LENGTH);
  }

  // Put an empty string with count 0 in the ST
  const char *mostFrequentWord = calloc(2, sizeof(*mostFrequentWord));
  REQUIRE_TRUE(IS_NOT_NULL(mostFrequentWord), ENOMEM, EXIT_FAILURE);

  int *mostFrequentWordCount = calloc(1, sizeof(*mostFrequentWordCount));
  REQUIRE_TRUE(IS_NOT_NULL(mostFrequentWordCount), ENOMEM, EXIT_FAILURE);
  *mostFrequentWordCount = 0;

  ENSURE_CALL_SUCCESS(SSST_Put(wordCountST, mostFrequentWord, mostFrequentWordCount));

  // Iterate through the keys to find the most frequent word
  SSSTKeysIter iterator = SSSTKeysIter_Create(wordCountST);
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, EXIT_FAILURE);

  while (SSSTKeysIter_HasNext(iterator)) {
    SSSTKey wordInST;
    ENSURE_CALL_SUCCESS(SSSTKeysIter_GetNext(iterator, &wordInST));
    const char *word = (const char *)wordInST;

    SSSTVal wordCountInST;
    ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, wordInST, &wordCountInST));
    const int *wordCount = (const int *)wordCountInST;

    ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, mostFrequentWord, &wordCountInST));
    const int *mostFrequentWordCount = (int *)wordCountInST;

    if (*wordCount > *mostFrequentWordCount) {
      mostFrequentWord = word;
    }
  }
  ENSURE_CALL_SUCCESS(SSSTKeysIter_Delete(&iterator));

  // Print the most frequent word & it's count
  SSSTVal wordCountInST;
  ENSURE_CALL_SUCCESS(SSST_Get(wordCountST, mostFrequentWord, &wordCountInST));
  mostFrequentWordCount = (int *) wordCountInST;
  fprintf(stdout, "%s %d\n", mostFrequentWord, *mostFrequentWordCount);

  // Cleanup
  iterator = SSSTKeysIter_Create(wordCountST);
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, EXIT_FAILURE);

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
