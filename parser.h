#ifndef __PARSER_H__
#define __PARSER_H__

#include "scanner.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    Symtable *global_table;
    Symtable *local_table;
    Token *token_buffer;
    bool buffer_active;
} Parser;

bool parser_init(Parser *parser);

// Check if current token is of the expected type and advance if it is
bool match(Parser *parser, TokenType token_type);

// Check if value of keyword is equal to the expected keyword
bool check_keyword(Parser *parser, KeywordType keyword);

// Advance to next token
void advance(Parser *parser);

// Returns true if current token is of the expected type
bool check_type(Parser *parser, TokenType token_type);

// Consumes next token, if it is not of the expected type, prints error message and exits
Token consume(Parser *parser, TokenType token_type, char *error_msg);

void return_def(Parser *parser, SymtableItem *item);

void func_params_n(Parser *parser, SymtableItem *item);

void func_params(Parser *parser, SymtableItem *item);

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def(Parser *parser);

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n(Parser *parser);

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params(Parser *parser);

void statement(Parser *parser);
void body(Parser *parser);

void program(Parser *parser);

void parse(Parser *parser);
void expression(Parser *parser);

void return_t(Parser *parser);

Token *current_token(Parser *parser);
bool match_peek(TokenType token_type);
Token *peek(Parser *parser);
#endif