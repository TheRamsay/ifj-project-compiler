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
#include "error.h"

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
unsigned int hash_function(const char *key, const unsigned int capacity)
{
	int h = 0;
	const unsigned char *p;

	for (p = (const unsigned char *)key; *p != '\0'; p++)
	{
		h = 65599 * h + *p;
	}

	return h % capacity;
}

/*                             Public functions                               */

/**
 *
 * @param table Pointer to the symtable structure
 * @param capacity Initial capacity of the symtable
 *
 */
bool symtable_init(Symtable *table, unsigned int capacity)
{
	if (table == NULL)
	{
		return false;
	}

	table->items = calloc(capacity, sizeof(SymtableItem *));

	if (table->items == NULL)
	{
		table->errorCode = SYMTABLE_INIT_ERROR;
		return false;
	}

	table->capacity = capacity;
	table->errorCode = 0;

	return true;
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be searched
 *
 * @returns true if the item was found, false otherwise
 *
 */
SymtableItem *symtable_search(Symtable *table, const char *key)
{
	if (table == NULL)
	{
		table->errorCode = SYMTABLE_SEARCH_ERROR;
		return false;
	}

	unsigned int hash = hash_function(key, table->capacity);

	SymtableItem *current = table->items[hash];

	if (current == NULL)
	{
		return NULL;
	}

	while (current->key != key && current->next != NULL)
	{
		current = current->next;
	}

	if (current->key == key)
	{
		return current;
	}
	else
	{
		return NULL;
	}
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be deleted
 *
 */
void symtable_delete(Symtable *table, const char *key)
{
	if (table == NULL)
	{
		table->errorCode = SYMTABLE_SEARCH_ERROR;
		return;
	}

	unsigned int hash = hash_function(key, table->capacity);

	SymtableItem *current = table->items[hash];
	SymtableItem *previous = NULL;

	while (current->key != key && current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	if (current->key == key)
	{
		if (previous == NULL)
		{
			table->items[hash] = current->next;
		}
		else
		{
			previous->next = current->next;
		}
		if (current->key != NULL)
		{
			free(current->key);
		}
		free(current);
	}
	else
	{
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
SymtableItem *symtable_get(const Symtable *table, const char *key)
{
	if (table == NULL)
	{
		return NULL;
	}

	unsigned int hash = hash_function(key, table->capacity);

	SymtableItem *current = table->items[hash];

	if (current == NULL)
	{
		return NULL;
	}

	while (current->key != key && current->next != NULL)
	{
		current = current->next;
	}

	if (current->key == key)
	{
		return current;
	}
	else
	{
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
void symtable_dispose(Symtable *table)
{
	if (table == NULL)
	{
		return;
	}

	for (unsigned int i = 0; i < table->capacity; i++)
	{
		SymtableItem *current = table->items[i];
		SymtableItem *next = NULL;

		while (current != NULL)
		{
			next = current->next;
			if (current->key != NULL)
			{
				free(current->key);
			}
			free(current);
			current = next;
		}
	}

	free(table->items);
	table->items = NULL;
	table->capacity = 0;
	table->errorCode = 0;
}

SymtableItem *symtable_insert(Symtable *table, char *key, SymtableValueType type, bool defined)
{
	SymtableItem *item = symtable_search(table, key);

	// Item already exists
	if (item != NULL)
	{
		// Function is already defined -> error
		if (type == SYMTABLE_FUNCTION && item->data.function.defined == true)
		{
			exit_with_error(SEMANTIC_ERR_FUNC);
		}
	}

	size_t index = hash_function(key, table->capacity) % table->capacity;

	item = calloc(1, sizeof(SymtableItem));

	if (item == NULL)
	{
		return NULL;
	}

	item->key = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->key == NULL)
	{
		free(item);
		return NULL;
	}

	strcpy(item->key, key);

	item->data.identifier = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->data.identifier == NULL)
	{
		free(item->key);
		free(item);
		return NULL;
	}

	strcpy(item->data.identifier, key);

	item->data.type = type;
	item->next = table->items[index];

	if (item->data.type == SYMTABLE_FUNCTION)
	{
		item->data.function.param_count = 0;
		item->data.function.return_count = 0;
		item->data.function.defined = defined;
		item->data.function.params = NULL;
		item->data.function.returns = NULL;
	}
	else
	{
		item->data.variable.data_type = UNKNOWN_TYPE;
	}

	return item;
}

bool symtable_add_param(SymtableItem *item, char *out_identifier, char *in_identifier, SymtableDataType data_type)
{
	SymtableParam *param = malloc(sizeof(SymtableParam));
	if (param == NULL)
	{
		return false;
	}

	param->data_type = data_type;

	param->next = NULL;

	if (out_identifier == NULL)
	{
		param->out_name = NULL;
	}
	else
	{
		param->out_name = malloc(sizeof(char) * (strlen(out_identifier) + 1));
		if (param->out_name == NULL)
		{
			free(param);
			return false;
		}

		strcpy(param->out_name, out_identifier);
	}

	if (in_identifier == NULL)
	{
		param->in_name = NULL;
	}
	else
	{
		param->in_name = malloc(sizeof(char) * (strlen(in_identifier) + 1));
		if (param->in_name == NULL)
		{
			free(param->out_name);
			free(param);
			return false;
		}

		strcpy(param->in_name, in_identifier);
	}

	if (item->data.function.params == NULL)
	{
		item->data.function.params = param;
	}
	else
	{
		SymtableParam *current = item->data.function.params;

		while (current->next != NULL)
		{
			current = current->next;
		}

		current->next = param;
	}

	item->data.function.param_count++;

	return true;
}