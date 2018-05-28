#ifndef __INDEX_H__
#define __INDEX_H__

#include <assert.h>
#include "../redismodule.h"
#include "../graph/graph_entity.h"
#include "../stores/store.h"
#include "../util/skiplist.h"
#include "../parser/ast.h"
#include "../graph/node.h"
#include "../dep/rax/rax.h"

typedef skiplistIterator IndexIterator;

/*
 * TODO Once index serialization code is finished, we will store indices in the keyspace -
 * for the moment, rely on this global vector */
Vector *tmp_index_store;

typedef struct {
  IndexTarget target;
  skiplist *string_sl;
  skiplist *numeric_sl;
  IndexIterator *iter;
} Index;

void indexProperty(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp);
Index* findIndex(const char *label, Vector *properties);
Index* createIndex(const char *label, const char *property);

#endif
