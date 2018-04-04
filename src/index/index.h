#ifndef __INDEX_H__
#define __INDEX_H__

#include "../redismodule.h"
#include "../graph/graph_entity.h"
#include "../stores/store.h"
#include "../util/triemap/triemap.h"
#include "../util/skiplist.h"
#include "../parser/ast.h"
#include "../graph/node.h"

// Routines that will be attached to the skiplist object
int compareNodes(const void *p1, const void *p2);
int compareSI(void *p1, void *p2, void *ctx);
void freeVal(void *p1);

skiplist* createIndex(RedisModuleCtx *ctx, const char *graphName, AST_QueryExpressionNode *ast);

#endif
