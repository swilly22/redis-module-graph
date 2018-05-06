#include "index.h"

int compareNodes(const void *p1, const void *p2) {
  Node *a = (Node *)p1;
  Node *b = (Node *)p2;

  if (a->id > b->id) {
    return 1;
  } else if (a->id < b->id) {
    return -1;
  } else {
    return 0;
  }
}

int compareSI(void *p1, void *p2, void *ctx) {
  SIValue *a = p1, *b = p2;

  if (a->type & b->type & T_STRING) {
    return strcmp(a->stringval, b->stringval);
  } else if ((a->type & SI_NUMERIC) && (b->type & SI_NUMERIC)) {

    if (a->doubleval > b->doubleval) {
      return 1;
    } else if (a->doubleval < b->doubleval) {
      return -1;
    } else {
      return 0;
    }
  }

  // We can only compare string and numeric SIValues, so this point
  // should be unreachable for a properly constructed index.

  return 0;
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

  while(LabelStoreIterator_Next(&it, &nodeId, &nodeIdLen, (void**)&node)) {
    for (int i = 0; i < node->prop_count; i ++) {
      prop = node->properties + i;

      /*
       * TODO (INDEX_INEFFICIENCY):
       * Running strcmp in a loop over all properties is a wasteful
       * solution to matching index targets, we should make something more efficient.
       */

      if (!strcmp(index->target.property, prop->name)) {
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
        break;
      }
    }
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

Index* findIndex(const char *label, const char *property) {
  if (!tmp_index_store) return NULL;

  Index *index_to_check;
  for (int q = 0; q < Vector_Size(tmp_index_store); q ++) {
    Vector_Get(tmp_index_store, q, &index_to_check);
    if (!strcmp(index_to_check->target.label, label)) {
      return index_to_check;
    }
  }
  return NULL;
}
