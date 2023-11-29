#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdbool.h>
#include <stdlib.h>

#include "builtin.h"
#include "error.h"
#include "scanner.h"
#include "symtable.h"

// Only include scanner and generator if not in debug mode, otherwise mock them
// #ifndef PARSER_TEST
// #include "generator.h"
// #endif

typedef struct {
  Symtable *global_table;
  void_stack_t *local_tables_stack;  // Stack of local tables
  // Symtable *local_table;
  Token *token_buffer;
  bool buffer_active;
  bool in_function;
  bool in_scope;

#ifndef PARSER_TEST
  // gen_t *gen;
#else
  Token *input_tokens;
  int input_index;
  Token *output_tokens;
  int output_index;
#endif
} Parser;

bool parser_init(Parser *parser);

void is_valid_statement(Parser *parser);

// Check if current token is of the expected type and advance if it is
bool match(Parser *parser, TokenType token_type, bool check_new_line);

// Check if value of keyword is equal to the expected keyword
bool check_keyword(Parser *parser, KeywordType keyword);

// Advance to next token
Token advance(Parser *parser);

// Returns true if current token is of the expected type
bool check_type(Parser *parser, TokenType token_type);

// Consumes next token, if it is not of the expected type, prints error message and exits
Token consume(Parser *parser, TokenType token_type, char *error_msg);

void return_def(Parser *parser, SymtableItem *item);

void func_params_n(Parser *parser, SymtableItem *item, Symtable *table, void_stack_t *stack);

void func_params(Parser *parser, SymtableItem *item, Symtable *table, void_stack_t *stack);

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def(Parser *parser);

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n(Parser *parser, SymtableItem *item, SymtableParam *param);

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params(Parser *parser, SymtableItem *item, SymtableParam *param);

bool statement(Parser *parser);
bool body(Parser *parser);

void program(Parser *parser);

#ifndef PARSER_TEST
void parse(Parser *parser);
#else
Token *parse(Parser *parser, Token *input_tokens);
#endif

void expression(Parser *parser);

bool return_t(Parser *parser);

void log_token_parsed(Parser *parser);

Token *current_token(Parser *parser);
bool match_peek(TokenType token_type);
Token *peek(Parser *parser);
#endif