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

/** Stack_Init error. */
#define STACK_SERR_INIT 1
/** Stack_Push error. */
#define STACK_SERR_PUSH 2
/** Stack_Top error. */
#define STACK_SERR_TOP 3

/** ADT zásobník implementovaný ve statickém poli. */
typedef struct {
  /** Pole pro uložení hodnot. */
  char *array;
  /** Index prvku na vrcholu zásobníku. */
  int topIndex;
  /** Stack size. */
  int size;
  /** Error code. */
  int error_code;
} Stack;

void Stack_Error(Stack *, int);

void Stack_ClearError(Stack *);

void Stack_Init(Stack *, int);

bool Stack_IsEmpty(const Stack *);

bool Stack_IsFull(const Stack *);

void Stack_Top(Stack *, int *);

void Stack_Pop(Stack *);

void Stack_Push(Stack *, int);

void Stack_Dispose(Stack *);

#endif
