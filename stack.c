/* ******************************* stack.c ********************************** */
/*                                                                            */
/*  Zpracoval: Lukáš Witpeerd, říjen-listopad 2023
 **/

#include "stack.h"

#include <stdlib.h>

#include "str.h"
#include "exp_parser.h"

#include <string.h>

 /*                            Private functions                               */
void stack_resize(void_stack_t* stack) {
  stack->size *= 2;

  void** new_items = realloc(stack->items, stack->size * sizeof(void*));

  if (new_items == NULL) {
    stack_error(stack, STACK_SERR_RESIZE);
    return;
  }

  stack->items = new_items;
}

/* ************************************************************************** */

/*                            Public functions                                */
/**
 *
 * @param stack Pointer to the stack structure
 * @param error_code Internal error identifier
 */
void stack_error(void_stack_t* stack, int error_code) {
  if (error_code <= 0) {
    error_code = 0;
  }

  stack->error_code = error_code;
}

/**
 *
 * @param stack Pointer to the stack structure
 */
void stack_clear_error(void_stack_t* stack) { stack->error_code = 0; }

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 *
 * V případě, že funkce dostane jako parametr stack == NULL,
 * je volána funkce stack_error(SERR_INIT).
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void_stack_t* stack_new(int size) {
  void_stack_t* stack = malloc(sizeof(void_stack_t));

  if (stack == NULL) {
    return NULL;
  }

  stack->items = malloc(size * sizeof(void*));
  stack->error_code = 0;
  stack->size = size;
  stack->top_index = -1;

  return stack;
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník prázdný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns true v případě, že je zásobník prázdný, jinak false
 */
bool stack_is_empty(const void_stack_t* stack) { return stack->top_index == -1; }

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns true v případě, že je zásobník plný, jinak false
 */
bool stack_is_full(const void_stack_t* stack) { return stack->top_index == stack->size - 1; }

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 * Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
 * Volání operace Top při prázdném zásobníku je nekorektní a je ošěřeno voláním
 * procedury stack_error(SERR_TOP).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void* stack_top(void_stack_t* stack) {
  if (stack_is_empty(stack)) {
    stack_error(stack, STACK_SERR_TOP);
    return 0;
  }

  return stack->items[stack->top_index];
}

/**
 * Odstraní prvek z vrcholu zásobníku.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void* stack_pop(void_stack_t* stack) {
  if (stack_is_empty(stack)) {
    stack_error(stack, STACK_SERR_POP);
    return 0;
  }

  return stack->items[stack->top_index--];
}

/**
 * Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
 * je nekorektní a je ošetřen voláním procedury stack_error(SERR_PUSH).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void stack_push(void_stack_t* stack, void* data) {
  if (stack_is_full(stack)) {
    stack_resize(stack);
    if (stack->error_code != 0) {
      return;
    }
  }

  stack->items[stack->top_index++ + 1] = data;
}

/**
 * Zruší a uvolní dynamicky alokované prostředky struktury.
 * Uvede zásobník do prázdného stavu.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void stack_dispose(void_stack_t* stack) {
  if (stack == NULL) {
    return;
  }

  if (stack->items != NULL) {
    free(stack->items);
    stack->items = NULL;
  }

  stack->size = 0;
  stack->top_index = -1;
}

void stack_reverse(void_stack_t* stack) {
  void_stack_t* stack_reversed = stack_new((stack)->size);

  while (!stack_is_empty(stack)) {
    stack_push(stack_reversed, stack_pop(stack));
  }

  *stack = *stack_reversed;
}

void stack_print(void_stack_t* stack) {
  for (int i = 0; i < stack->top_index + 1; i++) {
    if (strstr(((str*)stack->items[i])->data, "float@") != NULL) {
      float buffer;

      sscanf(((str*)stack->items[i])->data, "float@%a", &buffer);

      fprintf(stderr, "%d. %f\n", i, buffer);
    }
    else {
      fprintf(stderr, "%d. %s\n", i, ((str*)stack->items[i])->data);

    }
  }
}
/* ************************************************************************** */