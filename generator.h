/* ******************************* generator.h ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <stdio.h>

#include "stack.h"
#include "str.h"

typedef struct Generator {
  str *out_str;
  str *main_str;

  Stack *label_stack;

  int depth;
} Generator;

/*                            Orchestration functions                         */
Generator *generator_new();

void generator_print(Generator *gen);

void generator_dispose(Generator *gen);

/*                            Generator functions                             */
void generator_header(Generator *gen);

void generator_footer(Generator *gen);

void generator_var_create(Generator *gen, str *name);

void generator_function_begin(Generator *gen, str *name);

#endif