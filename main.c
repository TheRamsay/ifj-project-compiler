#include "scanner.h"
#include "stdio.h"

int main()
{
  FILE *input_file = stdin;
  Token token;
  scanner_init(input_file);
  while (get_next_token(&token) != TOKEN_EOF)
  {
    printf("%d", token.type);
  }

  scanner_destroy();

  return 0;
}