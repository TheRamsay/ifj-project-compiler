#include "error.h"

const char *error_msgs[] = {[NO_ERROR] = "No error", [LEXICAL_ERR] = "Lexical error", [SYNTAX_ERR] = "Syntax error", [SEMANTIC_ERR_FUNC] = "Semantic error: undefined function, redefined variable", [SEMANTIC_ERR_CALL] = "Semantic error: wrong number of arguments, wrong argument type, wrong return type", [SEMANTIC_ERR_VAR] = "Semantic error: usage of undefined or uninitialized variable", [SEMANTIC_ERR_RETURN] = "Semantic error: error in return statement", [SEMANTIC_ERR_EXPR] = "Semantic error: type error in expression", [SEMANTIC_ERR_INFER] = "Semantic error: type inference error", [SEMANTIC_ERR] = "Semantic error", [INTERNAL_ERROR] = "Internal error: memory allocation error"};

void exit_with_error(ReturnCode return_code, char *error_mesage_fmt, ...) {
  if (error_mesage_fmt == NULL) {
    fprintf(stderr, "%s\n", error_msgs[return_code]);
  } else {
    va_list args;
    va_start(args, error_mesage_fmt);
    fprintf(stderr, "%s (Error message: ", error_msgs[return_code]);
    vfprintf(stderr, error_mesage_fmt, args);
    fprintf(stderr, ")\n");
    va_end(args);
  }

  exit(return_code);
}

void exit_custom(ReturnCode return_code, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(return_code);
}