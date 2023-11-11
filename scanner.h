#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>

typedef enum keyword_type
{
    NONE,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_INT,
    KW_FLOAT,
    KW_CHAR,
    KW_RETURN,
    KW_VOID,
    KW_NIL,
    KW_LET,
    KW_UNKNOWN,
    KW_VAR,
    KW_IN,
    KW_FOR,
    KW_DOUBLE,
    KW_FUNC,
} KeywordType;

typedef enum token_type
{
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
    TOKEN_OPTIONAL_TYPE,
    TOKEN_EXPONENT,
    TOKEN_DECIMAL_LITERAL,
    TOKEN_INTEGER_LITERAL,
} TokenType;

typedef struct
{
    TokenType type;
    // TokenData data
    char *val;
    KeywordType keyword;
    int length;
} Token;

// typedef union token_data
// {
//     int num_val;
//     double float_val;
//     bool bool_val;
//     char *str_val;
//     Keyword keyword_type;
// } TokenData;

void scanner_init(FILE *file);
void scanner_destroy();
int get_next_token(Token *token);

#endif // __SCANNER_H__