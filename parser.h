#ifndef __PARSER_H__
#define __PARSER_H__

#include "scanner.h"
#include <stdbool.h>
#include <stdlib.h>

// Check if current token is of the expected type and advance if it is
bool match(TokenType token_type);

// Check if value of keyword is equal to the expected keyword
bool check_keyword(KeywordType keyword);

// Advance to next token
void advance();

// Returns true if current token is of the expected type
bool check_type(TokenType token_type);

// Consumes next token, if it is not of the expected type, prints error message and exits
void consume(TokenType token_type, char *error_msg);

void return_def();

void func_params_n();

void func_params();

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def();

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n();

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params();

void statement();
void body();

void program();

void parse();
void expression();

void return_t();

Token *current_token();
bool match_peek(TokenType token_type);
Token *peek();
#endif