extern "C" {
#include "../symtable.h"
}

#include <string.h>

#include "gtest/gtest.h"

void delete_symtable(Symtable *symtable) {
  symtable_dispose(symtable);
  free(symtable);
}

char *getKey(const char *key) {
  char *newKey = (char *)malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(newKey, key);
  return newKey;
}

TEST(SymtableTest, New) {
  Symtable *symtable = symtable_new(2);
  ASSERT_EQ(symtable->capacity, 2);
  ASSERT_EQ(symtable->error_code, 0);
  ASSERT_NE(symtable->items, nullptr);

  delete_symtable(symtable);
}

TEST(SymtableTest, Insert) {
  Symtable *symtable = symtable_new(2);

  SymtableData *item1 = (SymtableData *)malloc(sizeof(SymtableData));
  char *key1 = getKey("a");
  symtable_insert(symtable, key1, item1);
  SymtableData *item2 = (SymtableData *)malloc(sizeof(SymtableData));
  char *key2 = getKey("b");
  symtable_insert(symtable, key2, item2);
  SymtableData *item3 = (SymtableData *)malloc(sizeof(SymtableData));
  char *key3 = getKey("c");
  symtable_insert(symtable, key3, item3);
  SymtableData *item4 = (SymtableData *)malloc(sizeof(SymtableData));
  char *key4 = getKey("d");
  symtable_insert(symtable, key4, item4);

  /*
  Resulting structure:
    [
      [0]: b, d,
      [1]: a, c
    ]
  */

  ASSERT_EQ(symtable->error_code, 0);

  ASSERT_TRUE(symtable_search(symtable, key1));
  ASSERT_TRUE(symtable_search(symtable, key2));
  ASSERT_TRUE(symtable_search(symtable, key3));
  ASSERT_TRUE(symtable_search(symtable, key4));
  char *key5 = getKey("e");
  ASSERT_FALSE(symtable_search(symtable, key5));

  ASSERT_EQ(symtable_get(symtable, key1)->data, item1);
  ASSERT_EQ(symtable_get(symtable, key2)->data, item2);
  ASSERT_EQ(symtable_get(symtable, key3)->data, item3);
  ASSERT_EQ(symtable_get(symtable, key4)->data, item4);
  ASSERT_EQ(symtable_get(symtable, key5), nullptr);

  free(key5);
  delete_symtable(symtable);
}

TEST(SymtableTest, Get) {
  Symtable *symtable = symtable_new(2);

  SymtableData *item1 = (SymtableData *)malloc(sizeof(SymtableData));
  char *key1 = getKey("a");

  ASSERT_FALSE(symtable_search(symtable, key1));
  ASSERT_EQ(symtable_get(symtable, key1), nullptr);
  symtable_insert(symtable, key1, item1);
  ASSERT_TRUE(symtable_search(symtable, key1));
  ASSERT_EQ(symtable_get(symtable, key1)->data, item1);

  char *key2 = getKey("b");
  ASSERT_FALSE(symtable_search(symtable, key2));

  free(key2);
  delete_symtable(symtable);
}
