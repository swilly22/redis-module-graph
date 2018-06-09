#include "index.h"
#include "index_type.h"

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

RedisModuleKey* Index_LookupKey(RedisModuleCtx *ctx, const char *graph, const char *label, const char *property) {
  char *strKey;
  asprintf(&strKey, "%s_%s_%s_%s", INDEX_PREFIX, graph, label, property);

  RedisModuleString *rmIndexId = RedisModule_CreateString(ctx, strKey, strlen(strKey));
  free(strKey);

  RedisModuleKey *key = RedisModule_OpenKey(ctx, rmIndexId, REDISMODULE_WRITE);
  RedisModule_FreeString(ctx, rmIndexId);

  return key;
}

Index* Index_Get(RedisModuleCtx *ctx, const char *graph, const char *label, const char *property) {
  RedisModuleKey *key = Index_LookupKey(ctx, graph, label, property);
  Index *idx = NULL;

  if (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) {
    idx = RedisModule_ModuleTypeGetValue(key);
  }
  RedisModule_CloseKey(key);

  return idx;
}

// Create and populate index for specified property
// (This function will create separate string and numeric indices if property has mixed types)
void Index_Create(RedisModuleCtx *ctx, const char *graphName, AST_IndexNode *indexOp) {
  const char *label = indexOp->target.label;
  const char *prop_str = indexOp->target.property;
  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graphName, label);

  LabelStoreIterator it;
  LabelStore_Scan(store, &it);

  char *nodeId;
  uint16_t nodeIdLen;
  Node *node;
  EntityProperty *prop;

  RedisModuleKey *key = Index_LookupKey(ctx, graphName, label, prop_str);
  // Do nothing if this index already exists
  if (RedisModule_KeyType(key) != REDISMODULE_KEYTYPE_EMPTY) {
    RedisModule_CloseKey(key);
    return;
  }

  Index *index = malloc(sizeof(Index));
  RedisModule_ModuleTypeSetValue(key, IndexRedisModuleType, index);
  RedisModule_CloseKey(key);

  index->target.label = strdup(label);
  index->target.property = strdup(prop_str);

  index->string_sl = skiplistCreate(compareStrings, NULL, compareNodes, cloneKey, freeKey);
  index->numeric_sl = skiplistCreate(compareNumerics, NULL, compareNodes, cloneKey, freeKey);

  int prop_index = 0;
  while(LabelStoreIterator_Next(&it, &nodeId, &nodeIdLen, (void**)&node)) {
    // If the sought property is at a different offset than it occupied in the previous node,
    // then seek and update
    if (strcmp(prop_str, node->properties[prop_index].name)) {
      for (int i = 0; i < node->prop_count; i ++) {
        prop = node->properties + i;
        if (!strcmp(prop_str, prop->name)) {
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

/*
 * Before the ExecutionPlan has been constructed, we can analyze the FilterTree
 * to see if a scan operation can employ an index. This function will return a
 * struct containing an appropriate index (if any) and the boundaries for setting its iterator.
 */
IndexBounds selectIndexFromFilters(RedisModuleCtx *ctx, const char *graphName, Vector *filters, const char *label) {
  IndexBounds bounds;
  bounds.lower = bounds.upper = NULL;
  bounds.index = NULL;

  Index *chosen_index = NULL;
  FT_PredicateNode *current;
  while (Vector_Size(filters) > 0) {
    Vector_Pop(filters, &current);
    if (!chosen_index) {
      // Look this property up to see if it has been indexed (using the label rather than the node alias)
      chosen_index = Index_Get(ctx, graphName, label, current->Lop.property);
      if (chosen_index == NULL) continue;

      // TODO Currently, we will default to using the first valid index we find -
      // in the future, it would be worthwhile to compare all viable indices and
      // attempt to choose the most efficient one
      bounds.index = chosen_index;
      bounds.iter_type = current->constVal.type;
    } else if (strcmp(chosen_index->target.property, current->Lop.property)) {
      // This filter's property does not match our index
      continue;
    }

    /*
     * TODO This block assumes that multiple filters specified on the same alias-property pair
     * can be validly compared with either filter's comparator function.
     * Currently, this is not a safe assumption here or for the FilterTree as a whole
     * (a query like "MATCH (p:person) WHERE p.age < 50 AND p.age != "string"" is unhandled)
     */
    switch(current->op) {
      case EQ:
        bounds.lower = &current->constVal;
        bounds.upper = &current->constVal;
        bounds.minExclusive = bounds.maxExclusive = 0;
        // This is the best possible bound; no need to look further
        return bounds;

      case GT | GE:
        if (bounds.lower && (current->cf(bounds.lower, &current->constVal) > 0)) {
          break; // Don't set new bound unless it narrows the range
        }
        bounds.lower = &current->constVal;
        bounds.minExclusive = current->op == GT ? 1 : 0;
        break;

      case LT | LE:
        if (bounds.upper && (current->cf(bounds.upper, &current->constVal) < 0)) {
          break; // Don't set new bound unless it narrows the range
        }
        bounds.upper = &current->constVal;
        bounds.maxExclusive = current->op == LT ? 1 : 0;
        break;
    }
  }

  return bounds;
}
