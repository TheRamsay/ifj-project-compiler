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

typedef struct SymtableItem SymtableItem;

struct SymtableItem {
  /** The key of the item */
  char *key;
  /** The data of the item */
  void *data;  // TODO: Change to whatever we're gonna need
  /** Pointer to the next item with the same hash */
  SymtableItem *next;
};

typedef struct {
  /** Current table capacity. */
  unsigned int capacity;
  /** Error code. */
  int errorCode;
  /** Pointer to the first item of the table. */
  SymtableItem **items;
} Symtable;

void symtable_init(Symtable *table, unsigned int capacity);

void symtable_insert(Symtable *table, char *key, void *data);

bool symtable_search(Symtable *table, const char *key);

void symtable_delete(Symtable *table, const char *key);

void *symtable_get(const Symtable *table, const char *key);

#endif
