//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"
#include "stack.h"

#define TABLE_SIZE 2

char expression[] = "1 + 1";

enum {
  L,   // < /SHIFT
  R,   // > /REDUCTION
  E,   // = /REDUCTION PREPARE
  Err, // Invalid
};
const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //      +       i
    {R, R},  //+
    {R, Err} // i
};

int get_precedence(char stack_top, char input) {
  switch (stack_top) {
  case '+':
    switch (input) {
    case '+':
      return precedence_table[0][0];
    case 'i':
      return precedence_table[0][1];
    }
  case 'i':
    switch (input) {
    case '+':
      return precedence_table[1][0];
    case 'i':
      return precedence_table[1][1];
    }
  }

  return precedence_table[stack_top][input];
}

typedef struct {
  int type;
  int value;
} token;

int ParseExpression(char expressionToParse[]) {

  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack_init(stack, 1);

  stack_push(stack, (int)'$');

  // TODO
  // token = next_token();
  char token = 'i';
  while (420 == 420) {
    char stackTop = (char)stack_top(stack);

    int action = get_precedence(stackTop, token);

    switch (action) {
    case R:
      stack_push(stack, token);
      stack_push(stack, token);

    // TODO
    // token = next_token()
    case E:
    case L:
        stack_push(stack, action);
        stack_push(stack, token);
        // TODO
        // token = next_token()
    }
  }

  stack_dispose(stack);
  free(stack);
  return 1;
}

void stack_push_after_terminal(Stack *stack) {
  // Check if stack is empty
  if (stack_is_empty(stack)) {
    //TODO ERROR
  }
   int *token;

  for (int i = 1; i <= stack->size; i++) {
    token = stack->items[stack->size - i];
    if (token->terminal) {
      if (stack->len + 1 >= stack->size) {
        resize_stack(stack);
      }
      for (int j = 1; j < i + 1; j++) {
        stack->tokens[stack->len - j + 1] = stack->tokens[stack->len - j];
      }
      stack->tokens[stack->len - i + 1] = token_term_new(token_new(TOK_HANDLE_START, 0, 0), false);
      stack->len++;

      return;
    }
  }
  error_exit(ERR_SYN);
}