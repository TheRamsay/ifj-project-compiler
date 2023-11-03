/* ******************************* generator.h ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <stdio.h>

#include "str.h"

typedef struct Generator {
  str *out_str;
} Generator;

/*                            Orchestration functions                         */
Generator *generator_new();

void generator_print(Generator *gen);

void generator_dispose(Generator *gen);

/*                            Generator functions                             */
void generator_header(Generator *gen);

#endif