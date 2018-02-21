#ifndef __OP_BUILD_INDEX_H__
#define __OP_BUILD_INDEX_H__

#include "op.h"
#include "../../redismodule.h"
#include "../../graph/graph.h"
#include "../../graph/node.h"
#include "../../stores/store.h"
#include "../../util/skiplist.h"


int compareValues(void *a, void *b);
int compareKeys(void *p1, void *p2, void *ctx);

typedef struct {
  OpBase op;
  LabelStore *store;      /* store being scanned */
  RedisModuleCtx *ctx;    /* redis module API context */
  const char *indexed_property;
  LabelStoreIterator *iter;
  // TODO this is, post-execution, a complete index of property->node, but is not getting 
  // but is not persisted alongside the graph yet
  skiplist *index;
} OpBuildIndex;

OpBase* NewBuildIndexOp(RedisModuleCtx *ctx, Graph *g, const char *graph_name, const char *label, const char *property);
OpResult BuildIndexConsume(OpBase *opBase, Graph* graph);
OpResult BuildIndexReset(OpBase *op);
void BuildIndexFree(OpBase *ctx);

#endif