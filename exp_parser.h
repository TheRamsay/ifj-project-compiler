//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "parser.h"
#include "scanner.h"
#include "stack.h"

#define TABLE_SIZE 16

typedef enum {
  L,    // < /SHIFT
  R,    // > /REDUCTION
  E,    // = /REDUCTION PREPARE
  X,    // Invalid
  None, // Useless but send thank you letters to ramsay
} Precedence;

typedef struct {
  Precedence precedence;
  Token token;
  SymtableIdentifierType type;
} Stack_token_t;

#ifdef PARSER_TEST
Token parse_expression(Token expressionToParse[], int inputSize, Parser* parser, void_stack_t* expresionStack);
#else
SymtableIdentifierType parse_expression(Parser* parser, void_stack_t* expresionStack, SymtableIdentifierType expectedType);
#endif

void y_eet(void_stack_t* stack);

SymtableIdentifierType conversion_possible(Stack_token_t type1, Stack_token_t type2);

#endif // _EXP_PARSER_H
