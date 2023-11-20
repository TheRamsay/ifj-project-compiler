#include "scanner.h"
#include "stdio.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

FILE *source_file;

void scanner_init(FILE *file) { // Initialize the scanner
  source_file = file;           // Set the source file to the file passed in
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
  int character; // The current character being read
  token->type = TOKEN_UNKNOWN;
  token->keyword = KW_UNKNOWN;
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
  } else if (strcmp(token->val, "func") == 0) {
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
  } else if (strcmp(token->val, "String") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_STRING;
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
  } else if (token->val[0] == '"') { // String literal
    token->type = TOKEN_STRING_LITERAL;
    char *buffer = NULL; // The buffer to store the string literal

    int buffer_size = 0;
    int i = 0;

    while ((character = fgetc(source_file)) != EOF && character != '"') { // Read until the end of the string literal
      if (i >= buffer_size - 1) {                                         // If the buffer is full, reallocate it
        buffer_size += 64;                                                // Allocate 64 more bytes
        char *new_buffer = (char *)realloc(buffer, buffer_size);          // Reallocate the buffer
        if (new_buffer == NULL) {
          printf("Error allocating memory for string token\n");
          exit(1);
        }
        buffer = new_buffer;
      }
      buffer[i++] = (char)character; // Add the character to the buffer
    }
    buffer[i] = '\0';
    strcpy(token->val, buffer); // Copy the string literal into the buffer
    free(buffer);               // Free the buffer
  } else {
    char c, k;
    switch (token->val[0]) { // Determine the token type based on the first character
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
    case ':':
      token->type = TOKEN_COLON;
      break;
    case '+':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_PLUS_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_PLUS;
      }
      break;
    case '-':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_MINUS_ASSIGN;
      } else if (c == '>') {
        token->val[1] = '>';
        token->type = TOKEN_ARROW;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_MINUS;
      }
      break;
    case '*':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_MULTIPLY_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_MULTIPLY;
      }
      break;
    case '/':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_DIV_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_DIV;
      }
      break;
    case '%':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_MOD_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_MOD;
      }
      break;
    case '<':
      c = fgetc(source_file);
      k = fgetc(source_file);
      if (c == '<' && k == '=') {
        token->val[1] = '<';
        token->val[2] = '=';
        token->type = TOKEN_LSHIFT_ASSIGN;
      } else if (token->val[1] == '<') {
        ungetc(k, source_file);
        token->val[1] = '<';
        token->type = TOKEN_LSHIFT;
      } else {
        ungetc(k, source_file);
        ungetc(c, source_file);
        token->type = TOKEN_LT;
      }
      break;
    case '>':
      c = fgetc(source_file);
      k = fgetc(source_file);
      if (c == '>' && k == '=') {
        token->val[1] = '>';
        token->val[2] = '=';
        token->type = TOKEN_RSHIFT_ASSIGN;
      } else if (c == '>') {
        ungetc(k, source_file);
        token->val[1] = '>';
        token->type = TOKEN_RSHIFT;
      } else {
        ungetc(k, source_file);
        ungetc(c, source_file);
        token->type = TOKEN_GT;
      }
      break;
    case '=':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_EQ;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_ASSIGN;
      }
      break;
    case '!':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_NE;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_NOT;
      }
      break;
    case '&':
      c = fgetc(source_file);
      if (c == '&') {
        token->val[1] = '&';
        token->type = TOKEN_AND;
      } else if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_AND_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_BITWISE_AND;
      }
      break;
    case '|':
      c = fgetc(source_file);
      if (c == '|') {
        token->val[1] = '|';
        token->type = TOKEN_BITWISE_OR;
      } else if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_OR_ASSIGN;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_OR;
      }
      break;
    case '^':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_BITWISE_XOR;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_XOR_ASSIGN;
      }
      break;
    case '?':
      c = fgetc(source_file);
      if (c == '?') {
        token->val[1] = '?';
        token->type = TOKEN_NULL_COALESCING;
      } else {
        ungetc(c, source_file);
        token->type = TOKEN_OPTIONAL_TYPE;
      }
      break;
    default:
      token->type = TOKEN_UNKNOWN;
      token->keyword = KW_UNKNOWN;
      break;
    }
  }
}

void char_to_token(Token *token, char c) {
  char *new_val; // The new value of the token
  if (token->val == NULL) {
    new_val = (char *)malloc(2 * sizeof(char)); // Allocate memory for the token value
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
    c = fgetc(source_file); // Get the next character from the source file
    if (c == EOF) {
      break;
    }

    if (isdigit(c)) {                           // Integer or decimal literal
      char_to_token(token, c);                  // Add the character to the token value
      while (isdigit(c = fgetc(source_file))) { // Read until the end of the literal
        char_to_token(token, c);
      }
      ungetc(c, source_file);              // Put the last character back into the source file
      token->type = TOKEN_INTEGER_LITERAL; // Set the token type to integer literal
      break;
    }
    if (c == '.') { // Decimal literal
      char_to_token(token, c);
      while (isdigit(c = fgetc(source_file))) {
        char_to_token(token, c);
      }
      if (c == 'e' || c == 'E') { // Exponent
        char_to_token(token, c);
        if ((c = fgetc(source_file)) == '+' || c == '-') { // Exponent sign
          char_to_token(token, c);
        } else {
          ungetc(c, source_file); // Put the character back into the source file
        }
        while (isdigit(c = fgetc(source_file))) { // Exponent value
          char_to_token(token, c);
        }
        token->type = TOKEN_EXPONENT; // Set the token type to exponent
      } else {
        token->type = TOKEN_DECIMAL_LITERAL;
      }
      break;
    }
    if (inString) {   // String literal
      if (c == '"') { // End of string literal
        inString = false;
        char_to_token(token, c);
        token->type = TOKEN_STRING_LITERAL;
        break;
      } else {
        char_to_token(token, c); // Add the character to the token value
      }
    } else if (c == '"') { // Start of string literal
      inString = true;
      char_to_token(token, c);
    } else if (isalpha(c) || c == '_') { // Identifier
      char_to_token(token, c);
      while (isalnum(c = fgetc(source_file)) || c == '_') { // Read until the end of the identifier
        char_to_token(token, c);
      }
      ungetc(c, source_file);
      determine_token_type(token);
      if (token->type == TOKEN_UNKNOWN) {
        token->type = TOKEN_IDENTIFIER;
      }
      break;
    } else if (c == '\n' || c == '\t' || c == ' ' || c == '\r') { // Whitespace
      continue;                                                   // Skip the whitespace
    } else if (c == '/') {                                        // Comment
      c = fgetc(source_file);                                     // Get the next character
      if (c == '/') {
        c = fgetc(source_file);
        while (c != '\n' && c != EOF) {
          c = fgetc(source_file);
        }; // Read until the end of the line
        continue;
      } else if (c == '*') { // Block comment
        if (nested_block_comment > 0) {
          goto loop; // If we are in a nested block comment, skip the comment
        } else {
          nested_block_comment++; // Otherwise, increment the nested block comment counter
        loop:
          while (fgetc(source_file) != '*') // Read until the end of the block comment
            ;
          if (fgetc(source_file) == '/') { // If the block comment is over, decrement the nested block comment counter
            if (nested_block_comment > 0) {
              nested_block_comment--;
              goto loop;
            }
            continue;
          } else {
            goto loop; // Otherwise, continue reading the block comment
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
  } else if (token->type == TOKEN_UNKNOWN) {
    fprintf(stderr, "Unknown token: %s\n", token->val);
    exit(1);
  }
  // printf("Token Type: %d, Token Value: %s\n", token->type, token->val);
  return token->type;
}
