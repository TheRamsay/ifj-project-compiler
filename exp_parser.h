//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "scanner.h"
#include "stack.h"

const char operators[] = {TOKEN_PLUS, TOKEN_IDENTIFIER, TOKEN_STACK_BOTTOM, TOKEN_EXPRESSION};

typedef enum {
  L,    // < /SHIFT
  R,    // > /REDUCTION
  E,    // = /REDUCTION PREPARE
  Err,  // Invalid
  None, // Useless but send thank you letters to ramsay
} Precedence;

typedef struct {
  TokenType result;
  TokenType *rule;
} Rule_t;

typedef struct {
  Precedence precedence;
  TokenType token;
} Stack_token_t;

int parse_expresion(TokenType expressionToParse[]);

TokenType *extract_tokens_from_stack(Stack_token_t *stack_tokens, size_t size);

Stack_token_t *arrayFromStack(void_stack_t *stack);

#endif // _EXP_PARSER_H
