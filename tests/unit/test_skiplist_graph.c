#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "../../src/util/skiplist.h"
#include "../../src/value.h"
#include "../../src/graph/node.h"

char *words[] = {"foo",  "bar",     "zap",    "pomo",
                 "pera", "arancio", "limone", NULL};

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

int compareNodes(void *p1, void *p2) {
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

/*
 * This test serves two purposes:
 * Test skiplist compatibility with Redis-Graph's specialized types,
 * and ensure that values sharing a single skiplist key are also sorted
 */
void test_skiplist_graph(void) {
  // Build a skiplist of Node * values with an internal SIValue property as a key,
  // and use the Node id for a secondary sort

  skiplist *node_sl = skiplistCreate(compareSI, NULL, compareNodes);
  Node *cur_node;
  SIValue *cur_prop;
  const char *node_label = "default_label";
  char *prop_key = "default_prop_key";

  for (long i = 0; words[i] != NULL; i ++) {
    // Add nodes with greater IDs
    cur_node = NewNode(10 + i, node_label);
    cur_prop = malloc(sizeof(SIValue));
    cur_prop->stringval = strdup(words[i]);
    cur_prop->type = T_STRING;
    Node_Add_Properties(cur_node, 1, &prop_key, cur_prop);
    skiplistInsert(node_sl, cur_prop, cur_node);

    // Add nodes with lower IDs and different property keys
    cur_node = NewNode(i, node_label);
    cur_prop = malloc(sizeof(SIValue));
    cur_prop->stringval = strdup(words[6 - i]);
    cur_prop->type = T_STRING;
    Node_Add_Properties(cur_node, 1, &prop_key, cur_prop);
    skiplistInsert(node_sl, cur_prop, cur_node);
  }

  skiplistIterator iter = skiplistIterateAll(node_sl);

  void *val;
  char *last_key = "", *cur_key;
  long last_id, cur_id;
  while ((val = skiplistIterator_Next(&iter)) != NULL) {
    cur_node = (Node *) val;
    cur_id = cur_node->id;
    cur_key = cur_node->properties[0].value.stringval;
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

  printf("test_skiplist_graph - PASS!\n");
}

int main(void) {
  test_skiplist_graph();
}