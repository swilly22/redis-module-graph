#include "op_index_scan.h"

OpBase *NewIndexScanOp(RedisModuleCtx *ctx, Graph *g, Node **node,
    const char *graph_name, char *label, skiplist *index_sl) {
  return (OpBase*)NewIndexScan(ctx, g, node, graph_name, label, index_sl);
}

IndexScan* NewIndexScan(RedisModuleCtx *ctx, Graph *g, Node **node,
    const char *graph_name, char *label, skiplist *index_sl) {

  IndexScan *indexScan = malloc(sizeof(IndexScan));
  indexScan->ctx = ctx;
  indexScan->node = node;

  indexScan->sl = index_sl;
  skiplistIterator *iter = malloc(sizeof(skiplistIterator));
  *iter = skiplistIterateAll(indexScan->sl);
  indexScan->iter = iter;

  // Set our Op operations
  indexScan->op.name = "Index Scan";
  indexScan->op.type = OpType_INDEX_SCAN;
  indexScan->op.consume = IndexScanConsume;
  indexScan->op.reset = IndexScanReset;
  indexScan->op.free = IndexScanFree;
  indexScan->op.modifies = NewVector(char*, 1); // TODO will never modify?

  Vector_Push(indexScan->op.modifies, Graph_GetNodeAlias(g, *node));

  return indexScan;
}

OpResult IndexScanConsume(OpBase *opBase, Graph* graph) {
  IndexScan *op = (IndexScan*)opBase;

  if(op->iter == NULL) {
    return OP_DEPLETED;
  }

  /* Update node */
  *op->node = skiplistIterator_Next(op->iter);

  if(*op->node == NULL) {
    return OP_DEPLETED;
  }

  return OP_OK;
}

OpResult IndexScanReset(OpBase *ctx) {
  IndexScan *indexScan = (IndexScan*)ctx;

  /* Restore original node. */
  *indexScan->node = indexScan->_node;
  // TODO hm
  *indexScan->iter = skiplistIterateAll(indexScan->sl);

  return OP_OK;
}

void IndexScanFree(OpBase *op) {
  IndexScan *indexScan = (IndexScan *)op;
  free(indexScan->iter);
  free(indexScan);
}