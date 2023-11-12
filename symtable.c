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
#include <string.h>

/*                            Private functions                               */
/**
 *
 * @see http://www.cse.yorku.ca/~oz/hash.html (variant sdbm)
 *
 * @param key The key to hash
 * @param capacity Current capacity of the symtable
 *
 * @returns The hash of the key
 *
 */
unsigned int hash_function(const char *key, const unsigned int capacity) {
  int h = 0;
  const unsigned char *p;

  for (p = (const unsigned char *)key; *p != '\0'; p++) {
    h = 65599 * h + *p;
  }

  return h % capacity;
}

/*                             Public functions                               */

/**
 *
 * @param capacity Initial capacity of the symtable
 *
 */
symtable_t *symtable_new(unsigned int capacity) {
  symtable_t *table = malloc(sizeof(symtable_t));
  if (table == NULL) {
    return NULL;
  }

  table->items = calloc(capacity, sizeof(symtable_item_t *));

  if (table->items == NULL) {
    table->error_code = SYMTABLE_INIT_ERROR;
    return NULL;
  }

  table->capacity = capacity;
  table->error_code = 0;

  return table;
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be inserted
 * @param data Data of the item to be inserted
 *
 */
void symtable_insert(symtable_t *table, char *key, void *data) {
  if (table == NULL) {
    table->error_code = SYMTABLE_INSERT_ERROR;
    return;
  }

  symtable_item_t *item = malloc(sizeof(symtable_item_t));

  if (item == NULL) {
    table->error_code = SYMTABLE_INSERT_ERROR;
    return;
  }

  item->key = key;
  item->data = data;
  item->next = NULL;

  unsigned int hash = hash_function(key, table->capacity);

  if (table->items[hash] == NULL) {
    table->items[hash] = item;
  } else {
    symtable_item_t *current = table->items[hash];

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
bool symtable_search(symtable_t *table, const char *key) {
  if (table == NULL) {
    table->error_code = SYMTABLE_SEARCH_ERROR;
    return false;
  }

  unsigned int hash = hash_function(key, table->capacity);

  symtable_item_t *current = table->items[hash];

  if (current == NULL) {
    return false;
  }

  while (current->key != key && current->next != NULL) {
    current = current->next;
  }

  if (strcmp(current->key, key) == 0) {
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
void symtable_delete(symtable_t *table, const char *key) {
  if (table == NULL) {
    table->error_code = SYMTABLE_SEARCH_ERROR;
    return;
  }

  unsigned int hash = hash_function(key, table->capacity);

  symtable_item_t *current = table->items[hash];
  symtable_item_t *previous = NULL;

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

    if (current->data != NULL) {
      free(current->data);
    }
    if (current->key != NULL) {
      free(current->key);
    }
    free(current);
  } else {
    table->error_code = SYMTABLE_SEARCH_ERROR;
  }
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be found
 *
 * @returns Pointer to the data of the item if found, NULL otherwise
 */
void *symtable_get(const symtable_t *table, const char *key) {
  if (table == NULL) {
    return NULL;
  }

  unsigned int hash = hash_function(key, table->capacity);

  symtable_item_t *current = table->items[hash];

  if (current == NULL) {
    return NULL;
  }

  while (current->key != key && current->next != NULL) {
    current = current->next;
  }

  if (current->key == key) {
    return current->data;
  } else {
    return NULL;
  }
}

/**
 *
 * Deletes and frees the allocated memory of the symtable and all of it's allocated items
 *
 * @param table Pointer to the symtable structure
 *
 */
void symtable_dispose(symtable_t *table) {
  if (table == NULL) {
    return;
  }

  for (unsigned int i = 0; i < table->capacity; i++) {
    symtable_item_t *current = table->items[i];
    symtable_item_t *next = NULL;

    while (current != NULL) {
      next = current->next;
      if (current->data != NULL) {
        free(current->data);
      }
      if (current->key != NULL) {
        free(current->key);
      }
      free(current);
      current = next;
    }
  }

  free(table->items);
  table->items = NULL;
  table->capacity = 0;
  table->error_code = 0;
}