/* ******************************* gen_t.c ****************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#include "generator.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  gen->label_stack = stack_new(8192);
  if (gen->label_stack == NULL) {
    goto onfail;
  };

  // ! MAX FRAME DEPTH
  gen->frame_stack = stack_new(8192);
  if (gen->frame_stack == NULL) {
    goto onfail;
  };
  stack_push(gen->frame_stack, symtable_new(128));

  // ! MAX DEPTH
  gen->construct_count_stack = stack_new(8192);
  if (gen->construct_count_stack == NULL) {
    goto onfail;
  };
  stack_push(gen->construct_count_stack, 0);

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

  if (gen->label_stack != NULL) {
    stack_dispose(gen->label_stack);
  }

  if (gen->frame_stack != NULL) {
    stack_dispose(gen->frame_stack);
  }

  if (gen->construct_count_stack != NULL) {
    stack_dispose(gen->construct_count_stack);
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
str *get_label(gen_t *gen, int depth) {
  if (gen->label_stack->top_index == -1 || depth == 0) {
    return str_new_from("$");
  }

  str *label = str_new((gen->label_stack->top_index + 1) * 5);
  for (int i = 0; i <= gen->label_stack->top_index; i++) {
    if (depth == 0) {
      break;
    }

    str_append_str(label, gen->label_stack->items[i]);
    str_append_cstr(label, "$");

    depth--;
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

int get_closest_var_depth(gen_t *gen, str *name) {
  for (int i = gen->frame_stack->top_index; i >= 0; i--) {
    symtable_t *frame = gen->frame_stack->items[i];

    if (symtable_search(frame, name->data)) {
      return i;
    }
  }

  return -1;
}

bool is_constant(str *name) {
  for (int i = 0; i < strlen(name->data); i++) {
    if (name->data[i] == '@') {
      return true;
    }
  }

  return false;
}

str *get_closest_var_path(gen_t *gen, str *name) {
  int depth = get_closest_var_depth(gen, name);

  str *path;
  if (depth == 0) {
    path = str_new_from("GF@");
  } else {
    path = str_new_from("TF@");
  }

  str_append_str(path, get_label(gen, depth));
  str_append_str(path, name);
  return path;
}

str *get_symbol_path(gen_t *gen, str *name) {
  str *path;

  if (is_constant(name)) {
    path = str_new_from(name->data);
  } else {
    path = get_closest_var_path(gen, name);
  }
}

/*                            Public functions                                */
void generator_header(gen_t *gen) {
  if (gen == NULL) {
    return;
  }

  str_append_cstr(gen->out_str, ".IFJCode23\n");
  str_append_cstr(gen->out_str, "# Kanvica: The Conqueror of Worlds\n\n");
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

  symtable_insert(stack_top(gen->frame_stack), str_to_cstr(name), NULL);

  str *label = get_label(gen, -1);

  str_append_cstr(dest, "DEFVAR ");
  str_append_str_dispose(dest, &fr);
  str_append_str(dest, label);
  str_append_str(dest, name);
  str_append_cstr(dest, "\n");
};

void generator_var_set(gen_t *gen, str *dest_symbol, str *src_symbol) {
  str *dest = get_dest(gen);

  str *dest_path = get_symbol_path(gen, dest_symbol);
  str *src_path = get_symbol_path(gen, src_symbol);

  str_append_cstr(dest, "MOVE ");
  str_append_str(dest, dest_path);
  str_append_cstr(dest, " ");
  str_append_str(dest, src_path);
  str_append_cstr(dest, "\n");
};

void generator_function_begin(gen_t *gen, str *name, void_stack_t *args) {
  stack_push(gen->label_stack, str_new_from(name->data));
  stack_push(gen->frame_stack, symtable_new(128));
  gen->depth++;

  str *label = get_label(gen, -1);

  str_append_cstr(gen->out_str, "\nLABEL ");
  str_append_str(gen->out_str, label);
  str_append_cstr(gen->out_str, "\n");

  if (args == NULL) {
    return;
  }

  while (!stack_is_empty(args)) {
    str *arg = stack_pop(args);

    generator_var_create(gen, arg);

    str *path = get_closest_var_path(gen, arg);
    str_append_cstr(gen->out_str, "POPS ");
    str_append_str(gen->out_str, path);

    str_append_cstr(gen->out_str, "\n");

    str_dispose(arg);
  }

  str_append_cstr(gen->out_str, "\n");

  str_dispose(label);
};

void generator_function_end(gen_t *gen) {
  str_dispose(stack_pop(gen->label_stack));
  symtable_dispose(stack_pop(gen->frame_stack));
  gen->depth--;

  str_append_cstr(gen->out_str, "RETURN\n\n");
};

void generator_function_call(gen_t *gen, str *name, void_stack_t *args) {
  str *fr = get_frame(gen);
  str *dest = get_dest(gen);
  str *label = get_label(gen, -1);

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
  str_append_str(dest, name);
  str_append_cstr(dest, "$");
  str_append_cstr(dest, "\n");

  if (dest == gen->out_str) {
    str_append_cstr(dest, "POPFRAME\n");
  }
};

void generator_if_begin(gen_t *gen, str *left_symbol, char eq, str *right_symbol) {
  stack_push(gen->frame_stack, symtable_new(128));

  intptr_t struct_count = (intptr_t)stack_pop(gen->construct_count_stack);
  stack_push(gen->construct_count_stack, (void *)(struct_count + 1));
  stack_push(gen->construct_count_stack, (void *)0);

  str *new_sub_label = str_new_from("if");
  str_append_int(new_sub_label, struct_count);
  stack_push(gen->label_stack, new_sub_label);

  gen->depth++;

  str *dest = get_dest(gen);

  str *left_symbol_path = get_symbol_path(gen, left_symbol);
  str *right_symbol_path = get_symbol_path(gen, right_symbol);

  str_append_cstr(dest, "# IF START\n");

  if (eq == 'e') {
    str_append_cstr(dest, "JUMPIFNEQ ");
  } else {
    str_append_cstr(dest, "JUMPIFEQ ");
  }

  str_append_str(dest, get_label(gen, -1));
  str_append_cstr(dest, "else ");
  str_append_str(dest, left_symbol_path);
  str_append_cstr(dest, " ");
  str_append_str(dest, right_symbol_path);
  str_append_cstr(dest, "\n");
}

void generator_if_else(gen_t *gen) {
  str *if_label = get_label(gen, -1);

  stack_pop(gen->frame_stack);
  stack_push(gen->frame_stack, symtable_new(128));

  stack_pop(gen->construct_count_stack);
  intptr_t struct_count = (intptr_t)stack_pop(gen->construct_count_stack);
  stack_push(gen->construct_count_stack, (void *)(struct_count + 1));
  stack_push(gen->construct_count_stack, (void *)0);

  stack_pop(gen->label_stack);
  str *new_sub_label = str_new_from("else");
  str_append_int(new_sub_label, struct_count);
  stack_push(gen->label_stack, new_sub_label);

  str *dest = get_dest(gen);
  str *else_label = get_label(gen, -1);

  str_append_cstr(dest, "JUMP ");
  str_append_str(dest, else_label);
  str_append_cstr(dest, "end\n");

  str_append_cstr(dest, "# ELSE START\n");

  str_append_cstr(dest, "LABEL ");
  str_append_str(dest, if_label);
  str_append_cstr(dest, "else\n");
}

void generator_if_end(gen_t *gen) {
  str *label = get_label(gen, -1);
  str *dest = get_dest(gen);

  str_append_cstr(dest, "# IF END\n");

  str_append_cstr(dest, "LABEL ");
  str_append_str(dest, label);
  str_append_cstr(dest, "end\n");

  stack_pop(gen->label_stack);
  stack_pop(gen->frame_stack);
  stack_pop(gen->construct_count_stack);

  gen->depth--;
}
