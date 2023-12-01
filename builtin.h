/* ******************************* builtin.h ******************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#ifndef BUILTIN_H
#define BUILTIN_H

#include "str.h"
#include "symtable.h"

void builtin_add_to_symtable(Symtable *symtable);

#endif