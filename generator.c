/* ******************************* gen_t.c ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#include "generator.h"

#include <stdlib.h>

/*                            Orchestration functions                         */

/**
 *
 * @brief Creates a new gen_t.
 *
 * @returns Pointer to the new gen_t. NULL if the allocation failed.
 *
 */
gen_t *generator_new() {
  gen_t *gen = malloc(sizeof(struct gen_t));

  if (gen == NULL) {
    goto onfail;
  }

  gen->out_str = str_new(2000);
  if (gen->out_str == NULL) {
    goto onfail;
  }

  str *main_str = str_new_from("\nLABEL main\n");
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
 * @brief Prints the gen_t's output.
 *
 * @param gen The gen_t.
 *
 */
void generator_print(gen_t *gen) {
  if (gen == NULL) {
    return;
  }

  printf("%s", gen->out_str->data);
}

/**
 *
 * @brief Disposes the gen_t.
 *
 * @param gen_t The gen_t.
 *
 */
void generator_dispose(gen_t *gen) {
  if (gen == NULL) {
    return;
  }

  str_dispose(gen->out_str);
  str_dispose(gen->main_str);
  stack_dispose(gen->label_stack);

  free(gen);
}

/*                            gen_t functions                             */
/*                            Private functions                               */
str *get_label(gen_t *gen) {
  if (gen->label_stack->top_index == -1) {
    return str_new_from("$");
  }

  str *label = str_new((gen->label_stack->top_index + 1) * 5);
  for (int i = 0; i <= gen->label_stack->top_index; i++) {
    str_append_str(label, gen->label_stack->items[i]);
    str_append_cstr(label, "$");
  }

  return label;
}

str *get_frame(gen_t *gen) {
  str *fr = str_new(4);

  if (gen->depth == 0) {
    str_set_cstr(fr, "GF@");
  } else {
    str_set_cstr(fr, "TF@");
  }

  return fr;
}

str *get_dest(gen_t *gen) {
  str *dest;

  if (gen->depth == 0) {
    dest = gen->main_str;
  } else {
    dest = gen->out_str;
  }

  return dest;
}

/*                            Public functions                                */
void generator_header(gen_t *gen) {
  if (gen == NULL) {
    return;
  }

  str_append_cstr(gen->out_str, ".IFJCode23\n");
  str_append_cstr(gen->out_str, "# Kanvica: The Conqueror of Worls\n\n");
  str_append_cstr(gen->out_str, "JUMP main\n");
}

void generator_footer(gen_t *gen) {
  if (gen == NULL) {
    return;
  }

  str_append_str_dispose(gen->out_str, &gen->main_str);
}

void generator_var_create(gen_t *gen, str *name) {
  str *fr = get_frame(gen);
  str *dest = get_dest(gen);

  str_append_cstr(dest, "DEFVAR ");
  str_append_str_dispose(dest, &fr);
  str_append_str(dest, stack_top(gen->label_stack));
  str_append_cstr(dest, "$");
  str_append_str(dest, name);
  str_append_cstr(dest, "\n");
};

void generator_var_set(gen_t *gen, str *name, str *value) {
  str *fr = get_frame(gen);
  str *dest = get_dest(gen);

  str_append_cstr(dest, "MOVE ");
  str_append_str_dispose(dest, &fr);
  str_append_cstr(dest, "$");
  str_append_str(dest, name);
  str_append_cstr(dest, " ");
  str_append_str(dest, value);
  str_append_cstr(dest, "\n");
};

void generator_function_begin(gen_t *gen, str *name, void_stack_t *args) {
  stack_push(gen->label_stack, str_new_from(name->data));
  gen->depth++;

  str *label = get_label(gen);

  str_append_cstr(gen->out_str, "\nLABEL ");
  str_append_str(gen->out_str, label);
  str_append_cstr(gen->out_str, "\n");

  // str_append_cstr(gen->out_str, "CREATEFRAME\nPUSHFRAME\n");

  if (args == NULL) {
    return;
  }

  while (!stack_is_empty(args)) {
    str *arg = stack_pop(args);

    str_append_cstr(gen->out_str, "DEFVAR TF@");
    str_append_str(gen->out_str, label);
    str_append_str(gen->out_str, arg);

    str_append_cstr(gen->out_str, "\nPOPS TF@");
    str_append_str(gen->out_str, label);
    str_append_str(gen->out_str, arg);

    str_append_cstr(gen->out_str, "\n");

    str_dispose(arg);
  }

  str_append_cstr(gen->out_str, "\n");

  str_dispose(label);
};

void generator_function_end(gen_t *gen) {
  str_dispose(stack_pop(gen->label_stack));
  gen->depth--;

  str_append_cstr(gen->out_str, "POPFRAME\nRETURN\n\n");
};

void generator_function_call(gen_t *gen, str *name, void_stack_t *args) {
  str *fr = get_frame(gen);
  str *dest = get_dest(gen);
  str *label = get_label(gen);

  if (args != NULL) {
    while (!stack_is_empty(args)) {
      str *arg = stack_pop(args);

      str_append_cstr(dest, "PUSHS ");
      str_append_str(dest, fr);
      str_append_str(dest, label);
      str_append_str(dest, arg);
      str_append_cstr(dest, "\n");

      str_dispose(arg);
    }
  }

  if (dest == gen->out_str) {
    str_append_cstr(dest, "PUSHFRAME\n");
  }
  str_append_cstr(dest, "CREATEFRAME\n");

  str_append_cstr(dest, "CALL ");
  // str_append_str(dest, label);
  str_append_str(dest, name);
  str_append_cstr(dest, "$");
  str_append_cstr(dest, "\n");
};