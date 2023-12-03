/* ******************************* stack.h ********************************** */
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

#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include <stdio.h>

/** stack_push error. */
#define STACK_SERR_PUSH 1
/** void_stack_top error. */
#define STACK_SERR_TOP 2
/** stack_pop error. */
#define STACK_SERR_POP 3
/** stack_resize error. */
#define STACK_SERR_RESIZE 4

/** ADT stack implemented with a static array. */
typedef struct void_stack_t {
  /** Array for the elements of the stack. */
  void **items;
  /** Index of the top of the stack. */
  int top_index;
  /** Stack size. */
  int size;
  /** Error code. */
  int error_code;
} void_stack_t;

void_stack_t *stack_new(int size);

void stack_error(void_stack_t *stack, int error_code);

void stack_clear_error(void_stack_t *stack);

bool stack_is_empty(const void_stack_t *stack);

bool stack_is_full(const void_stack_t *stack);

void *stack_top(void_stack_t *stack);

void *stack_pop(void_stack_t *stack);

void stack_push(void_stack_t *stack, void *ptr);

void stack_dispose(void_stack_t *stack);

void stack_reverse(void_stack_t *stack);

void stack_print(void_stack_t *stack);

#endif