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
Symtable *symtable_new(unsigned int capacity)
{
	Symtable *table = malloc(sizeof(Symtable));

	if (table == NULL)
	{
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	table->items = calloc(capacity, sizeof(SymtableItem *));

	if (table->items == NULL)
	{
		table->error_code = SYMTABLE_INIT_ERROR;
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	for (unsigned int i = 0; i < capacity; i++)
	{
		table->items[i] = NULL;
	}

	table->capacity = capacity;
	table->error_code = 0;

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
		exit_with_error(INTERNAL_ERROR, "Symtable is NULL");
	}

	unsigned int hash = hash_function(key, table->capacity);

	SymtableItem *current = table->items[hash];

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return current;
		}
		current = current->next;
	}

	return NULL;
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
		table->error_code = SYMTABLE_SEARCH_ERROR;
		exit_with_error(INTERNAL_ERROR, "Symtable is NULL");
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
		table->error_code = SYMTABLE_SEARCH_ERROR;
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
		table->error_code = SYMTABLE_SEARCH_ERROR;
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
	table->error_code = 0;
}

SymtableItem *symtable_add_symbol(Symtable *table, char *key, SymtableValueType type, bool defined, bool constant, bool param)
{
	SymtableItem *item = symtable_get(table, key);
	// Item already exists
	if (item != NULL)
	{
		// Function is already defined -> error
		if (type == SYMTABLE_FUNCTION)
		{
			if (item->data->function.defined)
			{

				exit_with_error(SEMANTIC_ERR_FUNC, "Function %s already defined", key);
			}
			else
			{
				item->data->function.defined = defined;
				return item;
			}
		}
		else
		{
			// Variable is already defined but only like a parameter -> ok
			if (item->data->variable.param)
			{
				item->data->variable.constant = constant;
				item->data->variable.param = false;
			}
			// Variable is already defined and not like a parameter -> error
			else
			{
				// Variable is already defined -> error
				exit_with_error(SEMANTIC_ERR_FUNC, "Variable %s already defined", key);
			}
		}
	}

	size_t index = hash_function(key, table->capacity);

	item = calloc(1, sizeof(SymtableItem));

	if (item == NULL)
	{
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	item->data = calloc(1, sizeof(SymtableData));

	if (item == NULL || item->data == NULL)
	{
		free(item);
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	item->key = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->key == NULL)
	{
		free(item->data);
		free(item);
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	strcpy(item->key, key);

	item->data->identifier = malloc(sizeof(char) * (strlen(key) + 1));

	if (item->data->identifier == NULL)
	{
		free(item->data);
		free(item->key);
		free(item);
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	strcpy(item->data->identifier, key);

	item->data->type = type;
	item->next = table->items[index];

	if (item->data->type == SYMTABLE_FUNCTION)
	{
		item->data->function.param_count = 0;
		item->data->function.defined = defined;
		item->data->function.params = NULL;
		item->data->function._return = NULL;
	}
	else
	{
		item->data->variable.identifier_type = (SymtableIdentifierType){.data_type = UNKNOWN_TYPE, .nullable = false};
		item->data->variable.initialized = defined;
		item->data->variable.constant = constant;
		item->data->variable.param = param;
	}

	table->items[index] = item;
	return item;
}

bool symtable_add_param(SymtableItem *item, char *out_identifier, char *in_identifier, SymtableIdentifierType identifier_type)
{
	if (item == NULL)
	{
		// Its over Anakin, I have the high ground
		exit_with_error(INTERNAL_ERROR, "Symtable item is NULL");
	}

	if (strcmp(in_identifier, out_identifier) == 0)
	{
		exit_with_error(SEMANTIC_ERR_FUNC, "Parameter name cannot be the same as the function name");
	}

	if (strcmp(in_identifier, "_") == 0)
	{
		exit_with_error(SYNTAX_ERR, "Parametr name cannot be _");
	}

	SymtableParam *param = malloc(sizeof(SymtableParam));

	if (param == NULL)
	{
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
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
			exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
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
			exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
		}

		strcpy(param->in_name, in_identifier);
	}

	if (item->data->function.params == NULL)
	{
		item->data->function.params = param;
	}
	else
	{
		SymtableParam *current = item->data->function.params;

		while (current != NULL)
		{
			// Check for duplicate names
			if (strcmp(current->in_name, param->in_name) == 0)
			{
				exit_with_error(SEMANTIC_ERR_FUNC, "Duplicate parameter name");
			}

			current = current->next;
		}

		current = param;
	}

	item->data->function.param_count++;

	return true;
}

bool symtable_add_return(SymtableItem *item, SymtableIdentifierType type)
{
	if (item == NULL)
	{
		// Its over Anakin, I have the high ground
		exit_with_error(INTERNAL_ERROR, "Symtable item is NULL");
	}

	if (item->data->function._return == NULL)
	{

		SymtableReturn *return_type = malloc(sizeof(SymtableReturn));

		if (return_type == NULL)
		{
			exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
		}

		return_type->identifier_type = type;
		item->data->function._return = return_type;
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
			if (current->data->type == SYMTABLE_FUNCTION && !current->data->function.defined)
			{
				return false;
			}

			current = current->next;
		}
	}

	return true;
}

SymtableItem *symtable_insert(Symtable *table, char *key, SymtableData *data)
{
	if (table == NULL)
	{
		table->error_code = SYMTABLE_INSERT_ERROR;
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	SymtableItem *item = malloc(sizeof(SymtableItem));

	if (item == NULL)
	{
		table->error_code = SYMTABLE_INSERT_ERROR;
		exit_with_error(INTERNAL_ERROR, "Allocating memory for symtable failed");
	}

	item->key = key;
	item->data = data;
	item->next = NULL;

	unsigned int hash = hash_function(key, table->capacity);

	if (table->items[hash] == NULL)
	{
		table->items[hash] = item;
	}
	else
	{
		SymtableItem *current = table->items[hash];

		while (current->next != NULL)
		{
			current = current->next;
		}

		current->next = item;
	}

	return item;
}

void symtable_clear(Symtable *table)
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

		table->items[i] = NULL;
	}

	table->error_code = 0;
}