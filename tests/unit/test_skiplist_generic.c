#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "../../src/util/skiplist.h"
#include "../../src/value.h"
#include "../../src/graph/node.h"


// Key comparator function to construct skiplist with
int compStrKeys(void *a, void *b, void *ctx) { 
  return strcmp(a, b);
}

// Value comparator function to construct skiplist with
int compareVals(void *p1, void *p2) {
  int a = *(int *)p1;
  int b = *(int *)p2;

  if (a > b) {
    return 1;
  } else if (a < b) {
    return -1;
  } else {
    return 0;
  }
}

// Verify lexicographic sorting of string keys
void test_string_sorts(void) {

  char *words[] = {"foo",  "bar",     "zap",    "pomo",
                   "pera", "arancio", "limone", NULL};
  
  char *vals[] = {"foo val", "bar val", "zap val", "pomo val",
                  "pera val", "arancio val", "limone val", NULL};

  skiplist *string_sl = skiplistCreate(compStrKeys, NULL, compareVals);

  for (int i = 0; words[i] != NULL; i ++) {
    skiplistInsert(string_sl, words[i], vals[i]);
  }

  char *val, *prevVal = "";
  int str_ret;
  // LHS value should always return negative
  while ((val = skiplistPopHead(string_sl)) != NULL) {
    str_ret = strcmp(prevVal, val);
    assert(str_ret < 0);
  }

  skiplistFree(string_sl);
}

// Verify that the skiplist iterator passes over all elements (in order)
void test_sl_iterator(void) {
  skiplist *string_sl = skiplistCreate(compStrKeys, NULL, compareVals);
  skiplistNode *x;
  int first = 1, second = 2, third = 3;
  skiplistInsert(string_sl, "c_third", &third);
  skiplistInsert(string_sl, "a_first", &first);
  skiplistInsert(string_sl, "b_second", &second);

  skiplistIterator iter = skiplistIterateAll(string_sl);

  x = skiplistIterator_Next(&iter);
  assert (*(int *) x == 1);

  x = skiplistIterator_Next(&iter);
  assert (*(int *) x == 2);

  x = skiplistIterator_Next(&iter);
  assert (*(int *) x == 3);

  x = skiplistIterator_Next(&iter);
  assert (x == NULL);

  skiplistFree(string_sl);
}

int main(void) {
  test_string_sorts();
  test_sl_iterator();

  printf("test_skiplist - PASS!\n");
}
