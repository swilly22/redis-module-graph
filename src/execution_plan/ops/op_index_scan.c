#include "op_index_scan.h"

OpBase *NewIndexScanOp(Graph *g, Node **node, IndexBounds *index_data) {
  return (OpBase*)NewIndexScan(g, node, index_data);
}

IndexScan* NewIndexScan(Graph *g, Node **node, IndexBounds *index_data) {

  IndexScan *indexScan = malloc(sizeof(IndexScan));
  indexScan->node = node;
  indexScan->_node = *node;

  indexScan->index = index_data->index;
  SIValue *upper = NULL;
  // Only the upper bound must be cloned, as the lower bound is only accessed in the skiplistIterateRange call directly below
  if (index_data->upper) {
    upper = malloc(sizeof(SIValue));
    *upper = SI_Clone(*index_data->upper);
  }

  if (index_data->iter_type == T_STRING) {
    indexScan->iter = skiplistIterateRange(indexScan->index->string_sl, index_data->lower, upper, index_data->minExclusive, index_data->maxExclusive);
  } else if (index_data->iter_type & SI_NUMERIC) {
    indexScan->iter = skiplistIterateRange(indexScan->index->numeric_sl, index_data->lower, upper, index_data->minExclusive, index_data->maxExclusive);
  } else {
    assert(0);
  }

  // Set our Op operations
  indexScan->op.name = "Index Scan";
  indexScan->op.type = OpType_INDEX_SCAN;
  indexScan->op.consume = IndexScanConsume;
  indexScan->op.reset = IndexScanReset;
  indexScan->op.free = IndexScanFree;
  indexScan->op.modifies = NewVector(char*, 1);

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
  skiplistIterate_Reset(indexScan->iter);

  return OP_OK;
}

void IndexScanFree(OpBase *op) {
  IndexScan *indexScan = (IndexScan *)op;
  skiplistIterate_Free(indexScan->iter);
  free(indexScan);
}