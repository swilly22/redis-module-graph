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
#include "../filter_tree/filter_tree.h"
#include "../parser/grammar.h" // required for the definition of filter operations (LT, GT, etc)

#define INDEX_PREFIX "redis_graph_INDEX"

typedef skiplistIterator IndexIterator;

typedef struct {
  IndexTarget target;
  skiplist *string_sl;
  skiplist *numeric_sl;
} Index;

typedef struct {
  Index *index;
  SIValue *lower;
  SIValue *upper;
  int minExclusive;
  int maxExclusive;
  SIType iter_type;
} IndexBounds;

void Index_Create(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp);
/* Select an Index and range based on filters associated with Node */
IndexBounds selectIndexFromFilters(RedisModuleCtx *ctx, const char *graphName, Vector *filters, const char *label);

#endif
