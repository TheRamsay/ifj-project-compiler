/* ******************************* generator.c ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#include "generator.h"

#include <stdlib.h>

/*                            Orchestration functions                         */

/**
 *
 * @brief Creates a new generator.
 *
 * @returns Pointer to the new generator. NULL if the allocation failed.
 *
 */
Generator *generator_new() {
  Generator *gen = malloc(sizeof(struct Generator));

  if (gen == NULL) {
    goto onfail;
  }

  gen->out_str = str_new(2000);
  if (gen->out_str == NULL) {
    goto onfail;
  }

  str *main_str = str_new_from("LABEL main\n");
  gen->main_str = main_str;
  if (gen->main_str == NULL) {
    goto onfail;
  }

  // ! MAX LABEL DEPTH
  void_stack_t *label_stack = stack_new(2048);
  gen->label_stack = label_stack;
  if (gen->label_stack == NULL) {
    goto onfail;
  };

  gen->depth = 0;

  return gen;

onfail:
  if (gen != NULL) {
    free(gen);
  }

  if (gen->out_str == NULL) {
    str_dispose(gen->out_str);
  }

  if (gen->main_str == NULL) {
    str_dispose(gen->main_str);
  }

  if (label_stack != NULL) {
    stack_dispose(label_stack);
  }

  return NULL;
}

/**
 *
 * @brief Prints the generator's output.
 *
 * @param gen The generator.
 *
 */
void generator_print(Generator *gen) {
  if (gen == NULL) {
    return;
  }

  printf("%s", gen->out_str->data);
}

/**
 *
 * @brief Disposes the generator.
 *
 * @param generator The generator.
 *
 */
void generator_dispose(Generator *gen) {
  if (gen == NULL) {
    return;
  }

  str_dispose(gen->out_str);
  str_dispose(gen->main_str);
  stack_dispose(gen->label_stack);

  free(gen);
}

/*                            Generator functions                             */
/*                            Private functions                               */

/*                            Public functions                                */
void generator_header(Generator *gen) {
  if (gen == NULL) {
    return;
  }

  str_append_cstr(gen->out_str, ".IFJCode23\n");
  str_append_cstr(gen->out_str, "# Kanvica: The Conqueror of Worls\n\n");
  str_append_cstr(gen->out_str, "JUMP main\n");
}

void generator_footer(Generator *gen) {
  if (gen == NULL) {
    return;
  }

  str_append_str_dispose(gen->out_str, &gen->main_str);
}

void generator_var_create(Generator *gen, str *name) {
  str *fr = str_new(4);
  str *dest;

  if (gen->depth == 0) {
    str_set_cstr(fr, "GF@");
    dest = gen->main_str;
  } else {
    str_set_cstr(fr, "LF@");
    dest = gen->out_str;
  }

  str_append_cstr(dest, "DEFVAR ");
  str_append_str_dispose(dest, &fr);
  str_append_str(dest, name);
  str_append_cstr(dest, "\n");
};