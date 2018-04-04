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

void freeVal(void *p1) {
  free(p1);
}

skiplist* createIndex(RedisModuleCtx *ctx, const char *graphName, AST_IndexOpNode *indexOp) {
  const char *label = indexOp->target.label;
  const char *index_prop = indexOp->target.property;

  LabelStore *store = LabelStore_Get(ctx, STORE_NODE, graphName, label);

  LabelStoreIterator *it_test = LabelStore_Search(store, "");

  skiplist *sl = skiplistCreate(compareSI, NULL, compareNodes, freeVal);

  char *nodeId;
  tm_len_t nodeIdLen;
  Node *node;
  EntityProperty *prop;

  SIType lastKeyType = SI_NUMERIC | T_STRING;
  SIType xor;

  while(LabelStoreIterator_Next(it_test, &nodeId, &nodeIdLen, (void**)&node)) {
    for (int i = 0; i < node->prop_count; i ++) {
      prop = node->properties + i;

      if (!strcmp(index_prop, prop->name)) {
        SIValue *key = malloc(sizeof(SIValue));
        *key = SI_Clone(prop->value);

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

        skiplistInsert(sl, key, node);
        break;
      }
    }
  }

  /*
  skiplistIterator iter = skiplistIterateAll(sl);
  while ((node = skiplistIterator_Next(&iter)) != NULL) {
    printf("LABEL: %s\n", node->label);
    for (int i = 0; i < node->prop_count; i ++) {
      prop = node->properties + i;
      printf("PROP: %s\t:", prop->name);
      SIValue_Print(stdout, &prop->value);
      printf("\n");
    }
  }
  */

  return sl;
}

skiplist* findIndex(RedisModuleCtx *ctx, const char *graphName, IndexTarget *target) {
  // TODO Somewhere indices need to be stored, and can then be looked up by label-property pairs
  // IndexStore?

  return NULL;
}
