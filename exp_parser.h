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
#define RULE_MAX_LENGHT 5

typedef enum {
  L,     // < /SHIFT
  R,     // > /REDUCTION
  E,     // = /REDUCTION PREPARE
  X,     // Invalid
  None,  // Useless but send thank you letters to ramsay
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
Token parse_expression(Token expressionToParse[], int inputSize, Parser *parser, void_stack_t *expresionStack);
#else
SymtableIdentifierType parse_expression(Parser *parser, void_stack_t *expresionStack);
#endif

void y_eet(void_stack_t *stack);

#endif  // _EXP_PARSER_H
