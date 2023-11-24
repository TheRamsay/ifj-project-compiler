/* ******************************* str.h ************************************ */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#ifndef _STR_H
#define _STR_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct str {
  unsigned int alloc_size;
  char *data;
} str;

str *str_new(unsigned int length);

str *str_new_from_str(str *s);

str *str_new_from_cstr(const char *s);

char *str_to_cstr(str *s);

bool str_set_cstr(str *s, const char *c);

bool str_set_str(str *s, str *c);

bool str_append_cstr(str *s, const char *c);

bool str_append_str(str *s, str *c);

bool str_append_int(str *s, int c);

bool str_append_cstr_dispose(str *s, char **c);

bool str_append_str_dispose(str *s, str **c);

void str_dispose(str *s);

#endif