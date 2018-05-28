#include "index.h"

int compareNodes(const void *a, const void *b) {
  return ((Node*)a)->id - ((Node*)b)->id;
}

int compareStrings(void *a, void *b, void *ctx) {
  return strcmp(((SIValue*)a)->stringval, ((SIValue*)b)->stringval);
}

// TODO this seems inefficient
int compareNumerics(void *p1, void *p2, void *ctx) {
  double a, b;
  SIValue_ToDouble(p1, &a);
  SIValue_ToDouble(p2, &b);
  return a - b;
}

/*
 * The index must maintain its own copy of the indexed SIValue
 * so that it becomes outdated but not broken by updates to the property.
 */
void cloneKey(void **property) {
  SIValue *redirect = *property;
  *redirect = SI_Clone(*redirect);
}

void freeKey(void *key) {
  SIValue_Free(key);
}

// Create and populate index for specified property
// (This function will create separate string and numeric indices if property has mixed types)
void indexProperty(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp) {
  const char *index_label = indexOp->target.label;
  const char *index_prop = indexOp->target.property;
  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graphName, index_label);

  LabelStoreIterator it;
  LabelStore_Scan(store, &it);

  char *nodeId;
  uint16_t nodeIdLen;
  Node *node;
  EntityProperty *prop;

  Index *index = createIndex(index_label, index_prop);

  Vector_Push(tmp_index_store, index);

  int prop_index = 0;
  while(LabelStoreIterator_Next(&it, &nodeId, &nodeIdLen, (void**)&node)) {
    // If the sought property is at a different offset than it occupied in the previous node,
    // then seek and update
    if (strcmp(index_prop, node->properties[prop_index].name)) {
      for (int i = 0; i < node->prop_count; i ++) {
        prop = node->properties + i;
        if (!strcmp(index_prop, prop->name)) {
          prop_index = i;
          break;
        }
      }
    }
    prop = node->properties + prop_index;
    // This value will be cloned within the skiplistInsert routine if necessary
    SIValue *key = &prop->value;

    if (key->type == T_STRING) {
      skiplistInsert(index->string_sl, key, node);
    } else if (key->type & SI_NUMERIC) {
      skiplistInsert(index->numeric_sl, key, node);
    } else { // This property was neither a string nor numeric value; raise a run-time error.
      assert(0);
    }
  }
}

Index* createIndex(const char *label, const char *property) {
  Index *index = malloc(sizeof(Index));
  index->target.label = strdup(label);
  index->target.property = strdup(property);

  index->iter = NULL;

  index->string_sl = skiplistCreate(compareStrings, NULL, compareNodes, cloneKey, freeKey);
  index->numeric_sl = skiplistCreate(compareNumerics, NULL, compareNodes, cloneKey, freeKey);

  return index;
}

Index* retrieveIndex(const char *label, const char *property) {
  // TODO Once indices are stored in the Redis keyspace, we can replace this loop
  // with a more efficient lookup
  Index *current_index = NULL;
  for (int i = 0; i < Vector_Size(tmp_index_store); i ++) {
    Vector_Get(tmp_index_store, i, &current_index);
    if (!strcmp(label, current_index->target.label) && !strcmp(property, current_index->target.property)) {
      return current_index;
    }
  }
  return NULL;
}


    for (int j = 0; j < Vector_Size(properties); j ++) {
      Vector_Get(properties, j, &cur_prop);

      // TODO We are not yet assessing which viable index would
      // be best to use.
      if (!strcmp(index_to_check->target.property, cur_prop)) {
        return index_to_check;
      }
    }
  }
  return NULL;
}
