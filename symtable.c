/* ******************************* symbtable.c ****************************** */
/*  Verze: TRP-izp                                                            */
/* ************************************************************************** */
/*  Zdroje:                                                                   */
/*  - IAL, studijní opora, Prof. Ing. Jan M. Honzík, Csc., Verze 14 N         */
/* ************************************************************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, říjen 2023                          */
/*
 **/

#include "symtable.h"

#include <stdlib.h>

/*                            Private functions                               */
/**
 *
 * @see http://www.cse.yorku.ca/~oz/hash.html (variant sdbm)
 *
 * @param key The key to hash
 * @param capacity Current size of the symtable
 *
 * @returns The hash of the key
 *
 */
unsigned int hash_function(const char *key, const unsigned int size) {
  int h = 0;
  const unsigned char *p;

  for (p = (const unsigned char *)key; *p != '\0'; p++) {
    h = 65599 * h + *p;
  }

  return h % size;
}

/*                             Public functions                               */

/**
 *
 * @param table Pointer to the symtable structure
 * @param capacity Initial capacity of the symtable
 *
 */
void symtable_init(Symtable *table, unsigned int capacity) {
  if (table == NULL) {
    table->errorCode = SYMTABLE_INIT_ERROR;
    return;
  }

  table->size = 0;
  table->capacity = capacity;

  table->errorCode = 0;

  table->items = calloc(capacity, sizeof(SymtableItem *));

  if (table->items == NULL) {
    table->errorCode = SYMTABLE_INIT_ERROR;
  }
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be inserted
 * @param data Data of the item to be inserted
 *
 */
void symtable_insert(Symtable *table, char *key, void *data) {
  if (table == NULL) {
    table->errorCode = SYMTABLE_INSERT_ERROR;
    return;
  }

  SymtableItem *item = malloc(sizeof(SymtableItem));

  if (item == NULL) {
    table->errorCode = SYMTABLE_INSERT_ERROR;
    return;
  }

  item->key = key;
  item->data = data;
  item->next = NULL;

  unsigned int hash = hash_function(key, table->capacity);

  if (table->items[hash] == NULL) {
    table->items[hash] = item;
  } else {
    SymtableItem *current = table->items[hash];

    while (current->next != NULL) {
      current = current->next;
    }

    current->next = item;
  }
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be searched
 *
 * @returns true if the item was found, false otherwise
 *
 */
bool symtable_search(Symtable *table, const char *key) {
  if (table == NULL) {
    table->errorCode = SYMTABLE_SEARCH_ERROR;
    return false;
  }

  unsigned int hash = hash_function(key, table->capacity);

  SymtableItem *current = table->items[hash];
  while (current->key != key && current->next != NULL) {
    current = current->next;
  }

  if (current->key == key) {
    return true;
  } else {
    return false;
  }
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be deleted
 *
 */
void symtable_delete(Symtable *table, const char *key) {
  if (table == NULL) {
    table->errorCode = SYMTABLE_SEARCH_ERROR;
    return;
  }

  unsigned int hash = hash_function(key, table->capacity);

  SymtableItem *current = table->items[hash];
  SymtableItem *previous = NULL;

  while (current->key != key && current->next != NULL) {
    previous = current;
    current = current->next;
  }

  if (current->key == key) {
    if (previous == NULL) {
      table->items[hash] = current->next;
    } else {
      previous->next = current->next;
    }

    free(current);
  } else {
    table->errorCode = SYMTABLE_SEARCH_ERROR;
  }
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be found
 *
 * @returns Pointer to the data of the item if found, NULL otherwise
 */
void *symtable_get(const Symtable *table, const char *key) {
  if (table == NULL) {
    return NULL;
  }

  unsigned int hash = hash_function(key, table->capacity);

  SymtableItem *current = table->items[hash];
  while (current->key != key && current->next != NULL) {
    current = current->next;
  }

  if (current->key == key) {
    return current->data;
  } else {
    return NULL;
  }
}