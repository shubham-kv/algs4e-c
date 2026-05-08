#include <errno.h>
#include <stdlib.h>
#include "graph_search_bfs.h"
#include "queue_int.h"
#include "common_macros.h"

struct BreadthFirstSearch {
  Graph graph;
  int verticesCount;
  int sourceVertex;
  bool *marked;
  size_t count;
};

#define IS_VERTEX_IN_RANGE(n, v) (0 <= v && v < n)


static int _breadthFirstSearch(BFS bfs, const int s) {
  if (IS_NULL(bfs)) { errno = EINVAL; return -1; }
  if (!IS_VERTEX_IN_RANGE(bfs->verticesCount, s)) { errno = ERANGE; return -1; }

  struct IntegerQueue _queue, *queue = &_queue;
  IntQueue_Init(queue);

  IntQueue_Enqueue(queue, s);

  while (!IntQueue_IsEmpty(queue)) {
    IntQueueItem v;
    if (IntQueue_Dequeue(queue, &v) != 0) {
      return -1;
    }

    bfs->marked[v] = true;
    bfs->count++;

    AdjVertexIter adjacentVertexIterator = AdjVertexIter_Create(bfs->graph, v);
    if (IS_NULL(adjacentVertexIterator)) { IntQueue_Clear(queue); return -1; }

    while (AdjVertexIter_HasNext(adjacentVertexIterator)) {
      int w;
      if (AdjVertexIter_GetNext(adjacentVertexIterator, &w) == 0) {
        if (!bfs->marked[w]) {
          IntQueue_Enqueue(queue, w);
        }
      }
    }

    AdjVertexIter_Free(&adjacentVertexIterator);
  }

  IntQueue_Clear(queue);
  return 0;
}

BFS BFS_Create(Graph graph, const int s) {
  if (IS_NULL(graph)) { errno = EINVAL; return NULL; }

  const int verticesCount = Graph_Vertices(graph);
  if (!IS_VERTEX_IN_RANGE(verticesCount, s)) { errno = ERANGE; return NULL; }

  BFS bfs = calloc(1, sizeof(*bfs));
  if (IS_NULL(bfs)) { errno = ENOMEM; return NULL; }

  bool *marked = calloc(verticesCount, sizeof(*marked));
  if (IS_NULL(marked)) { errno = ENOMEM; free(bfs); return NULL; }

  bfs->graph = graph;
  bfs->verticesCount = verticesCount;
  bfs->sourceVertex = s;
  bfs->marked = marked;
  bfs->count = 0;

  if (_breadthFirstSearch(bfs, s) != 0) {
    free(bfs->marked), (bfs->marked = NULL);
    free(bfs), (bfs = NULL);
    return NULL;
  }

  return bfs;
}

void BFS_Free(BFS *bfs) {
  if (IS_NULL(bfs) || IS_NULL(*bfs)) { return; }

  if ((*bfs)->marked) {
    free((*bfs)->marked), ((*bfs)->marked = NULL);
  }

  free(*bfs), (*bfs = NULL);
}

inline bool BFS_Marked(BFS bfs, const int v) {
  return IS_VERTEX_IN_RANGE(bfs->verticesCount, v) ? bfs->marked[v] : false;
}

inline size_t BFS_Count(BFS bfs) {
  return bfs->count;
}

