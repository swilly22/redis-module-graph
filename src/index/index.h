#ifndef __INDEX_H__
#define __INDEX_H__

#include "../redismodule.h"
#include "../graph/graph_entity.h"
#include "../stores/store.h"
#include "../util/triemap/triemap.h"
#include "../util/skiplist.h"
#include "../parser/ast.h"
#include "../graph/node.h"

/*
 * TODO at time of index creation, graph does not exist.
 * Where to store indices? I lean towards external structure -
 * mock it up with a global vector for the moment
 */
Vector *tmp_index_store;

// TODO Not a terribly helpful solution
typedef struct {
  IndexTarget target;
  skiplist *sl;
} Index;

Index* createIndex(const char *label, const char *property);
void populateIndex(RedisModuleCtx *ctx, Index *index, const char *graphName, AST_IndexOpNode *indexOp);
Index* findIndex(const char *label, const char *property);

#endif
