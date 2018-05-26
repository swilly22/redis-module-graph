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

/*
 * TODO at time of index creation, graph does not exist.
 * Where to store indices? I lean towards external structure -
 * mock it up with a global vector for the moment
 */
Vector *tmp_index_store;

typedef struct {
  IndexTarget target;
  skiplist *sl;
  SIType value_type;
} Index;

Index* createIndex(const char *label, const char *property, SIType value_type);
void indexProperty(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp);
Index* findIndex(const char *label, Vector *properties);

#endif
