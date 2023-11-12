/* ******************************* symbtable.h ****************************** */
/*  Verze: TRP-izp                                                            */
/* ************************************************************************** */
/*  Zdroje:                                                                   */
/*  - IAL, studijní opora, Prof. Ing. Jan M. Honzík, Csc., Verze 14 N         */
/* ************************************************************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, říjen 2023                          */
/*
 **/

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdbool.h>

/** symtable_init error. */
#define SYMTABLE_INIT_ERROR 1
/** symtable_insert error. */
#define SYMTABLE_INSERT_ERROR 2
/** symtable_search error. */
#define SYMTABLE_SEARCH_ERROR 3
/** symtable_expend error. */
#define SYMTABLE_EXPAND_ERROR 4

typedef struct symtable_item_t symtable_item_t;

struct symtable_item_t {
  /** The key of the item */
  char *key;
  /** The data of the item */
  void *data;  // TODO: Change to whatever we're gonna need
  /** Pointer to the next item with the same hash */
  symtable_item_t *next;
};

typedef struct {
  /** Current table capacity. */
  unsigned int capacity;
  /** Error code. */
  int error_code;
  /** Pointer to the first item of the table. */
  symtable_item_t **items;
} symtable_t;

symtable_t *symtable_new(unsigned int capacity);

void symtable_insert(symtable_t *table, char *key, void *data);

bool symtable_search(symtable_t *table, const char *key);

void symtable_delete(symtable_t *table, const char *key);

void *symtable_get(const symtable_t *table, const char *key);

void symtable_dispose(symtable_t *table);

#endif
