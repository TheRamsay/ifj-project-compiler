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

/** stack_init error. */
#define STACK_SERR_INIT 1
/** stack_push error. */
#define STACK_SERR_PUSH 2
/** stack_top error. */
#define STACK_SERR_TOP 3
/** stack_pop error. */
#define STACK_SERR_POP 4

/** ADT stack implemented with a static array. */
typedef struct {
  /** Pole pro uložení hodnot. */
  char *items;
  /** Index prvku na vrcholu zásobníku. */
  int topIndex;
  /** Stack size. */
  int size;
  /** Error code. */
  int error_code;
} Stack;

void stack_error(Stack *, int);

void stack_clear_error(Stack *);

void stack_init(Stack *, int);

bool stack_is_empty(const Stack *);

bool stack_is_full(const Stack *);

int stack_top(Stack *);

int stack_pop(Stack *);

void stack_push(Stack *, int);

void stack_dispose(Stack *);

#endif
