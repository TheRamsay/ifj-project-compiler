//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "scanner.h"
#include "stack.h"

#define TABLE_SIZE 17
#define RULES_SIZE 15
#define RULE_MAX_LENGHT 3

const char operators[] = {TOKEN_PLUS, TOKEN_IDENTIFIER, TOKEN_STACK_BOTTOM, TOKEN_EXPRESSION};

typedef enum {
  L,    // < /SHIFT
  R,    // > /REDUCTION
  E,    // = /REDUCTION PREPARE
  X,    // Invalid
  None, // Useless but send thank you letters to ramsay
} Precedence;

typedef struct {
  TokenType result;
  int lenght;
  TokenType rule[RULE_MAX_LENGHT];
} Rule_t;

typedef struct {
  Precedence precedence;
  TokenType token;
} Stack_token_t;

int parse_expression(TokenType expressionToParse[], int inputSize);

TokenType *extract_tokens_from_stack(Stack_token_t *stack_tokens, size_t size);

Stack_token_t *arrayFromStack(void_stack_t *stack);

#endif // _EXP_PARSER_H
