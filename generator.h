/* ******************************* gen_t.h ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <stdio.h>

#include "stack.h"
#include "str.h"
#include "symtable.h"

typedef struct gen_t {
  str *out_str;
  str *main_str;

  void_stack_t *label_stack;
  void_stack_t *frame_stack;
  void_stack_t *construct_count_stack;

  int depth;
} gen_t;

/*                            Orchestration functions                         */
gen_t *generator_new();

void generator_print(gen_t *gen);

void generator_dispose(gen_t *gen);

/*                            Generator functions                             */
void generator_header(gen_t *gen);

void generator_footer(gen_t *gen);

void generator_var_create(gen_t *gen, str *name);

void generator_var_set(gen_t *gen, str *dest, str *src);

void generator_function_begin(gen_t *gen, str *name, void_stack_t *args);

void generator_function_end(gen_t *gen);

void generator_function_call(gen_t *gen, str *name, void_stack_t *args);

void generator_if_begin(gen_t *gen, str *left_symbol, char eq, str *right_symbol);

void generator_if_else(gen_t *gen);

void generator_if_end(gen_t *gen);

#endif