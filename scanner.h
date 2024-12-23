#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdbool.h>
#include <stdio.h>

typedef enum {
  NONE,
  KW_IF,
  KW_ELSE,
  KW_WHILE,
  KW_INT,
  KW_FLOAT,
  KW_RETURN,
  KW_VOID,
  KW_NIL,
  KW_LET,
  KW_UNKNOWN,
  KW_VAR,
  KW_IN,
  KW_FOR,
  KW_DOUBLE,
  KW_STRING,
  KW_FUNC,
} KeywordType;

typedef enum {
  TOKEN_KEYWORD,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIV,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_COLON,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_ASSIGN,
  TOKEN_NOT,
  TOKEN_STRING_LITERAL,
  TOKEN_ARROW,
  TOKEN_LTE,
  TOKEN_GTE,
  TOKEN_EQ,
  TOKEN_NE,
  TOKEN_NULL_COALESCING,
  TOKEN_EOF,
  TOKEN_COMMA,
  TOKEN_IDENTIFIER,
  TOKEN_IDENTIFIER_NOT_NULL,
  TOKEN_INTEGER_LITERAL,
  TOKEN_DECIMAL_LITERAL,
  TOKEN_EXPONENT,
  TOKEN_UNKNOWN,
  TOKEN_STACK_BOTTOM,  // used for parsing
  TOKEN_EXPRESSION,    // used for parsing
} TokenType;

typedef struct {
  TokenType type;
  KeywordType keyword;
  char *val;
  int length;
  bool is_nullable;
  bool after_newline;
} Token;

void scanner_init(FILE *file);
void scanner_destroy();
void token_destroy(Token *token);
int get_next_token(Token *token);
void char_to_token(Token *token, char c);
void determine_token_type(Token *token);

#endif  // __SCANNER_H__