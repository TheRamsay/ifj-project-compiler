#include "scanner.h"
#include "error.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* source_file;
int token_count = 0;

void scanner_init(FILE* file) { // Initialize the scanner
  source_file = file;           // Set the source file to the file passed in
  if (source_file == NULL) {
    fprintf(stderr, "Error opening file\n");
    exit(1);
  }
}
void scanner_destroy() {
  if (source_file != NULL) {
    fclose(source_file);
  }
}
void token_destroy(Token* token) {
  if (token->val != NULL) {
    free(token->val);
  }
}

void determine_token_type(Token* token) {
  int character; // The current character being read
  token->is_nullable = false;
  token->type = TOKEN_UNKNOWN;
  token->keyword = KW_UNKNOWN;
  if (strcmp(token->val, "if") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_IF;
  }
  else if (strcmp(token->val, "Double") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_DOUBLE;
    char c = fgetc(source_file);
    if (c == '?') {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "else") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_ELSE;
  }
  else if (strcmp(token->val, "while") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_WHILE;
  }
  else if (strcmp(token->val, "func") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FUNC;
  }
  else if (strcmp(token->val, "Int") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_INT;
    char c = fgetc(source_file);
    if (c == '?') {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "nil") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_NIL;
  }
  else if (strcmp(token->val, "return") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_RETURN;
  }
  else if (strcmp(token->val, "float") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FLOAT;
    char c = fgetc(source_file);
    if (c == '?') {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "String") == 0) {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_STRING;
    char c = fgetc(source_file);
    if (c == '?') {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "return") == 0) {
    token->keyword = KW_RETURN;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "void") == 0) {
    token->keyword = KW_VOID;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "var") == 0) {
    token->keyword = KW_VAR;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "let") == 0) {
    token->keyword = KW_LET;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "in") == 0) {
    token->keyword = KW_IN;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "for") == 0) {
    token->keyword = KW_FOR;
    token->type = TOKEN_KEYWORD;
  }
  else if (token->val[0] == '"') { // String literal
    token->type = TOKEN_STRING_LITERAL;
    char* buffer = NULL; // The buffer to store the string literal

    int buffer_size = 0;
    int i = 0;

    while ((character = fgetc(source_file)) != EOF && character != '"') { // Read until the end of the string literal
      if (i >= buffer_size - 1) {                                         // If the buffer is full, reallocate it
        buffer_size += 64;                                                // Allocate 64 more bytes
        char* new_buffer = (char*)realloc(buffer, buffer_size);          // Reallocate the buffer
        if (new_buffer == NULL) {
          fprintf(stderr, "Error allocating memory for string token\n");
          exit(99);
        }
        buffer = new_buffer;
      }
      buffer[i++] = (char)character; // Add the character to the buffer
    }
    buffer[i] = '\0';
    strcpy(token->val, buffer); // Copy the string literal into the buffer
    free(buffer);               // Free the buffer
  }
  else {
    char c;
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
    case ',':
      token->type = TOKEN_COMMA;
      break;
    case ':':
      token->type = TOKEN_COLON;
      break;
    case '+':
      token->type = TOKEN_PLUS;
      break;
    case '*':
      token->type = TOKEN_MULTIPLY;
      break;
    case '/':
      token->type = TOKEN_DIV;
      break;
    case '-':
      c = fgetc(source_file);
      if (c == '>') {
        token->val[1] = '>';
        token->type = TOKEN_ARROW;
      }
      else {
        ungetc(c, source_file);
        token->type = TOKEN_MINUS;
      }
      break;
    case '<':
      c = fgetc(source_file);
       if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_LTE;
      }
      else {
        ungetc(c, source_file);
        token->type = TOKEN_LT;
      }
      break;
      break;
    case '>':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_GTE;
      }
      else {
        ungetc(c, source_file);
        token->type = TOKEN_GT;
      }
      break;
    case '=':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_EQ;
      }
      else {
        ungetc(c, source_file);
        token->type = TOKEN_ASSIGN;
      }
      break;
    case '!':
      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_NE;
      }
      else {
        ungetc(c, source_file);
        token->type = TOKEN_IDENTIFIER_NOT_NULL;
      }
      break;
    case '?':
      c = fgetc(source_file);
      if (c == '?') {
        token->val[1] = '?';
        token->type = TOKEN_NULL_COALESCING;
      }
      break;
    default:
      token->type = TOKEN_UNKNOWN;
      token->keyword = KW_UNKNOWN;
      break;
    }
  }
}

void char_to_token(Token* token, char c) {
  char* new_val; // The new value of the token
  if (token->val == NULL) {
    new_val = (char*)malloc(2 * sizeof(char)); // Allocate memory for the token value
    if (new_val == NULL) {
      fprintf(stderr, "Error allocating memory for token value\n");
      exit(99);
    }
    new_val[0] = c; // Set the first character of the token value
    new_val[1] = '\0';
  }
  else {
    new_val = (char*)realloc(token->val, (token->length + 2) * sizeof(char));
    if (new_val == NULL) {
      fprintf(stderr, "Error reallocating memory for token value\n");
      exit(99);
    }
    new_val[token->length] = c;
    new_val[token->length + 1] = '\0';
  }

  token->val = new_val;
  token->length++;
}

int get_next_token(Token* token) {
  int nested_block_comment = 0;
  token->type = TOKEN_UNKNOWN;
  token->length = 0;
  token->val = NULL;
  char c, k, j;
  bool inString = false;
  bool newline = false;
  bool multiline = false;
  bool multiline_string = false;
  int next_char;
  while (true) {

    c = fgetc(source_file); // Get the next character from the source file
    if (c == EOF) {
      if (inString) {
        fprintf(stderr, "Invalid string literal: %s\n", token->val);
        exit(1);
      }
      break;
    }

    if (newline && c != '\t' && c != ' ' && c != '\r') {
      token->after_newline = true; // Set the after_newline flag to true
      newline = false;             // Reset the newline flag
    }
    else if (!token_count && !newline) {
      token->after_newline = true;
    }
    else {
      token->after_newline = false;
    }
    token_count++;

    if (c == '\n') {
      if (inString && !multiline_string) {
        fprintf(stderr, "Invalid string literal: %s\n", token->val);
        exit(1);
      }
      if (multiline) {
        char_to_token(token, '\\');
        char_to_token(token, '0');
        char_to_token(token, '1');
        char_to_token(token, '0');
      }
      else {
        newline = true;
      }
      continue;
    }

    if (c == '_' && inString != true) { // Identifier
      char_to_token(token, c);
      c = fgetc(source_file);
      if (!isalpha(c) && !isdigit(c) && c != ' ' && c != EOF && c != '=' && c != ':') {
        fprintf(stderr, "Invalid identifier: %s\n", token->val);
        exit(1);
      }

      if (c == EOF || c == ' ' || c == '\n' || c == '=' || c == ':') {
        token->type = TOKEN_IDENTIFIER;
        ungetc(c, source_file);
        break;
      }
    }

    if (isdigit(c) && inString != true) {
      char_to_token(token, c);                  // Add the character to the token value
      while (isdigit(c = fgetc(source_file))) { // Read until the end of the literal
        char_to_token(token, c);
      }
      if (c == 'e' || c == 'E') { // Exponent
        char_to_token(token, c);
        if ((c = fgetc(source_file)) == '+' || c == '-') { // Exponent sign
          char_to_token(token, c);
          c = fgetc(source_file);
        }
        if (!isdigit(c)) {
          fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
          exit(1);
        }
        while (isdigit(c)) { // Exponent value
          char_to_token(token, c);
          c = fgetc(source_file);
        }

        if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.' && c != '+' && c != '-') {
          fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
          exit(1);
        }

        token->type = TOKEN_DECIMAL_LITERAL;
      }

      if (c == '.') { // Decimal literal
        char_to_token(token, c);
        c = fgetc(source_file);

        if (!isdigit(c) && c != 'e' && c != 'E') {
          fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
          exit(1);
        }

        while (isdigit(c)) {
          char_to_token(token, c);
          c = fgetc(source_file);
        }

        if (c == 'e' || c == 'E') { // Exponent
          char_to_token(token, c);
          if ((c = fgetc(source_file)) == '+' || c == '-') { // Exponent sign
            char_to_token(token, c);
            c = fgetc(source_file);
          }
          else {
            // ungetc(c, source_file); // Put the character back into the source file
          }
          if (!isdigit(c) && c != '+' && c != '-') {
            fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
            exit(1);
          }

          while (isdigit(c)) { // Exponent value
            char_to_token(token, c);
            c = fgetc(source_file);
          }

          if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.' && c != '+' && c != '-') {
            fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
            exit(1);
          }

          token->type = TOKEN_DECIMAL_LITERAL; // Set the token type to exponent
        }
        else if (multiline_string != true && token->type != TOKEN_EXPONENT) {
          token->type = TOKEN_DECIMAL_LITERAL;
        }

        ungetc(c, source_file);
        break;
      }
      ungetc(c, source_file);              // Put the last character back into the source file

      if (token->type != TOKEN_DECIMAL_LITERAL) { // If the token type is not exponent
        token->type = TOKEN_INTEGER_LITERAL;
      }

      break;
    }

    if (inString) {   // String literal
      if (c == '"') { // End of string literal
        // Empty string
        if (token->val == NULL) {
          token->val = (char*)malloc(1 * sizeof(char));
          token->val[0] = '\0';
        }


        k = fgetc(source_file);
        j = fgetc(source_file);

        if (k == '"' && j == '"') {
          multiline = false;
          multiline_string = false;

          bool is_on_new_line_flag = false;

          // Last char is a newline
          if (token->val[strlen(token->val) - 4] == '\\') {
            if (token->val[strlen(token->val) - 3] == '0') {
              if (token->val[strlen(token->val) - 2] == '1') {
                if (token->val[strlen(token->val) - 1] == '0') {
                  token->val[strlen(token->val) - 4] = '\0';
                  is_on_new_line_flag = true;
                }
              }
            }
          }

          if (!is_on_new_line_flag) {
            fprintf(stderr, "Multiline string doesn't end on a new line\n");
            exit(1);
          }
        }
        else {
          ungetc(j, source_file);
          ungetc(k, source_file);
        }

        if (multiline_string) {
          char_to_token(token, '"');
          continue;
        }

        inString = false;
        token->type = TOKEN_STRING_LITERAL;

        break;
      }

      if (c == ' ') {
        char_to_token(token, '\\');
        char_to_token(token, '0');
        char_to_token(token, '3');
        char_to_token(token, '2');
      }
      else if (c == '\\') { // Escape sequence
        c = fgetc(source_file);
        if (c == 'n') {
          char_to_token(token, '\\');
          char_to_token(token, '0');
          char_to_token(token, '1');
          char_to_token(token, '0');
        }
        else if (c == 't') {
          char_to_token(token, '\\');
          char_to_token(token, '0');
          char_to_token(token, '0');
          char_to_token(token, '9');
        }
        else if (c == 'r') {
          char_to_token(token, '\\');
          char_to_token(token, '0');
          char_to_token(token, '1');
          char_to_token(token, '3');
        }
        else if (c == '\\') {
          char_to_token(token, '\\');
          char_to_token(token, '0');
          char_to_token(token, '9');
          char_to_token(token, '2');
        }
        else if (c == '"') {
          char_to_token(token, '"');
        }
        else if (c == 'u') {
          j = fgetc(source_file);
          if (j != '{') {
            fprintf(stderr, "Invalid unicode character: %c\n", j);
            exit(1);
          }
          char* unicode = (char*)malloc(8 * sizeof(char));
          if (unicode == NULL) {
            fprintf(stderr, "Error allocating memory for unicode character\n");
            exit(99);
          }
          for (int i = 0; i < 8; i++) {
            unicode[i] = fgetc(source_file);
            if (unicode[i] == '}') {
              unicode[i] = '\0';
              ungetc('}', source_file);
              break;
            }
            if (!isxdigit(unicode[i])) {
              fprintf(stderr, "Invalid unicode character: %s\n", unicode);
              exit(1);
            }
          }
          if (strlen(unicode) == 0) {
            fprintf(stderr, "Invalid unicode character: %s\n", unicode);
            exit(1);
          }
          c = strtol(unicode, NULL, 16);

          fprintf(stderr, "Unicode character: %d\n", (unsigned char)c);

          char_to_token(token, '\\');
          char_to_token(token, ((unsigned char)c / 100) + '0');
          char_to_token(token, (((unsigned char)c % 100) / 10) + '0');
          char_to_token(token, ((unsigned char)c % 10) + '0');

          c = fgetc(source_file);
          if (c != '}') {
            fprintf(stderr, "Invalid unicode character: %c\n", c);
            exit(1);
          }
        }
        else {
          fprintf(stderr, "Invalid escape sequence: \\%c\n", c);
          exit(1);
        }
      }
      else {
        char_to_token(token, c); // Add the character to the token value
        token->type = TOKEN_STRING_LITERAL;
      }
    }
    else if (c == '"') { // Start of string literal
      inString = true;
      k = fgetc(source_file);
      j = fgetc(source_file);
      if (k == '"' && j == '"') {
        c = fgetc(source_file);
        if (c != '\n') {
          fprintf(stderr, "Invalid string literal: %c\n", c);
          exit(1);
        }
        multiline = true;
        multiline_string = true;
      }
      else {
        ungetc(j, source_file);
        ungetc(k, source_file);
      }
    }
    else if (c == '!') {
      char_to_token(token, c);
      token->type = TOKEN_IDENTIFIER_NOT_NULL;

      c = fgetc(source_file);
      if (c == '=') {
        token->val[1] = '=';
        token->type = TOKEN_NE;
      }
      else {
        ungetc(c, source_file);
      }
      break;
    }
    else if (isalpha(c) || c == '_') { // Identifier
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
    }
    else if (c == '\t' || c == ' ' || c == '\r') { // Whitespace
      continue;                                      // Skip the whitespace
    }
    else if (c == '/') {                           // Comment
      c = fgetc(source_file);                        // Get the next character
      if (c == '/') {
        c = fgetc(source_file);
        while (c != '\n' && c != EOF) {
          c = fgetc(source_file);
        }; // Read until the end of the line
        ungetc(c, source_file);
        continue;
      }
      else if (c == '*') {
        if (nested_block_comment > 0) {
          while (1) {
            c = fgetc(source_file);
            if (c == EOF) {
              fprintf(stderr, "Error: Unterminated block comment\n");
              exit(1);
            }
            else if (c == '*') {
              if ((c = fgetc(source_file)) == '/') {
                if (nested_block_comment > 0) {
                  nested_block_comment--; // Exit nested block comment
                  break;
                }
              }
            }
          }
        }
        else {
          nested_block_comment++; // Increment the nested block comment counter
          while (1) {
            next_char = fgetc(source_file);
            if (next_char == '*') {
              if ((next_char = fgetc(source_file)) == '/') {
                nested_block_comment--; // Decrement the nested block comment counter
                if (nested_block_comment == 0) {
                  break; // Exit the outer block comment loop
                }
              }
            }
            else if (next_char == '/') {
              if ((next_char = fgetc(source_file)) == '*') {
                nested_block_comment++; // Increment the nested block comment counter
              }
            }
            else if (next_char == EOF) {
              fprintf(stderr, "Error: Unterminated block comment\n");
              exit(1);
            }
            continue;
          }
        }
        continue; // Skip the block comment
      }
      else {    // Division operator
        ungetc(c, source_file);
        char_to_token(token, '/');
        determine_token_type(token);
        break;
      }
    }
    else { // Other token
      char_to_token(token, c);
      determine_token_type(token);
      break;
    }
  }
  if (token->type == TOKEN_UNKNOWN && c == EOF) {
    token->type = TOKEN_EOF;
  }
  else if (token->type == TOKEN_UNKNOWN) {
    fprintf(stderr, "Unknown token: %s\n", token->val);
    exit(1);
  }
  // printf("Token Type: %d token: %s newline: %d\n", token->type, token->val, token->after_newline);
  return token->type;
}

// int main() {
//   FILE *input_file = stdin;
//   Token token;
//   scanner_init(input_file);
//   while (get_next_token(&token) != TOKEN_EOF)
//     ;

//   scanner_destroy();

//   return 0;
// }