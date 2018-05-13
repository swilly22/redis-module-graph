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

Index* populateIndex(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp) {
  Index *index = createIndex(indexOp->target.label, indexOp->target.property);

  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graphName, index->target.label);

  LabelStoreIterator it;
  LabelStore_Scan(store, &it);

  char *nodeId;
  uint16_t nodeIdLen;
  Node *node;
  EntityProperty *prop;

  SIType lastKeyType = SI_NUMERIC | T_STRING;
  SIType xor;

  int i;
  int prop_index = 0;
  while(LabelStoreIterator_Next(&it, &nodeId, &nodeIdLen, (void**)&node)) {
    // If the sought property is at a different offset than it occupied in the previous node,
    // then seek and update
    if (strcmp(index->target.property, node->properties[prop_index].name)) {
      for (i = 0; i < node->prop_count; i ++) {
        prop = node->properties + i;
        if (!strcmp(index->target.property, prop->name)) {
          prop_index = i;
          break;
        }
      }
    }
    prop = node->properties + prop_index;
    // This value will be cloned within the skiplistInsert routine if necessary
    SIValue *key = &prop->value;

    xor = key->type ^ lastKeyType;
    if (!xor) {
      // Exact match to last key type
    } else if (xor <= SI_NUMERIC) {
      // if ==, first comparison made and key type was string
      // if <, value was different non-numeric type or first comparison
      lastKeyType = key->type;
    } else {
      // TODO We will currently skip matching label-property pairs of incorrect property types
      continue;
    }

    skiplistInsert(index->sl, key, node);
  }

  return index;
}

Index* createIndex(const char *label, const char *property) {
  Index *index = malloc(sizeof(Index));
  index->target.label = strdup(label);
  index->target.property = strdup(property);

  index->sl = skiplistCreate(compareSI, NULL, compareNodes, cloneKey, freeKey);

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
