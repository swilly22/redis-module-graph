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

typedef skiplistIterator IndexIterator;

/*
 * TODO Once index serialization code is finished, we will store indices in the keyspace -
 * for the moment, rely on this global vector */
Vector *tmp_index_store;

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

void indexProperty(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp);
Index* createIndex(const char *label, const char *property);
Index* retrieveIndex(const char *label, const char *property);
/* Select an Index and range based on filters associated with Node */
IndexBounds selectIndexFromFilters(Vector *filters, const char *label);

#endif
