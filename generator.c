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
    return NULL;
  }

  gen->out_str = str_new(2000);

  if (gen->out_str == NULL) {
    free(gen);
    return NULL;
  }

  return gen;
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
}