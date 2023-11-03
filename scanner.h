#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>

typedef enum {
  NONE,
  KW_IF,
  KW_ELSE,
  KW_WHILE,
  KW_INT,
  KW_FLOAT,
  KW_CHAR,
  KW_RETURN,
  KW_VOID,
  KW_NULL,
  KW_LET,
  KW_UNKNOWN,
  KW_VAR,
  KW_PRINT,
  KW_IN,
  KW_FOR,
} Keyword;

typedef enum {
  TOKEN_KEYWORD,
  TOKEN_STRING,
  TOKEN_IDENTIFIER,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_SEMICOLON,
  TOKEN_COMMA,
  TOKEN_ASSIGN,
  TOKEN_EQ,
  TOKEN_NEQ,
  TOKEN_LT,
  TOKEN_LE,
  TOKEN_GT,
  TOKEN_GE,
  TOKEN_NE,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIV,
  TOKEN_MOD,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_PLUS_ASSIGN,
  TOKEN_MINUS_ASSIGN,
  TOKEN_MULTIPLY_ASSIGN,
  TOKEN_DIV_ASSIGN,
  TOKEN_MOD_ASSIGN,
  TOKEN_AND_ASSIGN,
  TOKEN_OR_ASSIGN,
  TOKEN_NOT_ASSIGN,
  TOKEN_XOR_ASSIGN,
  TOKEN_LSHIFT_ASSIGN,
  TOKEN_RSHIFT_ASSIGN,
  TOKEN_LSHIFT,
  TOKEN_RSHIFT,
  TOKEN_BITWISE_AND,
  TOKEN_BITWISE_OR,
  TOKEN_BITWISE_XOR,
  TOKEN_BITWISE_NOT,
  TOKEN_NULL_COALESCING,
  TOKEN_TERNARY,
  TOKEN_VAR,
  TOKEN_EOF,
  TOKEN_UNKNOWN,
  TOKEN_PRINT,
  TOKEN_IN,
  TOKEN_FOR,
} TokenType;

typedef struct {
  TokenType type;
  Keyword keyword;
  char *val;
  int length;
} Token;

void scanner_init(FILE *file);
void scanner_destroy();
int get_next_token(Token *token);

#endif // __SCANNER_H__