#include "scanner.h"
#include "error.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *source_file;
int token_count = 0;

<<<<<<< HEAD
void scanner_init(FILE *file)
{                     // Initialize the scanner
  source_file = file; // Set the source file to the file passed in
  if (source_file == NULL)
  {
=======
void scanner_init(FILE *file) { // Initialize the scanner
  source_file = file;           // Set the source file to the file passed in
  if (source_file == NULL) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
    fprintf(stderr, "Error opening file\n");
    exit(1);
  }
}
void scanner_destroy()
{
  if (source_file != NULL)
  {
    fclose(source_file);
  }
}
void token_destroy(Token *token)
{
  if (token->val != NULL)
  {
    free(token->val);
  }
}

void determine_token_type(Token *token)
{
  int character; // The current character being read
  token->is_nullable = false;
  token->type = TOKEN_UNKNOWN;
  token->keyword = KW_UNKNOWN;
  if (strcmp(token->val, "if") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_IF;
  }
  else if (strcmp(token->val, "Double") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_DOUBLE;
    char c = fgetc(source_file);
    if (c == '?')
    {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else
    {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "else") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_ELSE;
  }
  else if (strcmp(token->val, "while") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_WHILE;
  }
  else if (strcmp(token->val, "func") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FUNC;
  }
  else if (strcmp(token->val, "Int") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_INT;
    char c = fgetc(source_file);
    if (c == '?')
    {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else
    {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "nil") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_NIL;
  }
  else if (strcmp(token->val, "return") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_RETURN;
  }
  else if (strcmp(token->val, "float") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_FLOAT;
    char c = fgetc(source_file);
    if (c == '?')
    {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else
    {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "String") == 0)
  {
    token->type = TOKEN_KEYWORD;
    token->keyword = KW_STRING;
    char c = fgetc(source_file);
    if (c == '?')
    {
      token->is_nullable = true;
      token->val[strlen(token->val)] = c;
    }
    else
    {
      ungetc(c, source_file);
    }
  }
  else if (strcmp(token->val, "return") == 0)
  {
    token->keyword = KW_RETURN;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "void") == 0)
  {
    token->keyword = KW_VOID;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "var") == 0)
  {
    token->keyword = KW_VAR;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "let") == 0)
  {
    token->keyword = KW_LET;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "in") == 0)
  {
    token->keyword = KW_IN;
    token->type = TOKEN_KEYWORD;
  }
  else if (strcmp(token->val, "for") == 0)
  {
    token->keyword = KW_FOR;
    token->type = TOKEN_KEYWORD;
  }
  else if (token->val[0] == '"')
  { // String literal
    token->type = TOKEN_STRING_LITERAL;
    char *buffer = NULL; // The buffer to store the string literal

    int buffer_size = 0;
    int i = 0;

<<<<<<< HEAD
    while ((character = fgetc(source_file)) != EOF && character != '"')
    { // Read until the end of the string literal
      if (i >= buffer_size - 1)
      {                                                          // If the buffer is full, reallocate it
        buffer_size += 64;                                       // Allocate 64 more bytes
        char *new_buffer = (char *)realloc(buffer, buffer_size); // Reallocate the buffer
        if (new_buffer == NULL)
        {
=======
    while ((character = fgetc(source_file)) != EOF && character != '"') { // Read until the end of the string literal
      if (i >= buffer_size - 1) {                                         // If the buffer is full, reallocate it
        buffer_size += 64;                                                // Allocate 64 more bytes
        char *new_buffer = (char *)realloc(buffer, buffer_size);          // Reallocate the buffer
        if (new_buffer == NULL) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
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
  else
  {
    char c;
    switch (token->val[0])
    { // Determine the token type based on the first character
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
    case '$':
      token->type = TOKEN_DOLLAR;
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
    case '%':
      token->type = TOKEN_MOD;
      break;
    case '-':
      c = fgetc(source_file);
      if (c == '>')
      {
        token->val[1] = '>';
        token->type = TOKEN_ARROW;
      }
      else
      {
        ungetc(c, source_file);
        token->type = TOKEN_MINUS;
      }
      break;
    case '<':
      c = fgetc(source_file);
      if (token->val[1] == '=')
      {
        token->val[1] = '=';
        token->type = TOKEN_LTE;
      }
      else
      {
        ungetc(c, source_file);
        token->type = TOKEN_LT;
      }
      break;
    case '>':
      c = fgetc(source_file);
      if (c == '=')
      {
        token->val[1] = '=';
        token->type = TOKEN_GTE;
      }
      else
      {
        ungetc(c, source_file);
        token->type = TOKEN_GT;
      }
      break;
    case '=':
      c = fgetc(source_file);
      if (c == '=')
      {
        token->val[1] = '=';
        token->type = TOKEN_EQ;
      }
      else
      {
        ungetc(c, source_file);
        token->type = TOKEN_ASSIGN;
      }
      break;
    case '!':
      c = fgetc(source_file);
      if (c == '=')
      {
        token->val[1] = '=';
        token->type = TOKEN_NE;
      }
      else
      {
        ungetc(c, source_file);
        token->type = TOKEN_NOT;
      }
      break;
    // case '&':
    //   c = fgetc(source_file);
    //   if (c == '&') {
    //     token->val[1] = '&';
    //     token->type = TOKEN_AND;
    //   } else {
    //     ungetc(c, source_file);
    //     token->type = TOKEN_BITWISE_AND;
    //   }
    //   break;
    // case '|':
    //   c = fgetc(source_file);
    //   if (c == '|') {
    //     token->val[1] = '|';
    //     token->type = TOKEN_OR;
    //   }
    //   break;
    case '?':
      c = fgetc(source_file);
      if (c == '?')
      {
        token->val[1] = '?';
        token->type = TOKEN_NULL_COALESCING;
      }
      else
      {
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

void char_to_token(Token *token, char c)
{
  char *new_val; // The new value of the token
  if (token->val == NULL)
  {
    new_val = (char *)malloc(2 * sizeof(char)); // Allocate memory for the token value
<<<<<<< HEAD
    if (new_val == NULL)
    {
=======
    if (new_val == NULL) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
      fprintf(stderr, "Error allocating memory for token value\n");
      exit(99);
    }
    new_val[0] = c;
    new_val[1] = '\0';
  }
  else
  {
    new_val = (char *)realloc(token->val, (token->length + 2) * sizeof(char));
<<<<<<< HEAD
    if (new_val == NULL)
    {
=======
    if (new_val == NULL) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
      fprintf(stderr, "Error reallocating memory for token value\n");
      exit(99);
    }
    new_val[token->length] = c;
    new_val[token->length + 1] = '\0';
  }

  token->val = new_val;
  token->length++;
}

int get_next_token(Token *token)
{
  int nested_block_comment = 0;
  token->type = TOKEN_UNKNOWN;
  token->length = 0;
  token->val = NULL;
  char c, k, j;
  bool inString = false;
  bool newline = false;
  bool multiline = false;
  bool multiline_string = false;
<<<<<<< HEAD

  while (true)
  {
=======
  int next_char;
  while (true) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a

    c = fgetc(source_file); // Get the next character from the source file
    if (c == EOF)
    {
      break;
    }

    if (newline && c != '\t' && c != ' ' && c != '\r')
    {
      token->after_newline = true;
      newline = false;
    }
    else if (!token_count && !newline)
    {
      token->after_newline = true;
    }
    else
    {
      token->after_newline = false;
    }
    token_count++;

    if (c == '\n')
    {
      if (multiline)
      {
        char_to_token(token, '\n');
      }
      newline = true;
      continue;
    }
    if (c == '_')
    {
      char_to_token(token, c);
      c = fgetc(source_file);
<<<<<<< HEAD
      if (!isalpha(c) && !isdigit(c) && c != ' ')
      {
=======
      if (!isalpha(c) && !isdigit(c) && c != ' ' && c != EOF && c != '=') {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
        fprintf(stderr, "Invalid identifier: %s\n", token->val);
        exit(1);
      }

      if (c == EOF || c == ' ' || c == '\n' || c == '=') {
        token->type = TOKEN_IDENTIFIER;
        ungetc(c, source_file);
        break;
      }
    }
    if (isdigit(c))
    {
      char_to_token(token, c); // Add the character to the token value
      while (isdigit(c = fgetc(source_file)))
      { // Read until the end of the literal
        char_to_token(token, c);
      }
<<<<<<< HEAD
      if (c == 'e' || c == 'E')
      {
        fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
        exit(1);
      }
      if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.')
      {
=======
      if (c == 'e' || c == 'E') {
        fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
        exit(1);
      }
      if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.') {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
        fprintf(stderr, "Invalid integer literal: %s\n", token->val);
        exit(1);
      }
      //   ungetc(c, source_file);              // Put the last character back into the source file
      //   token->type = TOKEN_INTEGER_LITERAL; // Set the token type to integer literal
      //   break;
      // }
      if (c == '.')
      { // Decimal literal
        char_to_token(token, c);
        c = fgetc(source_file);

<<<<<<< HEAD
        if (!isdigit(c) && c != 'e' && c != 'E')
        {
=======
        if (!isdigit(c) && c != 'e' && c != 'E') {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
          fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
          exit(1);
        }
        while (isdigit(c))
        {
          char_to_token(token, c);
          c = fgetc(source_file);
        }
        // if (!isdigit(c) && c != 'e' && c != 'E') {
        //   fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
        //   exit(1);
        // }
        if (c == 'e' || c == 'E')
        { // Exponent
          char_to_token(token, c);
          if ((c = fgetc(source_file)) == '+' || c == '-')
          { // Exponent sign
            char_to_token(token, c);
          }
          else
          {
            // ungetc(c, source_file); // Put the character back into the source file
          }
<<<<<<< HEAD
          if (!isdigit(c))
          {
=======
          if (!isdigit(c)) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
            fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
            exit(1);
          }
          while (isdigit(c))
          { // Exponent value
            char_to_token(token, c);
            c = fgetc(source_file);
          }

<<<<<<< HEAD
          if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.')
          {
=======
          if (c != ' ' && c != '\n' && c != EOF && !isalpha(c) && c != '.') {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
            fprintf(stderr, "Invalid decimal literal: %s\n", token->val);
            exit(1);
          }

          token->type = TOKEN_EXPONENT; // Set the token type to exponent
        }
        else if (multiline_string != true && token->type != TOKEN_EXPONENT)
        {
          token->type = TOKEN_DECIMAL_LITERAL;
        }

        ungetc(c, source_file);
        break;
      }
      ungetc(c, source_file);              // Put the last character back into the source file
      token->type = TOKEN_INTEGER_LITERAL; // Set the token type to integer literal
      break;
    }

    if (inString)
    { // String literal
      if (c == '"')
      { // End of string literal
        k = fgetc(source_file);
        j = fgetc(source_file);
        if (k == '"' && j == '"')
        {
          multiline = false;
<<<<<<< HEAD
          if (token->val[strlen(token->val) - 1] != '\n')
          {
=======
          if (token->val[strlen(token->val) - 1] != '\n') {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
            fprintf(stderr, "Invalid string literal: %s\n", token->val);
            exit(1);
          }
          else
          {
            token->val[strlen(token->val) - 1] = '\0';
          }
        }
        else
        {
          ungetc(j, source_file);
          ungetc(k, source_file);
        }
        inString = false;
        token->type = TOKEN_STRING_LITERAL;
        break;
      }
      else if (c == '\\')
      {
        c = fgetc(source_file);
        if (c == 'n')
        {
          char_to_token(token, '\n');
        }
        else if (c == 't')
        {
          char_to_token(token, '\t');
        }
        else if (c == 'r')
        {
          char_to_token(token, '\r');
        }
        else if (c == '\\')
        {
          char_to_token(token, '\\');
        }
        else if (c == '"')
        {
          char_to_token(token, '"');
        }
        else if (c == 'u')
        {
          j = fgetc(source_file);
          if (j != '{')
          {
            exit(1);
          }
          char *unicode = (char *)malloc(8 * sizeof(char));
<<<<<<< HEAD
          if (unicode == NULL)
          {
=======
          if (unicode == NULL) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
            fprintf(stderr, "Error allocating memory for unicode character\n");
            exit(99);
          }
          for (int i = 0; i < 8; i++)
          {
            unicode[i] = fgetc(source_file);
            if (unicode[i] == '}')
            {
              unicode[i] = '\0';
              ungetc('}', source_file);
              break;
            }
<<<<<<< HEAD
            if (!isxdigit(unicode[i]))
            {
=======
            if (!isxdigit(unicode[i])) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
              fprintf(stderr, "Invalid unicode character: %s\n", unicode);
              exit(1);
            }
          }
          c = strtol(unicode, NULL, 16);
          char_to_token(token, c);

          c = fgetc(source_file);
          if (c != '}')
          {
            exit(1);
          }
        }
        else
        {
          exit(1);
        }
      }
      else
      {
        char_to_token(token, c); // Add the character to the token value
        multiline_string = true;
        token->type = TOKEN_STRING_LITERAL;
      }
    }
    else if (c == '"')
    { // Start of string literal
      inString = true;
      k = fgetc(source_file);
      j = fgetc(source_file);
      if (k == '"' && j == '"')
      {
        c = fgetc(source_file);
        if (c != '\n')
          exit(1);
        multiline = true;
      }
      else
      {
        ungetc(j, source_file);
        ungetc(k, source_file);
      }
    }
    else if (isalpha(c) || c == '_')
    { // Identifier
      char_to_token(token, c);
      while (isalnum(c = fgetc(source_file)) || c == '_')
      { // Read until the end of the identifier
        char_to_token(token, c);
      }

      if (c == '!')
      {
        char_to_token(token, c);
        token->type = TOKEN_IDENTIFIER_NOT_NULL;
        break;
      }

      ungetc(c, source_file);
      determine_token_type(token);

      if (token->type == TOKEN_UNKNOWN)
      {
        token->type = TOKEN_IDENTIFIER;
      }
      break;
    }
    else if (c == '\t' || c == ' ' || c == '\r')
    {           // Whitespace
      continue; // Skip the whitespace
    }
    else if (c == '/')
    {                         // Comment
      c = fgetc(source_file); // Get the next character
      if (c == '/')
      {
        c = fgetc(source_file);
        while (c != '\n' && c != EOF)
        {
          c = fgetc(source_file);
        }; // Read until the end of the line
        continue;
<<<<<<< HEAD
      }
      else if (c == '*')
      { // Block comment
        if (nested_block_comment > 0)
        {
          goto loop; // If we are in a nested block comment, skip the comment
        }
        else
        {
          nested_block_comment++; // Otherwise, increment the nested block comment counter
        loop:
        {
          int next_char = fgetc(source_file);
          while (next_char != '*')
          {
            if (next_char == EOF)
            {
=======
      } else if (c == '*') { // Block comment
        if (nested_block_comment > 0) {
          printf("viac\n");
          goto loop; // If we are in a nested block comment, skip the comment
        } else {
          printf("nula\n");
          nested_block_comment++; // Otherwise, increment the nested block comment counter
        loop:
          printf("loop\n");
          next_char = fgetc(source_file);
          while (next_char != '*') {
            if (next_char == EOF) {
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
              fprintf(stderr, "Error: Unterminated block comment\n");
              exit(1);
            }
            next_char = fgetc(source_file); // Read until the end of the block comment
          }
<<<<<<< HEAD
          if (fgetc(source_file) == '/')
          { // If the block comment is over, decrement the nested block comment counter
            if (--nested_block_comment > 0)
            {
              goto loop;
            }
            continue;
          }
          else
          {
=======
          if (fgetc(source_file) == '/') { // If the block comment is over, decrement the nested block comment counter
            printf("if\n");
            if (--nested_block_comment > 0) {
              printf("menej\n");
              goto loop;
            }
            continue;
          } else {
            printf("else\n");
>>>>>>> d92f932ffd138392c4a8a180a2b86cf760e50b5a
            goto loop; // Otherwise, continue reading the block comment
          }
        }
        }
      }
      else
      {
        ungetc(c, source_file);
        char_to_token(token, '/');
        determine_token_type(token);
        break;
      }
    }
    else
    {
      char_to_token(token, c);
      determine_token_type(token);
      break;
    }
  }
  if (token->type == TOKEN_UNKNOWN && c == EOF)
  {
    token->type = TOKEN_EOF;
  }
  else if (token->type == TOKEN_UNKNOWN)
  {
    fprintf(stderr, "Unknown token: %s\n", token->val);
    exit(1);
  }
  printf("Token Type: %d token: %s\n", token->type, token->val);

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