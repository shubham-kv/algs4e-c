#include <errno.h>
#include <stdlib.h>
#include "graph_paths_bfp.h"
#include "common_macros.h"
#include "queue_int.h"

struct BreadthFirstPaths {
  Graph graph;
  int verticesCount;
  int sourceVertex;
  bool *marked;
  int *edgeTo;
};

#define IS_VERTEX_IN_RANGE(n, v) (0 <= v && v < n)


static int _breathFirstPaths(BFP bfp, const int s) {
  struct IntegerQueue _queue, *queue = &_queue;
  IntQueue_Init(queue);

  bfp->marked[s] = true;
  IntQueue_Enqueue(queue, s);

  while (!IntQueue_IsEmpty(queue)) {
    IntQueueItem v;
    if (IntQueue_Dequeue(queue, &v) != 0) {
      return -1;
    }

    AdjVertexIter iterator = AdjVertexIter_Create(bfp->graph, v);

    while (AdjVertexIter_HasNext(iterator)) {
      int w;
      if (AdjVertexIter_GetNext(iterator, &w) == 0) {
        if (!bfp->marked[w]) {
          bfp->marked[w] = true;
          bfp->edgeTo[w] = v;
          IntQueue_Enqueue(queue, w);
        }
      }
    }

    AdjVertexIter_Free(&iterator);
  }

  IntQueue_Clear(queue);
  return 0;
}

BFP BFP_Create(Graph graph, const int s) {
  if (IS_NULL(graph)) { errno = EINVAL; return NULL; }

  const int verticesCount = Graph_Vertices(graph);
  if (!IS_VERTEX_IN_RANGE(verticesCount, s)) { errno = ERANGE; return NULL; }

  BFP bfp = calloc(1, sizeof(*bfp));
  if (IS_NULL(bfp)) { errno = ENOMEM; return NULL; }

  bfp->graph = graph;
  bfp->verticesCount = verticesCount;
  bfp->sourceVertex = s;

  bfp->marked = calloc(bfp->verticesCount, sizeof(bool));
  if (IS_NULL(bfp->marked)) { errno = ENOMEM; return NULL; }

  bfp->edgeTo = calloc(bfp->verticesCount, sizeof(int));
  if (IS_NULL(bfp->edgeTo)) { errno = ENOMEM; return NULL; }

  for (int i = 0; i < bfp->verticesCount; i++) {
    bfp->edgeTo[i] = -1;
  }

  _breathFirstPaths(bfp, s);

  return bfp;
}

void BFP_Free(BFP *bfp) {
  if (IS_NULL(bfp) || IS_NULL(*bfp)) { return; }

  if ((*bfp)->marked) {
    free((*bfp)->marked), ((*bfp)->marked = NULL);
  }

  if ((*bfp)->edgeTo) {
    free((*bfp)->edgeTo), ((*bfp)->edgeTo = NULL);
  }

  free(*bfp), (*bfp = NULL);
}

inline bool BFP_HasPathTo(BFP bfp, const int v) {
  return IS_VERTEX_IN_RANGE(bfp->verticesCount, v) ? bfp->marked[v] : false;
}


struct BFPVertexIterator {
  int *verticesStack;
  int stackSize;
  int i;
};

#define ITER BFPVertexIter

ITER BFPVertexIter_Create(BFP bfp, const int v) {
  if (IS_NULL(bfp)) { errno = EINVAL; return NULL; }
  if (!IS_VERTEX_IN_RANGE(bfp->verticesCount, v)) { errno = ERANGE; return NULL; }
  if (!BFP_HasPathTo(bfp, v)) { errno = EINVAL; return NULL; }

  ITER iterator = calloc(1, sizeof(*iterator));
  if (IS_NULL(iterator)) { errno = ENOMEM; return NULL; }

  int stackSize = 0;
  int stackCapacity = 8;
  int *verticesStack = calloc(stackCapacity, sizeof(*verticesStack));
  if (IS_NULL(verticesStack)) { errno = ENOMEM; free(iterator); return NULL; }

  for (int w = v; w >= 0; w = bfp->edgeTo[w]) {
    // Resize array
    if (stackSize >= stackCapacity) {
      stackCapacity *= 2;
      verticesStack = reallocf(verticesStack, stackCapacity * sizeof(*verticesStack));

      if (IS_NULL(verticesStack)) { errno = ENOMEM; free(iterator); return NULL; }
    }

    verticesStack[stackSize++] = w;
  }

  iterator->verticesStack = verticesStack;
  iterator->stackSize = stackSize;
  iterator->i = iterator->stackSize - 1;

  return iterator;
}

void BFPVertexIter_Free(ITER *iterator) {
  if (IS_NULL(iterator) || IS_NULL(*iterator)) { return; }

  if ((*iterator)->verticesStack) {
    free((*iterator)->verticesStack), ((*iterator)->verticesStack = NULL);
  }

  free(*iterator), (*iterator = NULL);
}

inline bool BFPVertexIter_HasNext(ITER iterator) {
  return iterator->i >= 0;
}

inline int BFPVertexIter_GetNext(ITER iterator, int *out) {
  if (IS_NULL(iterator)) { errno = EINVAL; return -1; }
  if (!BFPVertexIter_HasNext(iterator)) { errno = ENODATA; return -1; }

  *out = iterator->verticesStack[iterator->i];
  iterator->i--;
  return 0;
}

#undef ITER

