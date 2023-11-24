#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

typedef enum
{
    NO_ERROR,
    LEXICAL_ERR,
    SYNTAX_ERR,
    SEMANTIC_ERR_FUNC,
    SEMANTIC_ERR_CALL,
    SEMANTIC_ERR_VAR,
    SEMANTIC_ERR_RETURN,
    SEMANTIC_ERR_EXPR,
    SEMANTIC_ERR_INFER,
    SEMANTIC_ERR,
    INTERNAL_ERROR
} ReturnCode;

void exit_with_error(ReturnCode return_code, char *error_mesage_fmt, ...);
void exit_custom(ReturnCode return_code, const char *fmt, ...);

#endif