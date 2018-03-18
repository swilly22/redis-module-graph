#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "../../src/util/skiplist.h"
#include "../../src/value.h"
#include "../../src/graph/node.h"

char *words[] = {"foo",  "bar",     "zap",    "pomo",
                 "pera", "arancio", "limone", NULL};

const char *node_label = "default_label";
char *prop_key = "default_prop_key";

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
    default:
      fprintf(stderr, "Unexpected value type encountered in compKey");
      exit(1);
  }
}

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

    double a_cast = compKey(a);
    double b_cast = compKey(b);
    if (a_cast > b_cast) {
      return 1;
    } else if (a_cast < b_cast) {
      return -1;
    } else {
      return 0;
    }
  }
}

void freeVal(void *p1) {
  // SIValue_Free breaks on stack-allocated variables
  // SIValue_Free(p1);
  free(p1);
}

skiplist* build_skiplist(void) {
  skiplist *sl = skiplistCreate(compareSI, NULL, compareNodes, freeVal);
  Node *cur_node;
  SIValue *cur_prop;

  for (long i = 0; words[i] != NULL; i ++) {
    cur_node = NewNode(10 + i, node_label);
    cur_prop = malloc(sizeof(SIValue));
    SIValue_FromString(cur_prop, words[i]);
    Node_Add_Properties(cur_node, 1, &prop_key, cur_prop);
    skiplistInsert(sl, cur_prop, cur_node);

    cur_node = NewNode(i, node_label);
    cur_prop = malloc(sizeof(SIValue));
    SIValue_FromString(cur_prop, words[6 - i]);
    Node_Add_Properties(cur_node, 1, &prop_key, cur_prop);
    skiplistInsert(sl, cur_prop, cur_node);
  }

  return sl;
}

// Update key-value pair
skiplistNode* update_skiplist(skiplist *sl, void *val, void *old_key, void *new_key) {
  // Ignore the return value from skiplistDelete
  skiplistDelete(sl, old_key, val);
  return skiplistInsert(sl, new_key, val);
}

void test_skiplist_range(void) {
  skiplist *sl = skiplistCreate(compareSI, NULL, compareNodes, freeVal);
  Node *cur_node;
  SIValue *cur_prop;

  char *keys[] = {"5.5", "0", "-30.2", "7", "1", "2", "-1.5", NULL};

  // The IDs we will assign to the Node values in the skiplist
  // (defined as the order the keys should be in after sorting)
  long ids[] = {5, 2, 0, 6, 3, 4, 1};

  for (long i = 0; keys[i] != NULL; i ++) {
    cur_node = NewNode(ids[i], node_label);
    cur_prop = malloc(sizeof(SIValue));
    SIValue_FromString(cur_prop, keys[i]);
    Node_Add_Properties(cur_node, 1, &prop_key, cur_prop);
    skiplistInsert(sl, cur_prop, cur_node);
  }

  long last_id = 3;
  skiplistIterator iter;
  // Iterate over a range of keys [1, INF)
  SIValue min = {.doubleval = 1, .type = T_DOUBLE};
  iter = skiplistIterateRange(sl, &min, NULL, 1, 0);
  while ((cur_node = skiplistIterator_Next(&iter)) != NULL) {
    assert(last_id + 1 == cur_node->id);
    last_id = cur_node->id;
  }

  skiplistFree(sl);
}

void test_skiplist_delete(void) {
  int delete_result;
  SIValue prop_to_delete;

  skiplist *sl = build_skiplist();

  SIValue_FromString(&prop_to_delete, words[2]);
  skiplistNode *old_skiplist_node = skiplistFind(sl, &prop_to_delete);
  Node *node_to_delete = old_skiplist_node->vals[0];
  SIValue_FromString(&prop_to_delete, words[3]);

  // Attempt to delete a non-existent key-value pair
  delete_result = skiplistDelete(sl, &prop_to_delete, node_to_delete);
  assert(delete_result == 0);

  // Attempt to delete a non-existent skiplist key
  SIValue_FromString(&prop_to_delete, "not_a_key");
  delete_result = skiplistDelete(sl, &prop_to_delete, NULL);
  assert(delete_result == 0);

  // Delete a single value from the skiplist
  delete_result = skiplistDelete(sl, Node_Get_Property(node_to_delete, prop_key), node_to_delete);
  assert(delete_result == 1);

  // Delete full nodes from the skiplist - a single key and all values that share it
  SIValue_FromString(&prop_to_delete, words[3]);
  delete_result = skiplistDelete(sl, &prop_to_delete, NULL);
  assert(delete_result == 1);

  // Verify that the skiplistNode has been deleted
  void *search_result = skiplistFind(sl, &prop_to_delete);
  assert(search_result == NULL);

  skiplistFree(sl);
}

void test_skiplist_update(void) {
  void *search_result;
  skiplist *sl = build_skiplist();
  SIValue find_prop;
  SIValue_FromString(&find_prop, words[2]);
  skiplistNode *old_skiplist_node = skiplistFind(sl, &find_prop);
  Node *node_to_update = old_skiplist_node->vals[0];

  SIValue *new_prop = malloc(sizeof(SIValue));
  SIValue old_prop = SI_Clone(*Node_Get_Property(node_to_update, prop_key));
  SIValue_FromString(new_prop, "updated_val");
  // Update an existing key-value pair in the index
  skiplistNode *new_skiplist_node = update_skiplist(sl, node_to_update, &old_prop, new_prop);
  // Update an existing key-value pair in the Node properties
  GraphEntity_Update_Property((GraphEntity *)node_to_update, prop_key, new_prop);

  // The new skiplistNode should have the new key
  assert(!strcmp(((SIValue *)new_skiplist_node->obj)->stringval, "updated_val"));

  // The old key-value pair must have been deleted
  search_result = skiplistFind(sl, &old_prop);
  void *ret;
  if (search_result) {
    ret = searchSkiplistNode(search_result, node_to_update, sl->valcmp);
    assert(ret == NULL);
  }

  // The new key-value pair can be found
  search_result = skiplistFind(sl, new_prop);
  ret = searchSkiplistNode(search_result, node_to_update, sl->valcmp);
  assert(ret != NULL);

  skiplistFree(sl);
}

/*
 * This test serves two purposes:
 * Test skiplist compatibility with Redis-Graph's specialized types,
 * and ensure that values sharing a single skiplist key are also sorted
 */
void test_skiplist_sort(void) {
  // Build a skiplist of Node * values with an internal SIValue property as a key,
  // and use the Node id for a secondary sort

  skiplist *sl = build_skiplist();
  Node *cur_node;
  SIValue *cur_prop;
  const char *node_label = "default_label";
  char *prop_key = "default_prop_key";

  skiplistIterator iter = skiplistIterateAll(sl);

  void *val;
  char *last_key = "", *cur_key;
  long last_id, cur_id;
  while ((val = skiplistIterator_Next(&iter)) != NULL) {
    cur_node = (Node *) val;
    cur_id = cur_node->id;
    cur_key = Node_Get_Property(cur_node, prop_key)->stringval;
    if (strcmp(last_key, cur_key)) {
      // We have moved on to a different key; reset the secondary sort comparison
      last_id = -1;
    }
    // Keys should be alphabetically sorted
    assert (strcmp(last_key, cur_key) <= 0);
    // Value IDs should be numerically sorted
    assert (last_id < cur_id);
    // Debug printing
    // printf("indexed property: \"%s\"\n",  cur_node->properties[0].value.stringval);
    // printf("node id: %ld\n", cur_node->id);
  }

  skiplistFree(sl);
}

int main(void) {
  test_skiplist_sort();
  test_skiplist_delete();
  test_skiplist_update();
  test_skiplist_range();

  printf("test_skiplist_graph - PASS!\n");
}
