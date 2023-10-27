/* ******************************* stack.c ********************************** */
/*  Zdroj:                                                                    */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/*          Daniel Dolejška, září 2021                                        */
/* ************************************************************************** */
/*                                                                            */
/*  Zpracoval a upravil: Lukáš Witpeerd, říjen 2023
 **/

#include "stack.h"

#include <stdlib.h>

/**
 *
 * @param stack Pointer to the stack structure
 * @param error_code Internal error identifier
 */
void stack_error(Stack *stack, int errorCode) {
  if (errorCode <= 0) {
    errorCode = 0;
  }

  stack->error_code = errorCode;
}

/**
 *
 * @param stack Pointer to the stack structure
 */
void stack_clear_error(Stack *stack) { stack->error_code = 0; }

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 *
 * V případě, že funkce dostane jako parametr stack == NULL,
 * je volána funkce stack_error(SERR_INIT).
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void stack_init(Stack *stack, int size) {
  if (stack == NULL) {
    stack_error(stack, STACK_SERR_INIT);
    return;
  }

  stack->items = malloc(size * sizeof(int));
  stack->error_code = 0;
  stack->size = size;
  stack->topIndex = -1;
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník prázdný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns true v případě, že je zásobník prázdný, jinak false
 */
bool stack_is_empty(const Stack *stack) { return stack->topIndex == -1; }

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns true v případě, že je zásobník plný, jinak false
 */
bool stack_is_full(const Stack *stack) { return stack->topIndex == stack->size - 1; }

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 * Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
 * Volání operace Top při prázdném zásobníku je nekorektní a je ošěřeno voláním
 * procedury stack_error(SERR_TOP).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
int stack_top(Stack *stack) {
  if (stack_is_empty(stack)) {
    stack_error(stack, STACK_SERR_TOP);
    return 0;
  }

  return stack->items[stack->topIndex];
}

/**
 * Odstraní prvek z vrcholu zásobníku.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
int stack_pop(Stack *stack) {
  if (stack_is_empty(stack)) {
    stack_error(stack, STACK_SERR_POP);
    return 0;
  }

  return stack->items[stack->topIndex--];
}

/**
 * Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
 * je nekorektní a je ošetřen voláním procedury stack_error(SERR_PUSH).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void stack_push(Stack *stack, int data) {
  if (stack_is_full(stack)) {
    stack_error(stack, STACK_SERR_PUSH);
    return;
  }

  stack->items[stack->topIndex++ + 1] = data;
}

/**
 * Zruší a uvolní dynamicky alokované prostředky struktury.
 * Uvede zásobník do prázdného stavu.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void stack_dispose(Stack *stack) {
  if (stack == NULL) {
    return;
  }

  if (stack->items != NULL) {
    free(stack->items);
    stack->items = NULL;
  }

  stack->size = 0;
  stack->topIndex = -1;
}
