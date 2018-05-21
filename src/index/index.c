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

  // We maintain one index per property per type;
  // these will be created as necessary
  Index *numeric_index = NULL;
  Index *string_index = NULL;

  // In the most common case, the node currently being indexed will be placed in the same index as the
  // previous node, so we will try to optimize for that scenario
  SIType last_key_type = T_NULL;
  Index *current_index = NULL;

  int i;
  int prop_index = 0;
  while(LabelStoreIterator_Next(&it, &nodeId, &nodeIdLen, (void**)&node)) {
    // If the sought property is at a different offset than it occupied in the previous node,
    // then seek and update
    if (strcmp(index_prop, node->properties[prop_index].name)) {
      for (i = 0; i < node->prop_count; i ++) {
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

    if (key->type ^ last_key_type) {
      // The current property is of a different type than the last,
      // or we have not yet constructed an index
      last_key_type = key->type;
      if (key->type == T_STRING) {
        if (string_index == NULL) {
          // This is the first string property for this label; make a new index
          string_index = createIndex(index_label, index_prop, T_STRING);
          Vector_Push(tmp_index_store, string_index);
        }
        current_index = string_index;
      } else if (key->type & SI_NUMERIC) {
        if (numeric_index == NULL) {
          // This is the first numeric property for this label; make a new index
          numeric_index = createIndex(index_label, index_prop, SI_NUMERIC);
          Vector_Push(tmp_index_store, numeric_index);
        }
        current_index = numeric_index;
      } else {
        // This property was neither a string nor numeric value.
        // TODO I don't think that this scenario should be possible, but if we reach this
        // point we will currently just skip this node
        last_key_type = T_NULL;
        continue;
      }
    }

    skiplistInsert(current_index->sl, key, node);
  }
}

Index* createIndex(const char *label, const char *property, SIType value_type) {
  Index *index = malloc(sizeof(Index));
  index->target.label = strdup(label);
  index->target.property = strdup(property);
  index->value_type = value_type;

  if (value_type == T_STRING) {
    index->sl = skiplistCreate(compareStrings, NULL, compareNodes, cloneKey, freeKey);
  } else {
    index->sl = skiplistCreate(compareNumerics, NULL, compareNodes, cloneKey, freeKey);
  }

  return index;
}

Index* findIndex(const char *label, Vector *properties) {
  if (!tmp_index_store) return NULL;

  Index *index_to_check;
  const char *cur_prop;
  for (int i = 0; i < Vector_Size(tmp_index_store); i ++) {
    Vector_Get(tmp_index_store, i, &index_to_check);
    if (strcmp(index_to_check->target.label, label)) continue;

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
