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
// skiplist object comparator
int compare(void *a, void *b, void *ctx) {
  // TODO I dunno what this is about, why do I need to compare the objects?
  return strcmp(a, b);
  // return 0;
}

// skiplist value comparator
int compareVals(void *p1, void *p2) {
  return compVal((SIValue *)p1) < compVal((SIValue *)p2);
}
*/
// skiplist value comparator
int compareValues(void *a, void *b) {
  // TODO vals, for us, are nodes - is there a comparator
  // for them, or even a need for them?
  return (a > b);
}

// skiplist key comparator
// TODO what is with the context here?
int compareKeys(void *p1, void *p2, void *ctx) {
  return compKey((SIValue *)p1) < compKey((SIValue *)p2);
}

OpBase* NewBuildIndexOp(RedisModuleCtx *ctx, Graph *g, const char *graph_name, const char *label, const char *property) {
  // Dunno if we actually need a LabelStore (this is from NewOpBuildIndex)
  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graph_name, label);
  if(store == NULL) {
    return NULL;
  }

  OpBuildIndex *op_build_index = malloc(sizeof(OpBuildIndex));

  op_build_index->ctx = ctx;
  op_build_index->store = store;
  op_build_index->graph = graph_name;
  op_build_index->iter = LabelStore_Search(store, "");

  // Set our Op operations
  op_build_index->op.name = "Build Index";
  op_build_index->op.type = OPType_BUILD_INDEX;
  op_build_index->op.consume = BuildIndexConsume;
  op_build_index->op.reset = BuildIndexReset;
  op_build_index->op.free = BuildIndexFree;
  op_build_index->op.modifies = NULL;
  
  op_build_index->label = label;
  op_build_index->indexed_property = property;
  // 
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
  // Can maybe add it to skip list here, but probably more efficient to gather all values first
  // (saves us from reallocing, and I assume that skip lists are like other sorted lists in terms of
  // cost)
  // Lots of stuff in here is redundant, but that makes it easy to read! Clean up later.
  EntityProperty *current_prop;
  SIValue *sort_value;
  double raw_property_val;
  for (int i = 0; i < node->prop_count; i ++) {
    current_prop = node->properties + i;
    // We're currently only supporting the indexing of numeric types, so let's verify that here.
    // Can we support multiple numeric types in one index?
    // Further, is it okay if the property name matches on a non-numeric value?
    if (current_prop->value.type | SI_NUMERIC && !strcmp(current_prop->name, op->indexed_property)) {
      // Index this node on this property
      // An SIValue is 24 bytes (although I feel like it should only be 16?)
      // We could maybe save space by using just the inner value as a key, but premature
      sort_value = &current_prop->value;  
      // SIValue_ToDouble(&to_index, &raw_property_val);
      skiplistInsert(op->index, (void*)sort_value, (void*)node);
      break;
    }
  }
  return OP_OK;
}

OpResult BuildIndexReset(OpBase *op) {
  OpBuildIndex *buildIndex = (OpBuildIndex*)op;

  /* *buildIndex->node = buildIndex->_node; */

  if(buildIndex->iter != NULL) {
    LabelStoreIterator_Free(buildIndex->iter);
  }

  buildIndex->iter = LabelStore_Search(buildIndex->store, "");
  return OP_OK;
}

void BuildIndexFree(OpBase *ctx) {
  OpBuildIndex *buildIndex = (OpBuildIndex *)ctx;
  if(buildIndex->iter != NULL) {
    LabelStoreIterator_Free(buildIndex->iter);
  }
  free(buildIndex);
}