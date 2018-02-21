#include "op_build_index.h"

// Efficient? I don't know - think about it!
static inline double compKey(SIValue *v) {
  switch (v->type) {
    case T_DOUBLE:
      return v->doubleval;
    case T_INT64: // do nothing!
      return (double)v->longval;
    case T_INT32:
      return (double)v->intval;
    case T_UINT:
      return (double)v->uintval;
    case T_FLOAT:
      return (double)v->floatval;
  }
}

/*
 * skiplist value comparator:
 * This is solely to ensure that
 * we do not insert duplicates.
 */
int compareValues(void *a, void *b) {
  return a != b;
}

/*
 * skiplist key comparator:
 * We use strcmp-style returns for comparing keys.
 * `ctx` is included to match the skiplist source,
 * but currently has no purpose.
 */
int compareKeys(void *p1, void *p2, void *ctx) {
  double a = compKey((SIValue *)p1);
  double b = compKey((SIValue *)p2);

  if (a > b) {
    return 1;
  } else if (a < b) {
    return -1;
  } else {
    return 0;
  }
}

OpBase* NewBuildIndexOp(RedisModuleCtx *ctx, Graph *g, const char *graph_name, const char *label, const char *property) {
  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graph_name, label);
  if(store == NULL) {
    return NULL;
  }

  OpBuildIndex *op_build_index = malloc(sizeof(OpBuildIndex));

  op_build_index->ctx = ctx;
  op_build_index->store = store;
  op_build_index->iter = LabelStore_Search(store, "");

  // Set our Op operations
  op_build_index->op.name = "Build Index";
  op_build_index->op.type = OPType_BUILD_INDEX;
  op_build_index->op.consume = BuildIndexConsume;
  op_build_index->op.reset = BuildIndexReset;
  op_build_index->op.free = BuildIndexFree;
  op_build_index->op.modifies = NULL;

  op_build_index->indexed_property = property;
  op_build_index->index = skiplistCreate(compareKeys, NULL, compareValues);

  return (OpBase*)op_build_index;
}

OpResult BuildIndexConsume(OpBase *opBase, Graph* graph) {
  OpBuildIndex *op = (OpBuildIndex*)opBase;

  if(op->iter == NULL) {
    return OP_DEPLETED;
  }

  char *id;
  tm_len_t idLen;
  Node *node;
  int res = LabelStoreIterator_Next(op->iter, &id, &idLen, (void**)&node);

  if(res == 0) {
    return OP_DEPLETED;
  }

  // Retrieve a node by label, and find the property we want to index on
  // Verify that property is numeric (does it also need to always be of same numeric type?)
  EntityProperty *current_prop;
  SIValue *sort_value;
  for (int i = 0; i < node->prop_count; i ++) {
    current_prop = node->properties + i;
    // If this property matches our new index and is any supported numeric,
    // let's index this node (we're validing numericity with a bitmask)
    if ((current_prop->value.type | SI_NUMERIC) && !strcmp(current_prop->name, op->indexed_property)) {
      sort_value = &current_prop->value;
      skiplistInsert(op->index, (void*)sort_value, (void*)node);
      break;
    }
  }
  return OP_OK;
}

// As with ProduceResults, not important for this op currently
OpResult BuildIndexReset(OpBase *op) {
  return OP_OK;
}

void BuildIndexFree(OpBase *ctx) {
  OpBuildIndex *buildIndex = (OpBuildIndex *)ctx;
  if(buildIndex->iter != NULL) {
    LabelStoreIterator_Free(buildIndex->iter);
  }
  free(buildIndex);
}