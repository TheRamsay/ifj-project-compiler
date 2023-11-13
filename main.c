#include "scanner.h"
#include "parser.h"
#include "stdio.h"

int main()
{

  // FILE *input_file = stdin;
  FILE *input_file = fopen("ahoj.swift", "r");
  Token token;
  (void)token;
  scanner_init(input_file);
  // while (get_next_token(&token) != TOKEN_EOF)
  // {
  //   printf("%d", token.type);
  // }
  parse();

  scanner_destroy();

  return 0;
}