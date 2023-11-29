#include "parser.h"
#include "scanner.h"
#include "stdio.h"

#ifndef PARSER_TEST
int main() {
  // FILE *input_file = stdin;
  FILE *input_file = fopen("ahoj.swift", "r");
  Token token;
  (void)token;
  scanner_init(input_file);
  Parser parser;
  parser_init(&parser);
  printf("scanner_init\n");
  // while (get_next_token(&token) != TOKEN_EOF)
  // {
  //   printf("tokenik: %d %s\n", token.type, token.val);
  // }
  parse(&parser);

  scanner_destroy();

  return 0;
}
#else
int main() { return 0; }
#endif