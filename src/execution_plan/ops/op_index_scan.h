#ifndef __OP_INDEX_SCAN_H
#define __OP_INDEX_SCAN_H

#include "op.h"
#include "../../redismodule.h"
#include "../../graph/graph.h"
#include "../../graph/node.h"
#include "../../stores/store.h"
#include "../../index/index.h"

typedef skiplistIterator IndexIterator;

typedef struct {
    OpBase op;
    Node **node;            /* node being scanned */
    Node *_node;
    Index *index;
    IndexIterator *iter;
} IndexScan;

/* Creates a new IndexScan operation */
OpBase *NewIndexScanOp(Graph *g, Node **node, Index *index);

IndexScan* NewIndexScan(Graph *g, Node **node, Index *index);

/* IndexScan next operation
 * called each time a new ID is required */
OpResult IndexScanConsume(OpBase *opBase, Graph* graph);

/* Restart iterator */
OpResult IndexScanReset(OpBase *ctx);

/* Frees IndexScan */
void IndexScanFree(OpBase *ctx);

#endif