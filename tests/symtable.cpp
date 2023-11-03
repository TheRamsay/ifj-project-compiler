extern "C"
{
#include "../symtable.h"
}

#include <string.h>
#include "gtest/gtest.h"

Symtable *create_symtable(unsigned int capacity) {
  Symtable *symtable = (Symtable *) malloc(sizeof(Symtable));
  symtable_init(symtable, capacity);
  return symtable;
}

void delete_symtable(Symtable *symtable) {
  symtable_dispose(symtable);
  free(symtable);
}

char *getKey(const char* key) {
  char *newKey = (char *) malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(newKey, key);
  return newKey;
}

TEST(SymtableTest, Init)
{
  Symtable *symtable = NULL;
  ASSERT_FALSE(symtable_init(symtable, 5));

  symtable = create_symtable(2);
  ASSERT_EQ(symtable->capacity, 2);
  ASSERT_EQ(symtable->errorCode, 0);
  ASSERT_NE(symtable->items, nullptr);

  delete_symtable(symtable);
}

TEST(SymtableTest, Insert)
{

  Symtable *symtable = create_symtable(2);

  void *item1 = malloc(1);
  char *key1 = getKey("a");
  symtable_insert(symtable, key1, item1);
  void *item2 = malloc(1);
  char *key2 = getKey("b");
  symtable_insert(symtable, key2, item2);
  void *item3 = malloc(1);
  char *key3 = getKey("c");
  symtable_insert(symtable, key3, item3);
  void *item4 = malloc(1);
  char *key4 = getKey("d");
  symtable_insert(symtable, key4, item4);

  /*
  Resulting structure:
    [
      [0]: b, d,
      [1]: a, c
    ]
  */

  ASSERT_EQ(symtable->errorCode, 0);

  ASSERT_TRUE(symtable_search(symtable, key1));
  ASSERT_TRUE(symtable_search(symtable, key2));
  ASSERT_TRUE(symtable_search(symtable, key3));
  ASSERT_TRUE(symtable_search(symtable, key4));
  char *key5 = getKey("e");
  ASSERT_FALSE(symtable_search(symtable, key5));

  ASSERT_EQ(symtable_get(symtable, key1), item1);
  ASSERT_EQ(symtable_get(symtable, key2), item2);
  ASSERT_EQ(symtable_get(symtable, key3), item3);
  ASSERT_EQ(symtable_get(symtable, key4), item4);
  ASSERT_EQ(symtable_get(symtable, key5), nullptr);

  free(key5);
  delete_symtable(symtable);
}

TEST(SymtableTest, Get) {
  Symtable *symtable = create_symtable(2);

  void *item1 = malloc(1);
  char *key1 = getKey("a");

  ASSERT_FALSE(symtable_search(symtable, key1));
  ASSERT_EQ(symtable_get(symtable, key1), nullptr);
  symtable_insert(symtable, key1, item1);
  ASSERT_TRUE(symtable_search(symtable, key1));
  ASSERT_EQ(symtable_get(symtable, key1), item1);

  char *key2 = getKey("b");
  ASSERT_FALSE(symtable_search(symtable, key2));

  free(key2);
  delete_symtable(symtable);
}

TEST(SymtableTest, Delete) {
  Symtable *symtable = create_symtable(2);

  void *item1 = malloc(1);
  char *key1 = getKey("a");
  symtable_insert(symtable, key1, item1);
  void *item2 = malloc(1);
  char *key2 = getKey("b");
  symtable_insert(symtable, key2, item2);
  void *item3 = malloc(1);
  char *key3 = getKey("c");
  symtable_insert(symtable, key3, item3);
  void *item4 = malloc(1);
  char *key4 = getKey("d");
  symtable_insert(symtable, key4, item4);

  /*
    Resulting structure:
      [
        [0]: b, d
        [1]: a, c
      ]
  */
 
  symtable_delete(symtable, key2); // b

  /*
    Resulting structure:
      [
        [0]: d
        [1]: a, c
      ]
  */

  key2 = getKey("b"); // deleted key2, need to create new one
  ASSERT_FALSE(symtable_search(symtable, key2));
  ASSERT_EQ(symtable_get(symtable, key2), nullptr);

  ASSERT_TRUE(symtable_search(symtable, key4)); // d



  symtable_delete(symtable, key3); // c

  /*
    Resulting structure:
      [
        [0]: d
        [1]: a
      ]
  */

  key3 = getKey("c"); // deleted key3, need to create new one
  ASSERT_FALSE(symtable_search(symtable, key3));

  ASSERT_TRUE(symtable_search(symtable, key1)); // a



  symtable_delete(symtable, key1); // a

  /*
    Resulting structure:
      [
        [0]: d
        [1]:
      ]
  */

  key1 = getKey("a"); // deleted key3, need to create new one
  ASSERT_FALSE(symtable_search(symtable, key1));

  free(key1);
  free(key2);
  free(key3);
  delete_symtable(symtable);
}