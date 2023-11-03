#include "scanner.h"
#include "stdio.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static FILE *f;
#define MAX_STRING_LENGTH 256

int main() {
  FILE *input_file = fopen("test.swift", "r");
  if (input_file == NULL) {
    printf("Error opening input file\n");
    return 1;
  }
  Token token;
  scanner_init(input_file);
  while (get_next_token(&token, f) != TOKEN_EOF) {
  }

  scanner_destroy();

  return 0;
}

void determine_token_type(Token *token, int character) {
  token->type = TOKEN_UNKNOWN;
  if (strcmp(token->val, "if") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_IF;
  } else if (strcmp(token->val, "else") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_ELSE;
  } else if (strcmp(token->val, "while") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_WHILE;
  } else if (strcmp(token->val, "int") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_INT;
  } else if (strcmp(token->val, "float") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_FLOAT;
  } else if (strcmp(token->val, "char") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = TOKEN_CHAR;
  } else if (strcmp(token->val, "return") == 0) {
    token->keyword = TOKEN_RETURN;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "void") == 0) {
    token->keyword = TOKEN_VOID;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "null") == 0) {
    token->keyword = TOKEN_NULL;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "var") == 0) {
    token->keyword = TOKEN_VAR;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "print") == 0) {
    token->keyword = TOKEN_PRINT;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "let") == 0) {
    token->keyword = TOKEN_LET;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "in") == 0) {
    token->keyword = TOKEN_IN;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "for") == 0) {
    token->keyword = TOKEN_FOR;
    token->type = TOKEN_KEYWORD;
  } else if (token->val[0] == '"') {
    token->type = TOKEN_STRING;
    char buffer[MAX_STRING_LENGTH];
    int i = 0;
    while ((character = fgetc(f)) != EOF && character != '"') {
      if (i < MAX_STRING_LENGTH - 1) {
        buffer[i++] = (char)character;
      }
    }
    buffer[i] = '\0';
    token->val = strdup(buffer);
  } else {
    switch (token->val[0]) {

    case '(':
      token->type = TOKEN_LPAREN;

      break;
    case ')':
      token->type = TOKEN_RPAREN;

      break;
    case '{':
      token->type = TOKEN_LBRACE;
      break;
    case '}':
      token->type = TOKEN_RBRACE;
      break;
    case '[':
      token->type = TOKEN_LBRACKET;
      break;
    case ']':
      token->type = TOKEN_RBRACKET;

      break;
    case ';':
      token->type = TOKEN_SEMICOLON;
      break;
    case ',':
      token->type = TOKEN_COMMA;

      break;
    case '+':
      if (token->val[1] == '=') {
        token->type = TOKEN_PLUS_ASSIGN;
      } else {
        token->type = TOKEN_PLUS;
      }
      break;
    case '-':
      if (token->val[1] == '=') {
        token->type = TOKEN_MINUS_ASSIGN;
      } else {
        token->type = TOKEN_MINUS;
      }
      break;
    case '*':
      if (token->val[1] == '=') {
        token->type = TOKEN_MULTIPLY_ASSIGN;

      } else {
        token->type = TOKEN_MULTIPLY;
        ungetc(token->val[1], f);
      }
      break;
    case '/':
      if (token->val[1] == '=') {
        token->type = TOKEN_DIV_ASSIGN;

      } else {
        token->type = TOKEN_DIV;
        ungetc(token->val[1], f);
      }
      break;
    case '%':
      if (token->val[1] == '=') {
        token->type = TOKEN_MOD_ASSIGN;

      } else {
        token->type = TOKEN_MOD;
        ungetc(token->val[1], f);
      }
      break;
    case '<':
      if (token->val[1] == '<' && token->val[2] == '=') {
        token->type = TOKEN_LSHIFT_ASSIGN;
        ungetc(token->val[2], f);
      } else if (token->val[1] == '<') {
        token->type = TOKEN_LSHIFT;
        ungetc(token->val[1], f);
      } else {
        token->type = TOKEN_LT;
        ungetc(token->val[1], f);
      }
      break;
    case '>':
      if (token->val[1] == '>' && token->val[2] == '=') {
        token->type = TOKEN_RSHIFT_ASSIGN;
        ungetc(token->val[2], f);
      } else if (token->val[1] == '>') {
        token->type = TOKEN_RSHIFT;
        ungetc(token->val[1], f);
      } else {
        token->type = TOKEN_GT;
        ungetc(token->val[1], f);
      }
      break;
    case '=':
      if (token->val[1] == '=') {
        token->type = TOKEN_EQ;
      } else {
        token->type = TOKEN_ASSIGN;
      }
      break;
    case '!':
      if (token->val[1] == '=') {
        token->type = TOKEN_NE;

      } else {
        token->type = TOKEN_NOT;
        ungetc(token->val[1], f);
      }
      break;
    case '&':
      if (token->val[1] == '&') {
        token->type = TOKEN_AND;

      } else if (token->val[1] == '=') {
        token->type = TOKEN_AND_ASSIGN;

      } else {
        token->type = TOKEN_BITWISE_AND;
        ungetc(token->val[1], f);
      }
      break;
    case '|':
      if (token->val[1] == '|') {
        token->type = TOKEN_BITWISE_OR;

      } else if (token->val[1] == '=') {
        token->type = TOKEN_OR_ASSIGN;

      } else {
        token->type = TOKEN_OR;
        ungetc(token->val[1], f);
      }
      break;
    case '^':
      if (token->val[1] == '=') {
        token->type = TOKEN_BITWISE_XOR;
      } else {
        token->type = TOKEN_XOR_ASSIGN;
        ungetc(token->val[1], f);
      }
      break;
    case '?':
      if (strcmp(token->val, "??") == 0) {
        token->type = TOKEN_NULL_COALESCING;

      } else {
        token->type = TOKEN_UNKNOWN;
        ungetc(token->val[1], f);
      }
      break;
    // case '? :':
    //   token->type = TOKEN_TERNARY;
    //   ungetc(character, f);
    //   break;
    default:
      token->type = TOKEN_UNKNOWN;
      token->keyword = TOKEN_UNKNOWN_KEYWORD;
      break;
    }
  }
}

void char_to_token(Token *token, char c) {
  char *new_val = realloc(token->val, (token->length + 2) * sizeof(char));
  if (new_val == NULL) {
    printf("Error reallocating memory\n");
    exit(1);
  }
  token->val = new_val;
  token->val[token->length] = c;
  token->val[token->length + 1] = '\0';
  token->length++;
}

int get_next_token(Token *token, FILE *f) {
  token->type = TOKEN_UNKNOWN;
  token->length = 0;
  token->val = NULL;
  char c;
  int i = 0;
  bool inString = false;

  while (true) {
    c = fgetc(f);
    if (c == EOF) {
      break;
    }
    // printf("c: %c\n", c);

    if (inString) {
      if (c == '"') {
        inString = false;
        char_to_token(token, c);
        token->type = TOKEN_STRING;
        break;
      } else {
        char_to_token(token, c);
      }
    } else if (c == '"') {
      inString = true;
      char_to_token(token, c);
    } else if (isalpha(c) || c == '_') { // Identifier
      char_to_token(token, c);
      while (isalnum(c = fgetc(f)) || c == '_') {
        char_to_token(token, c);
      }
      determine_token_type(token, token->val);
      if (token->type == TOKEN_UNKNOWN) {
        token->type = TOKEN_IDENTIFIER;
      }
      break;
    } else if (c == '\n' || c == '\t' || c == ' ') {
      continue;
    } else {
      char_to_token(token, c);
      determine_token_type(token, token->val);
      break;
    }
  }

  if (token->type == TOKEN_UNKNOWN && c == EOF) {
    token->type = TOKEN_EOF;
  }
  printf("Token Type: %d, Token Value: %s\n", token->type, token->val);

  return token->type;
}

void scanner_init(FILE *file) {
  f = file;
  if (f == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
}
void scanner_destroy() {
  if (f != NULL) {
    fclose(f);
  }
}
void token_destroy(Token *token) {
  if (token->val != NULL) {
    free(token->val);
  }
}
