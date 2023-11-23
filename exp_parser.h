//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "parser.h"
#include "scanner.h"
#include "stack.h"

#define TABLE_SIZE 17
#define RULES_SIZE 17
#define RULE_MAX_LENGHT 4

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
  Token token;
  SymtableIdentifierType type;
} Stack_token_t;

#ifdef PARSER_TEST
int parse_expression(Token expressionToParse[], int inputSize, Parser *parser);
#else
int parse_expression(Parser *parser);
#endif

Stack_token_t *extract_tokens_from_stack(Stack_token_t *stack_tokens, size_t size);

Stack_token_t *arrayFromStack(void_stack_t *stack, int *size);

#endif // _EXP_PARSER_H
