#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "../../src/util/skiplist.h"
#include "../../src/value.h"
#include "../../src/graph/node.h"

// Numeric key comparator
int cmpDoubleKeys(void *p1, void *p2, void *ctx) {
  int a = *(double *)p1;
  int b = *(double *)p2;

  if (a > b) {
    return 1;
  } else if (a < b) {
    return -1;
  } else {
    return 0;
  }
}

// Numeric value comparator
int cmpDoubleVals(const void *p1, const void *p2) {
  int a = *(double *)p1;
  int b = *(double *)p2;

  if (a > b) {
    return 1;
  } else if (a < b) {
    return -1;
  } else {
    return 0;
  }
}

// String key comparator
int cmpStrKeys(void *a, void *b, void *ctx) {
  return strcmp(a, b);
}

// String value comparator
int cmpStrVals(const void *a, const void *b) {
  return strcmp(a, b);
}

void test_vals_allocation(void) {
  skiplist *sl = skiplistCreate(cmpStrKeys, NULL, cmpDoubleVals, NULL);
  double val_array[100];
  int i;
  for (i = 0; i < 100; i ++) {
    val_array[i] = (double) i;
    skiplistInsert(sl, "a", val_array + i);
  }
  skiplistNode *node = skiplistFind(sl, "a");

  // The values array initially increases by powers of 2
  assert(node->valsAllocated == 128);

  for (i = 0; i <= 100; i ++) {
    if (i % 10) skiplistDelete(sl, "a", val_array + i);
  }

  // The array shrinks to numVals when a delete causes
  // numVals to be <= valsAllocated / 4 ( 128 / 4 == 32)
  assert(node->valsAllocated == 32);
  // We are deleting 90 of the initial 100 values
  assert(node->numVals == 10);
}

// Verify lexicographic sorting of string keys
void test_string_sorts(void) {

  char *words[] = {"foo",  "bar",     "zap",    "pomo",
                   "pera", "arancio", "limone", NULL};

  char *vals[] = {"foo val", "bar val", "zap val", "pomo val",
                  "pera val", "arancio val", "limone val", NULL};

  skiplist *string_sl = skiplistCreate(cmpStrKeys, NULL, cmpStrVals, NULL);

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

// Verify skiplist operations on numeric keys
void test_numeric_sorts(void) {

  double keys[] = {5.5, 0, -30.2, 7, 1, 2, -1.5};

  // vals are defined as the order the keys should be in after sorting
  double vals[] = {5, 2, 0, 6, 3, 4, 1};

  skiplist *numeric_sl = skiplistCreate(cmpDoubleKeys, NULL, cmpDoubleVals, NULL);

  for (int i = 0; i < 7; i ++) {
    skiplistInsert(numeric_sl, keys + i, vals + i);
  }

  double num1 = -1;
  double num2;
  double *retval;
  skiplistIterator iter = skiplistIterateAll(numeric_sl);
  // Verify that skiplist is sorted on numeric keys
  while ((retval = skiplistIterator_Next(&iter)) != NULL) {
    assert(num1  + 1 == *(double *)retval);
    num1 = *retval;
  }

  // Search for a key not in skiplist
  skiplistNode *ret;
  num1 = -2;
  ret = skiplistFind(numeric_sl, &num1);
  assert (ret == NULL);

  // Search for a key in skiplist
  ret = skiplistFind(numeric_sl, keys + 5);
  assert (*(double*)ret->vals[0] == vals[5]);

  int delete_result;
  // Attempt to delete from a key that does not exist
  delete_result = skiplistDelete(numeric_sl, &num1, &num1);
  assert(delete_result == 0);

  // Attempt to delete a non-existent value from a real key
  delete_result = skiplistDelete(numeric_sl, &keys[0], &num1);
  assert(delete_result == 0);

  // Attempt to delete a non-existent skiplist key
  delete_result = skiplistDelete(numeric_sl, &num1, NULL);
  assert(delete_result == 0);

  // Retrieve the first node greater than an arbitrary number
  num1 = 0.5;
  ret = skiplistFindAtLeast(numeric_sl, &num1, 1);
  assert (*(double*)ret->obj == 1);

  // Iterate over a range of keys (-INF, 0]
  num2 = 0;
  iter = skiplistIterateRange(numeric_sl, NULL, &num2, 0, 1);

  // For this skiplist, this iterator should solely contain the first 2 key-value pairs
  retval = skiplistIterator_Next(&iter);
  assert(*retval == 0);
  retval = skiplistIterator_Next(&iter);
  assert(*retval == 1);
  retval = skiplistIterator_Next(&iter);
  assert(retval == NULL);

  // Iterate over a range of keys [0, 5]
  num1 = 0;
  num2 = 5;
  iter = skiplistIterateRange(numeric_sl, &num1, &num2, 0, 1);

  retval = skiplistIterator_Next(&iter);
  assert(*retval == 2);
  retval = skiplistIterator_Next(&iter);
  assert(*retval == 3);
  retval = skiplistIterator_Next(&iter);
  assert(*retval == 4);
  retval = skiplistIterator_Next(&iter);
  assert(retval == NULL);

  // Delete a node from the skiplist
  delete_result = skiplistDelete(numeric_sl, keys + 3, NULL);
  assert(delete_result == 1);
  ret = skiplistFind(numeric_sl, keys + 3);
  assert (ret == NULL);

  // Verify that the delete operation removes nodes from the skiplist
  iter = skiplistIterateAll(numeric_sl);
  while ((retval = skiplistIterator_Next(&iter)) != NULL) {
    assert(*(double *)retval != keys[3]);
  }

  skiplistFree(numeric_sl);
}

// Verify that the skiplist iterator passes over all elements (in order)
void test_sl_iterator(void) {
  skiplist *string_sl = skiplistCreate(cmpStrKeys, NULL, cmpDoubleVals, NULL);
  skiplistNode *x;
  double first = 1, second = 2, third = 3;
  skiplistInsert(string_sl, "c_third", &third);
  skiplistInsert(string_sl, "a_first", &first);
  skiplistInsert(string_sl, "b_second", &second);

  skiplistIterator iter = skiplistIterateAll(string_sl);

  x = skiplistIterator_Next(&iter);
  assert (*(double *) x == 1);

  x = skiplistIterator_Next(&iter);
  assert (*(double *) x == 2);

  x = skiplistIterator_Next(&iter);
  assert (*(double *) x == 3);

  x = skiplistIterator_Next(&iter);
  assert (x == NULL);

  skiplistFree(string_sl);
}

int main(void) {
  test_string_sorts();
  test_sl_iterator();
  test_numeric_sorts();
  test_vals_allocation();

  printf("test_skiplist - PASS!\n");
}
