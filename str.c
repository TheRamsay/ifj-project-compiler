/* ******************************* str.c ************************************ */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#include "str.h"

#include <stdlib.h>
#include <string.h>

/*                            Private functions                               */
/**
 *
 * @brief Resizes the string to the given size.
 *
 * @param s The string.
 * @param new_size The new size of the string.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_resize(str *s, unsigned int new_size) {
  if (s == NULL || new_size <= s->alloc_size) {
    return NULL;
  }

  char *data_prev = s->data;

  s->data = realloc(s->data, new_size);

  if (s->data == NULL) {
    s->data = data_prev;
    return false;
  }

  s->alloc_size = new_size;
  return true;
}

/*                             Public functions                               */
/**
 *
 * @brief Creates a new string with the given length.
 *
 * @param length The length of the string.
 * @returns Pointer to the new string. NULL if the allocation failed.
 *
 */
str *str_new(unsigned int length) {
  if (length == 0) {
    return NULL;
  }

  str *str = malloc(sizeof(struct str));

  if (str == NULL) {
    return NULL;
  }

  str->alloc_size = length;
  str->data = malloc(sizeof(char) * length);

  if (str->data == NULL) {
    free(str);
    return NULL;
  }

  str->data[0] = '\0';

  return str;
}

/**
 *
 * @brief Creates a new string from the given C string.
 *
 * @param s The C string.
 * @returns Pointer to the new string.
 *
 */
str *str_new_from_str(str *s) {
  if (s == NULL) {
    return NULL;
  }

  str *str = str_new(s->alloc_size);

  if (str == NULL) {
    str_dispose(str);
    return NULL;
  }

  strcpy(str->data, s->data);

  return str;
}

/**
 *
 * @brief Creates a new string from the given C string.
 *
 * @param s The C string.
 * @returns Pointer to the new string.
 *
 */
str *str_new_from_cstr(const char *s) {
  if (s == NULL) {
    return NULL;
  }

  str *str = str_new(strlen(s) + 1);

  if (str == NULL) {
    str_dispose(str);
    return NULL;
  }

  strcpy(str->data, s);

  return str;
}

/**
 *
 * @brief Creates a string containing a number constant in the appropriate
 * format.
 *
 * @param s C string containing a number.
 * @returns Pointer to the new string.
 *
 */
str *str_new_int_const(const char *s) {
  str *str = str_new_from_cstr("int@");
  str_append_cstr(str, s);

  return str;
}

/**
 *
 * @brief Creates a string containing a float constant in the appropriate
 * format.
 *
 * @param s C string containing a float.
 * @returns Pointer to the new string.
 *
 */
str *str_new_float_const(const char *s) {
  str *str = str_new(128);

  str_append_cstr(str, "float@");
  str_append_cstr(str, s);

  float flt;
  sscanf(s, "%a", &flt);
  snprintf(str->data, str->alloc_size, "%a", flt);

  return str;
}

/**
 *
 * @brief Creates a string containing a string constant in the appropriate format.
 *
 * @param s C string containing a string.
 * @returns Pointer to the new string.
 *
 */
str *str_new_string_const(const char *s) {
  str *str = str_new_from_cstr("string@");
  str_append_cstr(str, s);

  return str;
}

/**
 *
 * @brief Creates a string containing a nil constant in the appropriate format.
 *
 * @returns Pointer to the new string.
 *
 */
str *str_new_nil_const() { return str_new_from_cstr("nil@nil"); }

/**
 * @brief Converts the string to a new C string.
 *
 * @param s The string.
 * @returns Pointer to the new C string.
 */
char *str_to_cstr(str *s) {
  if (s == NULL) {
    return NULL;
  }

  char *cstr = malloc(sizeof(char) * strlen(s->data) + 1);

  if (cstr == NULL) {
    return NULL;
  }

  strcpy(cstr, s->data);

  return cstr;
}

/**
 *
 * @brief Sets the string to the given C string.
 *
 * @param s The string.
 * @param c The C string.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_set_cstr(str *s, const char *c) {
  if (s == NULL || c == NULL) {
    return false;
  }

  if (strlen(c) + 1 > s->alloc_size) {
    if (!str_resize(s, strlen(c) + 1)) {
      return false;
    }
  }

  strcpy(s->data, c);

  return true;
}

/**
 *
 * @brief Sets the string to the given string.
 *
 * @param s The string.
 * @param c The string.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_set_str(str *s, str *c) {
  if (s == NULL || c == NULL) {
    return false;
  }

  if (strlen(c->data) + 1 > s->alloc_size) {
    if (!str_resize(s, strlen(c->data) + 1)) {
      return false;
    }
  }

  strcpy(s->data, c->data);

  return true;
}

/**
 *
 * @brief Appends the given C string to the string.
 *
 * @param s The string.
 * @param c The C string.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_append_cstr(str *s, const char *c) {
  if (s == NULL) {
    return false;
  } else if (c == NULL) {
    return s;
  }

  if (strlen(s->data) + strlen(c) + 1 > s->alloc_size) {
    if (!str_resize(s, strlen(s->data) + strlen(c) + 1)) {
      return false;
    }
  }

  strcat(s->data, c);

  return true;
}

/**
 *
 * @brief Appends the given string to the string.
 *
 * @param s The string.
 * @param c The string.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_append_str(str *s, str *c) {
  if (s == NULL || c == NULL) {
    return false;
  }

  if (strlen(s->data) + strlen(c->data) + 1 > s->alloc_size) {
    if (!str_resize(s, strlen(s->data) + strlen(c->data) + 1)) {
      return false;
    }
  }

  strcat(s->data, c->data);

  return true;
}

/**
 *
 * @brief Appends the given number to the string.
 *
 * @param s The string.
 * @param c The number.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_append_int(str *s, int c) {
  if (s == NULL) {
    return false;
  }

  str *str = str_new(12);
  sprintf(str->data, "%d", c);
  str_append_str_dispose(s, &str);

  return true;
}

/**
 *
 * @brief Appends the given C string to the string and destroys the C string.
 *
 * @param s The string to append to.
 * @param c The C string to append and destroy.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_append_cstr_dispose(str *s, char **c) {
  if (s == NULL || c == NULL || *c == NULL) {
    return false;
  }

  if (!str_append_cstr(s, *c)) {
    return false;
  }

  free(*c);
  *c = NULL;

  return true;
}

/**
 *
 * @brief Appends the given string to the string and destroys the second string.
 *
 * @param s The string to append to.
 * @param c The string to append and destroy.
 * @returns True if the operation was successful, false otherwise.
 *
 */
bool str_append_str_dispose(str *s, str **c) {
  if (s == NULL || c == NULL || *c == NULL) {
    return false;
  }

  if (!str_append_str(s, *c)) {
    return false;
  }

  str_dispose(*c);
  *c = NULL;

  return true;
}

/**
 *
 * @brief Destroys the given string.
 *
 * @param s The string.
 *
 */
void str_dispose(str *s) {
  if (s == NULL) {
    return;
  }

  if (s->data != NULL) {
    free(s->data);
  }

  free(s);
}