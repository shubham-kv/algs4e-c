#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "graph_connected_components.h"
#include "queue_int.h"
#include "common_macros.h"

struct Args {
  const char *graphFileName;
};

static void _printUsage(FILE *file, const char *binaryName);

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build graph_connected_components_client
 *
 * ### Run
 * % ./build/bin/graph_connected_components_client tinyG.txt
 * 
 */
int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    _printUsage(stderr, argv[0]);
    return -1;
  }

  struct Args args = {.graphFileName = argv[1]};

  FILE *graphFileInput = fopen(args.graphFileName, "r");
  if (IS_NULL(graphFileInput)) {
    fprintf(stderr, "Error opening file '%s': %s\n", args.graphFileName, strerror(errno));
    fprintf(stderr, "\n");
    return -1;
  }

  Graph graph = Graph_CreateFromFile(graphFileInput);
  fclose(graphFileInput), (graphFileInput = NULL);

  if (IS_NULL(graph)) {
    fprintf(stderr, "Error creating graph: %s\n", strerror(errno));
    fprintf(stderr, "\n");
    return -1;
  }

  GraphCC graphCC = GraphCC_Create(graph);

  if (IS_NULL(graphCC)) {
    fprintf(stderr, "Error processing the graph: %s\n", strerror(errno));
    fprintf(stderr, "\n");
    Graph_Free(&graph);
    return -1;
  }

  const int n = GraphCC_Count(graphCC);
  IntQueue queues[n];

  for (int i = 0; i < n; i++) {
    IntQueue queue = IntQueue_Create();
    if (IS_NULL(queue)) {
      fprintf(stderr, "Error during processing: %s\n", strerror(errno));
      fprintf(stderr, "\n");
      GraphCC_Free(&graphCC);
      Graph_Free(&graph);
      return -1;
    }

    queues[i] = queue;
  }

  printf("** Connected components **\n");
  printf("\n");
  printf("Total %d connected components\n", n);

  const int vertices = Graph_Vertices(graph);

  for (int v = 0; v < vertices; v++) {
    const int componentId = GraphCC_Id(graphCC, v);
    assert(0 <= componentId && componentId < n);

    const int code = IntQueue_Enqueue(queues[componentId], v);
    assert(code == 0);
  }

  for (int i = 0; i < n; i++) {
    IntQueueIterator iterator = IntQueueIter_Create(queues[i]);
    assert(iterator != NULL);

    while (IntQueueIter_HasNext(iterator)) {
      IntQueueItem v;
      if (IntQueueIter_GetNext(iterator, &v) != 0) {
        return -1;
      }

      assert(v >= 0);
      printf("%lld ", v);
    }
    printf("\n");

    IntQueueIter_Delete(&iterator);
  }
  printf("\n");

  for (int i = 0; i < n; i++) {
    IntQueue_Delete(&queues[i]);
  }

  GraphCC_Free(&graphCC);
  Graph_Free(&graph);

  return 0;
}

static void _printUsage(FILE *file, const char *binaryName) {
  fprintf(file, "Usage: %s <fileName>\n", binaryName);
  fprintf(
      file,
      "Create a graph from the given graph input file name <fileName> and "
      "processes the graph for finding all connected components in the graph.\n");
  fprintf(file, "\n");
  fprintf(file, "%-10s %s\n", "file", "File name used as input to build the graph");
  fprintf(file, "\n");
}

