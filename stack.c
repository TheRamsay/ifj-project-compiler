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
void Stack_Error(Stack *stack, int error_code) {
  if (error_code <= 0) {
    error_code = 0;
  }

  stack->error_code = error_code;
}

/**
 *
 * @param stack Pointer to the stack structure
 */
void Stack_ClearError(Stack *stack) { stack->error_code = 0; }

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 *
 * V případě, že funkce dostane jako parametr stack == NULL,
 * je volána funkce Stack_Error(SERR_INIT).
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void Stack_Init(Stack *stack, int size) {
  if (stack == NULL) {
    Stack_Error(stack, STACK_SERR_INIT);
    return;
  }

  stack->array = malloc(size * sizeof(char));
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
bool Stack_IsEmpty(const Stack *stack) { return stack->topIndex == -1; }

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns true v případě, že je zásobník plný, jinak false
 */
bool Stack_IsFull(const Stack *stack) { return stack->topIndex == stack->size - 1; }

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 * Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
 * Volání operace Top při prázdném zásobníku je nekorektní a je ošěřeno voláním
 * procedury Stack_Error(SERR_TOP).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Stack_Top(Stack *stack, char *dataPtr) {
  if (Stack_IsEmpty(stack)) {
    Stack_Error(stack, STACK_SERR_TOP);
    return;
  }

  *dataPtr = stack->array[stack->topIndex];
}

/**
 * Odstraní prvek z vrcholu zásobníku.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void Stack_Pop(Stack *stack) {
  if (Stack_IsEmpty(stack)) {
    return;
  }

  stack->topIndex--;
}

/**
 * Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
 * je nekorektní a je ošetřen voláním procedury Stack_Error(SERR_PUSH).
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void Stack_Push(Stack *stack, char data) {
  if (Stack_IsFull(stack)) {
    Stack_Error(stack, STACK_SERR_PUSH);
    return;
  }

  stack->array[stack->topIndex++ + 1] = data;
}

/**
 * Zruší a uvolní dynamicky alokované prostředky struktury.
 * Uvede zásobník do prázdného stavu.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void Stack_Dispose(Stack *stack) {
  if (stack == NULL) {
    return;
  }

  if (stack->array == NULL) {
    return;
  }

  free(stack->array);
  stack->array = NULL;
  stack->size = 0;
  stack->topIndex = -1;
}
