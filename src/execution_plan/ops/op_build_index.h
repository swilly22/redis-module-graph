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
// WIP
// This is just an AllNodeScan with a skip_list right now
typedef struct {
  OpBase op;
  // These are in an AllNodeScan, but I do not know if I need them
  // Node **node;            [> node being scanned <]
  // Node *_node;
  LabelStore *store;           /* store being scanned */
  RedisModuleCtx *ctx;    /* redis module API context */
  const char *graph; // graph name, used to access LabelStore
  LabelStoreIterator *iter; // what sets iter? I want to iterate only over nodes that match the label stored here (and am not doing that right now)
  const char *label; // Is this necessary? I dunno.
  const char *indexed_property;
  skiplist *index; // Probably something like this?
} OpBuildIndex;

OpBase* NewBuildIndexOp(RedisModuleCtx *ctx, Graph *g, const char *graph_name, const char *label, const char *property);
OpResult BuildIndexConsume(OpBase *opBase, Graph* graph);
OpResult BuildIndexReset(OpBase *op);
void BuildIndexFree(OpBase *ctx);

#endif