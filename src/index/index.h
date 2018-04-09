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
} IndexSL;

// Routines that will be attached to the skiplist object
int compareNodes(const void *p1, const void *p2);
int compareSI(void *p1, void *p2, void *ctx);
void freeVal(void *p1);

IndexSL* createIndex(RedisModuleCtx *ctx, const char *graphName, AST_IndexOpNode *ast);
IndexSL* findIndex(const char *label, const char *property);

#endif
