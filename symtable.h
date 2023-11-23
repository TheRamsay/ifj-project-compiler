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
#include "stack.h"

/** symtable_init error. */
#define SYMTABLE_INIT_ERROR 1
/** symtable_insert error. */
#define SYMTABLE_INSERT_ERROR 2
/** symtable_search error. */
#define SYMTABLE_SEARCH_ERROR 3
/** symtable_expend error. */
#define SYMTABLE_EXPAND_ERROR 4

typedef struct SymtableItem SymtableItem;
typedef struct SymtableParam SymtableParam;
typedef struct SymtableReturn SymtableReturn;
typedef struct SymtableData SymtableData;

typedef enum
{
	SYMTABLE_VARIABLE,
	SYMTABLE_FUNCTION
} SymtableValueType;

typedef enum
{
	UNKNOWN_TYPE,
	INT_TYPE,
	DOUBLE_TYPE,
	STRING_TYPE,
	VOID_TYPE
} SymtableDataType;

typedef struct
{
	bool nullable;
	SymtableDataType data_type;
} SymtableIdentifierType;

typedef struct
{
	SymtableIdentifierType identifier_type;
	bool initialized;
	bool constant;
	bool param;
} SymtableVariable;

struct SymtableParam
{
	char *out_name;
	char *in_name;
	SymtableIdentifierType identifier_type;
	SymtableParam *next;
};

struct SymtableReturn
{
	SymtableIdentifierType identifier_type;
};

typedef struct
{
	int param_count;
	bool defined;
	SymtableParam *params;
	SymtableReturn *_return;
} SymtableFunction;

struct SymtableData
{
	SymtableValueType type;
	char *identifier;
	union
	{
		SymtableFunction function;
		SymtableVariable variable;
	};
};

struct SymtableItem
{
	/** The key of the item */
	char *key;
	/** The data of the item */
	// void *data; // TODO: Change to whatever we're gonna need
	SymtableData *data;
	/** Pointer to the next item with the same hash */
	SymtableItem *next;
};

typedef struct
{
	/** Current table capacity. */
	unsigned int capacity;
	/** Error code. */
	int error_code;
	/** Pointer to the first item of the table. */
	SymtableItem **items;
} Symtable;

Symtable *symtable_new(unsigned int capacity);

bool symtable_search(Symtable *table, const char *key);

void symtable_delete(Symtable *table, const char *key);

SymtableItem *symtable_get(const Symtable *table, const char *key);

void symtable_dispose(Symtable *table);

void symtable_clear(Symtable *table);

SymtableItem *symtable_add_symbol(Symtable *table, char *key, SymtableValueType type, bool defined, bool constant, bool param);
SymtableItem *symtable_insert(Symtable *table, char *key, SymtableData *data);

bool symtable_add_param(SymtableItem *item, char *out_identifier, char *in_identifier, SymtableIdentifierType identifier_type);

bool symtable_add_return(SymtableItem *item, SymtableIdentifierType type);

bool check_if_all_functions_defined(Symtable *table);

#endif