//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "stack.h"
#include "scanner.h"

const char operators[] = {TOKEN_PLUS, TOKEN_IDENTIFIER, TOKEN_STACK_BOTTOM, 'E'};

typedef struct {
    TokenType *rule;
    TokenType result;
} ExtendedToken;

int parse_expresion(char *expressionToParse);

char *charArrayFromStack(Stack *stack);

#endif // _EXP_PARSER_H
