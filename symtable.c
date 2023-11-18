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
 * @param capacity Initial capacity of the symtable
 *
 */
Symtable *symtable_init(unsigned int capacity)
{
	Symtable *table = malloc(sizeof(Symtable));

	if (table == NULL)
	{
		return NULL;
	}

	table->items = calloc(capacity, sizeof(SymtableItem *));

	if (table->items == NULL)
	{
		table->errorCode = SYMTABLE_INIT_ERROR;
		return NULL;
	}

	for (unsigned int i = 0; i < capacity; i++)
	{
		table->items[i] = NULL;
	}

	table->capacity = capacity;
	table->errorCode = 0;

	return table;
}

/**
 *
 * @param table Pointer to the symtable structure
 * @param key Key of the item to be searched
 *
 * @returns true if the item was found, false otherwise
 *
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

	while (strcmp(current->key, key) != 0 && current->next != NULL)
	{
		current = current->next;
	}

	if (strcmp(current->key, key) == 0)
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
 * @param key Key of the item to be searched
 *
 * @returns true if the item was found, false otherwise
 *
 */
bool symtable_search(Symtable *table, const char *key)
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
		return false;
	}

	while (strcmp(current->key, key) != 0 && current->next != NULL)
	{
		current = current->next;
	}

	if (strcmp(current->key, key) == 0)
	{
		return true;
	}
	else
	{
		return false;
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
	SymtableItem *item = symtable_get(table, key);

	// Item already exists
	if (item != NULL)
	{
		// Function is already defined -> error
		if (type == SYMTABLE_FUNCTION)
		{
			if (item->data.function.defined)
			{

				exit_with_error(SEMANTIC_ERR_FUNC);
			}
			else
			{
				item->data.function.defined = defined;
				return item;
			}
		}
		else
		{
			// Variable is already defined -> error
			exit_with_error(SEMANTIC_ERR_FUNC);
		}
	}

	size_t index = hash_function(key, table->capacity);

	item = calloc(1, sizeof(SymtableItem));

	if (item == NULL)
	{
		printf("malloc failed\n");
		return NULL;
	}

	item->key = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->key == NULL)
	{
		free(item);
		printf("malloc failed\n");
		return NULL;
	}

	strcpy(item->key, key);

	item->data.identifier = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->data.identifier == NULL)
	{
		free(item->key);
		free(item);
		printf("malloc failed\n");
		return NULL;
	}

	strcpy(item->data.identifier, key);

	item->data.type = type;
	item->next = table->items[index];

	if (item->data.type == SYMTABLE_FUNCTION)
	{
		item->data.function.param_count = 0;
		item->data.function.defined = defined;
		item->data.function.params = NULL;
		item->data.function._return = NULL;
	}
	else
	{
		item->data.variable.identifier_type = (SymtableIdentifierType){.data_type = UNKNOWN_TYPE, .nullable = false};
	}

	return item;
}

bool symtable_add_param(SymtableItem *item, char *out_identifier, char *in_identifier, SymtableIdentifierType identifier_type)
{
	if (item == NULL)
	{
		// Its over Anakin, I have the high ground
		exit_with_error(INTERNAL_ERROR);
	}

	SymtableParam *param = malloc(sizeof(SymtableParam));

	if (param == NULL)
	{
		return false;
	}

	param->identifier_type = identifier_type;
	param->next = NULL;

	if (out_identifier == NULL)
	{
		// TODO: figure out what to do with this
		// param->out_name = NULL;
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
		// TODO: figure out what to do with this
		// param->in_name = NULL;
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
			// Check for duplicate names
			if (strcmp(current->in_name, param->in_name) == 0 || strcmp(current->out_name, param->out_name) == 0)
			{
				exit_with_error(SEMANTIC_ERR_FUNC);
			}

			current = current->next;
		}

		current->next = param;
	}

	item->data.function.param_count++;

	return true;
}

bool symtable_add_return(SymtableItem *item, SymtableIdentifierType type)
{
	if (item == NULL)
	{
		// Its over Anakin, I have the high ground
		exit_with_error(INTERNAL_ERROR);
	}

	if (item->data.function._return == NULL)
	{

		SymtableReturn *return_type = malloc(sizeof(SymtableReturn));

		if (return_type == NULL)
		{
			return false;
		}

		return_type->data_type = type.data_type;
		item->data.function._return = return_type;
	}
	else
	{
		return false;
	}

	return true;
}

bool check_if_all_functions_defined(Symtable *table)
{
	for (unsigned int i = 0; i < table->capacity; i++)
	{
		SymtableItem *current = table->items[i];

		while (current != NULL)
		{
			if (current->data.type == SYMTABLE_FUNCTION && !current->data.function.defined)
			{
				return false;
			}

			current = current->next;
		}
	}

	return true;
}
