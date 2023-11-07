#include "scanner.h"
#include "stdio.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

FILE *source_file;

void scanner_init(FILE *file) {
  source_file = file;
  if (source_file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
}
void scanner_destroy() {
  if (source_file != NULL) {
    fclose(source_file);
  }
}
void token_destroy(Token *token) {
  if (token->val != NULL) {
    free(token->val);
  }
}

void determine_token_type(Token *token) {
  int character;
  token->type = TOKEN_UNKNOWN;
  if (strcmp(token->val, "if") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_IF;
  } else if (strcmp(token->val, "Double") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_DOUBLE;
  } else if (strcmp(token->val, "else") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_ELSE;
  } else if (strcmp(token->val, "while") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_WHILE;
  } else if (strcmp(token->val, "Func") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FUNC;
  } else if (strcmp(token->val, "Int") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_INT;
  } else if (strcmp(token->val, "nil") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_NIL;
  } else if (strcmp(token->val, "return") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_RETURN;
  } else if (strcmp(token->val, "float") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FLOAT;
  } else if (strcmp(token->val, "char") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_CHAR;
  } else if (strcmp(token->val, "return") == 0) {
    token->keyword = KW_RETURN;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "void") == 0) {
    token->keyword = KW_VOID;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "var") == 0) {
    token->keyword = KW_VAR;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "let") == 0) {
    token->keyword = KW_LET;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "in") == 0) {
    token->keyword = KW_IN;
    token->type = TOKEN_KEYWORD;
  } else if (strcmp(token->val, "for") == 0) {
    token->keyword = KW_FOR;
    token->type = TOKEN_KEYWORD;
  } else if (token->val[0] == '"') {
    token->type = TOKEN_STRING;
    char *buffer = NULL;
    int buffer_size = 0;
    int i = 0;
    strcpy(token->val, buffer);

    while ((character = fgetc(source_file)) != EOF && character != '"') {
      if (i >= buffer_size - 1) {
        buffer_size += 64; // Allocate 64 more bytes
        char *new_buffer = (char *)realloc(buffer, buffer_size);
        if (new_buffer == NULL) {
          printf("Error allocating memory for string token\n");
          exit(1);
        }
        buffer = new_buffer;
      }
      buffer[i++] = (char)character;
    }

    buffer[i] = '\0';
    token->val = buffer;
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
      }
      break;
    case '/':
      if (token->val[1] == '=') {
        token->type = TOKEN_DIV_ASSIGN;
      } else {
        token->type = TOKEN_DIV;
      }
      break;
    case '%':
      if (token->val[1] == '=') {
        token->type = TOKEN_MOD_ASSIGN;
      } else {
        token->type = TOKEN_MOD;
      }
      break;
    case '<':
      if (token->val[1] == '<' && token->val[2] == '=') {
        token->type = TOKEN_LSHIFT_ASSIGN;
      } else if (token->val[1] == '<') {
        token->type = TOKEN_LSHIFT;
      } else {
        token->type = TOKEN_LT;
      }
      break;
    case '>':
      if (token->val[1] == '>' && token->val[2] == '=') {
        token->type = TOKEN_RSHIFT_ASSIGN;
      } else if (token->val[1] == '>') {
        token->type = TOKEN_RSHIFT;
      } else {
        token->type = TOKEN_GT;
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
      }
      break;
    case '&':
      if (token->val[1] == '&') {
        token->type = TOKEN_AND;
      } else if (token->val[1] == '=') {
        token->type = TOKEN_AND_ASSIGN;
      } else {
        token->type = TOKEN_BITWISE_AND;
      }
      break;
    case '|':
      if (token->val[1] == '|') {
        token->type = TOKEN_BITWISE_OR;
      } else if (token->val[1] == '=') {
        token->type = TOKEN_OR_ASSIGN;
      } else {
        token->type = TOKEN_OR;
      }
      break;
    case '^':
      if (token->val[1] == '=') {
        token->type = TOKEN_BITWISE_XOR;
      } else {
        token->type = TOKEN_XOR_ASSIGN;
      }
      break;
    case '?':
      if (strcmp(token->val, "??") == 0) {
        token->type = TOKEN_NULL_COALESCING;
      } else {
        token->type = TOKEN_OPTIONAL_TYPE;
      }
      break;
    // case '? :':
    //   token->type = TOKEN_TERNARY;
    //   ungetc(character, f);
    //   break;
    default:
      token->type = TOKEN_UNKNOWN;
      token->keyword = KW_UNKNOWN;
      break;
    }
  }
}

void char_to_token(Token *token, char c) {
  char *new_val;
  if (token->val == NULL) {
    new_val = (char *)malloc(2 * sizeof(char));
    if (new_val == NULL) {
      printf("Error allocating memory for token value\n");
      exit(1);
    }
    new_val[0] = c;
    new_val[1] = '\0';
  } else {
    new_val = (char *)realloc(token->val, (token->length + 2) * sizeof(char));
    if (new_val == NULL) {
      printf("Error reallocating memory for token value\n");
      exit(1);
    }
    new_val[token->length] = c;
    new_val[token->length + 1] = '\0';
  }

  token->val = new_val;
  token->length++;
}

int get_next_token(Token *token) {
  int nested_block_comment = 0;
  token->type = TOKEN_UNKNOWN;
  token->length = 0;
  token->val = NULL;
  char c;
  bool inString = false;

  while (true) {
    c = fgetc(source_file);
    if (c == EOF) {
      break;
    }
    if (isdigit(c)) {
      char_to_token(token, c);
      while (isdigit(c = fgetc(source_file))) {
        char_to_token(token, c);
      }
      ungetc(c, source_file);
      token->type = TOKEN_INTEGER_LITERAL;
      break;
    }
    if (c == '.') {
      char_to_token(token, c);
      while (isdigit(c = fgetc(source_file))) {
        char_to_token(token, c);
      }
      if (c == 'e' || c == 'E') {
        char_to_token(token, c);
        if ((c = fgetc(source_file)) == '+' || c == '-') {
          char_to_token(token, c);
        } else {
          ungetc(c, source_file);
        }
        while (isdigit(c = fgetc(source_file))) {
          char_to_token(token, c);
        }
        token->type = TOKEN_EXPONENT;
      } else {
        token->type = TOKEN_DECIMAL_LITERAL;
      }
      break;
    }
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
      while (isalnum(c = fgetc(source_file)) || c == '_') {
        char_to_token(token, c);
      }
      ungetc(c, source_file);
      determine_token_type(token);
      if (token->type == TOKEN_UNKNOWN) {
        token->type = TOKEN_IDENTIFIER;
      }
      break;
    } else if (c == '\n' || c == '\t' || c == ' ' || c == '\r') {
      continue;
    } else if (c == '/') {
      c = fgetc(source_file);
      if (c == '/') {
        while (fgetc(source_file) != '\n')
          ;
        continue;
      } else if (c == '*') {
        if (nested_block_comment > 0) {
          goto loop;
        } else {
          nested_block_comment++;
        loop:
          while (fgetc(source_file) != '*')
            ;
          if (fgetc(source_file) == '/') {
            if (nested_block_comment > 0) {
              nested_block_comment--;
              goto loop;
            }
            continue;
          } else {
            goto loop;
          }
        }
      } else {
        ungetc(c, source_file);
        char_to_token(token, '/');
        determine_token_type(token);
        break;
      }
    } else {
      char_to_token(token, c);
      determine_token_type(token);
      break;
    }
  }

  if (token->type == TOKEN_UNKNOWN && c == EOF) {
    token->type = TOKEN_EOF;
  }
  // printf("Token Type: %d, Token Value: %s\n", token->type, token->val);

  return token->type;
}