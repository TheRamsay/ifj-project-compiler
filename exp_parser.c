//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"

#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"

const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //+ -  *  /  <  <= >  => == != (  ) ??  !  i  $  E
    {R, R, L, L, R, R, R, R, R, R, L, R, R, L, L, R, E},  // +
    {R, R, L, L, R, R, R, R, R, R, L, R, R, L, L, R, E},  // -
    {R, R, R, R, R, R, R, R, R, R, L, R, R, L, L, R, E},  // *
    {R, R, R, R, R, R, R, R, R, R, L, R, R, L, L, R, E},  // /
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // <
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // <=
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // >
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // >=
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // ==
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E},  // !=
    {L, L, L, L, L, L, L, L, L, L, L, E, L, L, L, X, E},  // (
    {R, R, R, R, R, R, R, R, R, R, X, R, X, X, X, R, L},  // )
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, L},  // ??
    {R, R, R, R, X, X, X, X, X, X, X, R, X, X, R, L, X},  // !
    {R, R, R, R, R, R, R, R, R, R, X, R, R, L, L, R, X},  // i
    {L, L, L, L, L, L, L, L, L, L, L, X, L, L, L, X, L},  // $
    {E, E, E, E, E, E, E, E, E, E, L, R, L, L, X, R, X}   // E
};

Rule_t rules[] = {
    {TOKEN_EXPRESSION, 1, {TOKEN_INTEGER_LITERAL}},
    {TOKEN_EXPRESSION, 1, {TOKEN_DECIMAL_LITERAL}},
    {
        TOKEN_EXPRESSION,
        1,
        {TOKEN_STRING_LITERAL},
    },
    {
        TOKEN_EXPRESSION,
        1,
        {TOKEN_IDENTIFIER},
    },
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_PLUS, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_MINUS, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_MULTIPLY, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_DIV, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_EQ, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_GT, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_GE, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_LT, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_LE, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_NEQ, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_NULL_COALESCING, TOKEN_EXPRESSION}},
    {TOKEN_EXPRESSION, 3, {TOKEN_LPAREN, TOKEN_EXPRESSION, TOKEN_RPAREN}},
    {TOKEN_EXPRESSION,
     5,
     {TOKEN_LPAREN, TOKEN_EXPRESSION, TOKEN_PLUS, TOKEN_EXPRESSION, TOKEN_RPAREN}},

};

int get_operator_index(TokenType op) {
  switch (op) {
    case TOKEN_PLUS:
      return 0;
    case TOKEN_MINUS:
      return 1;
    case TOKEN_MULTIPLY:
      return 2;
    case TOKEN_DIV:
      return 3;
    case TOKEN_LT:
      return 4;
    case TOKEN_LE:
      return 5;
    case TOKEN_GT:
      return 6;
    case TOKEN_GE:
      return 7;
    case TOKEN_EQ:
      return 8;
    case TOKEN_NEQ:
      return 9;
    case TOKEN_LPAREN:
      return 10;
    case TOKEN_RPAREN:
      return 11;
    case TOKEN_NULL_COALESCING:
      return 12;
    case TOKEN_NOT:
      return 13;
    case TOKEN_IDENTIFIER:
    case TOKEN_STRING_LITERAL:
    case TOKEN_INTEGER_LITERAL:
    case TOKEN_DECIMAL_LITERAL:
      return 14;
    case TOKEN_STACK_BOTTOM:
      return 15;

    default:
      return -1;  // Operator not found
  }
}

Stack_token_t get_precedence(Stack_token_t stack_top, Stack_token_t input) {
  int stack_index = get_operator_index(stack_top.token.type);
  int input_index = get_operator_index(input.token.type);

  if (stack_index == -1 || input_index == -1) {
    return (Stack_token_t){.precedence = X};  // Handle invalid operators
  }

  return (Stack_token_t){.precedence = precedence_table[stack_index][input_index]};
}

Stack_token_t evaluate_rule(Stack_token_t token) {
  switch (token.token.type) {
    case TOKEN_INTEGER_LITERAL:
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                             .precedence = None,
                             .type = {.data_type = INT_TYPE, .nullable = false}};

    case TOKEN_DECIMAL_LITERAL:
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                             .precedence = None,
                             .type = {.data_type = DOUBLE_TYPE, .nullable = false}};

    case TOKEN_STRING_LITERAL:
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                             .precedence = None,
                             .type = {.data_type = STRING_TYPE, .nullable = false}};
    case TOKEN_IDENTIFIER: {
      break;
    }
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_MULTIPLY:
    case TOKEN_DIV:
    case TOKEN_LT:
    case TOKEN_LE:
    case TOKEN_GT:
    case TOKEN_GE:
    case TOKEN_EQ:
    case TOKEN_NEQ: {
      // TODO type check
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                             .precedence = None,
                             .type = {.data_type = INT_TYPE, .nullable = false}};
    }
    case TOKEN_NULL_COALESCING: {
      // TODO type check
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                             .precedence = None,
                             .type = {.data_type = INT_TYPE, .nullable = false}};
    }

    default:
      break;
  }
  return (Stack_token_t){.token = {TOKEN_EOF, KW_UNKNOWN, "Eof", 3},
                         .precedence = None,
                         .type = {.data_type = UNKNOWN_TYPE, .nullable = false}};
}

Stack_token_t get_next_token_wrap(Token array[], int index, int size) {
#ifdef PARSER_TEST
  if (index >= 0 && index < size) {
    // Return the character at the specified index
    return (Stack_token_t){.precedence = None, .token = array[index]};
  }
  return (Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None};
#else
  Token *token = malloc(sizeof(Token));
  int result = get_next_token(token);

  return (Stack_token_t){.token = token, .precedence = None};
#endif
}

void handle_shift_case(void_stack_t *stack, Stack_token_t token) {
  Stack_token_t *new_token = malloc(sizeof(Stack_token_t));

  *new_token = token;
  stack_push(stack, new_token);
}

int handle_reduce_case(void_stack_t *stack, Stack_token_t token, Stack_token_t precedence) {
  while (precedence.precedence == R) {
    Stack_token_t firstToken = *(Stack_token_t *)stack_pop(stack);

    Stack_token_t *ruleProduct = malloc(sizeof(Stack_token_t));

    *ruleProduct = evaluate_rule(firstToken);

    if (ruleProduct->token.type == TOKEN_EOF) {
      Stack_token_t secondToken = *(Stack_token_t *)stack_pop(stack);
      Stack_token_t thirdToken = *(Stack_token_t *)stack_pop(stack);

      if (firstToken.token.type == TOKEN_EXPRESSION && thirdToken.token.type == TOKEN_EXPRESSION) {
        if (firstToken.type.data_type != thirdToken.type.data_type) {
          exit_with_error(SEMANTIC_ERR_EXPR, "Cannot use operator on different types");
        }
        *ruleProduct = evaluate_rule(secondToken);
      }

      if (firstToken.token.type == TOKEN_RPAREN && thirdToken.token.type == TOKEN_LPAREN) {
        if (secondToken.token.type == TOKEN_EXPRESSION) {
          *ruleProduct = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1},
                                         .precedence = None,
                                         .type = {.data_type = INT_TYPE, .nullable = false}};
        }
      }
    }

    stack_push(stack, ruleProduct);

    Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);
    if (stackTop.token.type == TOKEN_EXPRESSION) {
      // Pop the top token
      Stack_token_t tempToken = *(Stack_token_t *)stack_pop(stack);

      // Get the second most top token
      Stack_token_t second_most_top_token = *(Stack_token_t *)stack_top(stack);

      // Update the global variable _stack_top
      stackTop = second_most_top_token;

      // Push the top token back onto the stack
      stack_push(stack, &tempToken);
    }
    precedence = get_precedence(stackTop, token);
  }
  return 0;
}

void handle_equals_case(void_stack_t *stack, Stack_token_t token) {
  Stack_token_t *new_token = malloc(sizeof(Stack_token_t));
  *new_token = token;
  stack_push(stack, new_token);
}
#ifdef PARSER_TEST
Token parse_expression(Token *testExpressionToParse, int inputSize, Parser *parser) {
  int expIndex = 0;
#else
Token parse_expression(Parser *parser) {
#endif

  void_stack_t *stack = stack_new(8192);
  stack_push(stack, &(Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None});

  Stack_token_t token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
  expIndex++;
  while (420 == 420) {
    Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

    if (token.token.type == TOKEN_STACK_BOTTOM && stackTop.token.type == TOKEN_EXPRESSION) {
      Stack_token_t top_token = *(Stack_token_t *)stack_pop(stack);
      Stack_token_t second_most_top_token = *(Stack_token_t *)stack_top(stack);

      stack_push(stack, &top_token);
      if (second_most_top_token.token.type == TOKEN_STACK_BOTTOM) {
        break;
      }
    }

    if (stackTop.token.type == TOKEN_EXPRESSION) {
      // Pop the top token
      Stack_token_t tempToken = *(Stack_token_t *)stack_pop(stack);

      // Get the second most top token
      Stack_token_t second_most_top_token = *(Stack_token_t *)stack_top(stack);

      // Update the global variable _stack_top
      stackTop = second_most_top_token;

      // Push the top token back onto the stack
      stack_push(stack, &tempToken);
    }

    // if (token.token.type == TOKEN_STACK_BOTTOM && stackTop.token.type == TOKEN_EXPRESSION) {

    //   break;
    // }

    Stack_token_t action = get_precedence(stackTop, token);
    if (action.precedence == X) {
      if (stackTop.token.type == TOKEN_STACK_BOTTOM && token.token.type == TOKEN_STACK_BOTTOM) {
        break;
        // Stack_token_t tempToken = *(Stack_token_t *)stack_top(stack);
        // ;
        // if (tempToken.token.type == TOKEN_EXPRESSION) {
        //   break;
        // }
      }
      printf("Invalid relation between %d and %d\n", stackTop.token.val, token.token.val);
      exit_with_error(SYNTAX_ERR, "Invalid expression");
    }

    switch (action.precedence) {
      case L:
        handle_shift_case(stack, token);
        token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
        expIndex++;

        break;

      case R:
        // If there is no rule, expression is not valid
        if (handle_reduce_case(stack, token, action) == -1) {
          printf("No rule for token %d\n", token.token.type);
          exit_with_error(SYNTAX_ERR, "Invalid expression");
        }
        break;

      case E:
        handle_equals_case(stack, token);
        token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
        expIndex++;

        break;
    }
  }

  Token *result = malloc(sizeof(Token));
  *result = ((Stack_token_t *)stack_top(stack))->token;

  stack_dispose(stack);
  free(stack);
  return *result;
}

// Runs thought the stack until it finds an L precedence, returns the array of tokens to be reduced
Stack_token_t *arrayFromStack(void_stack_t *stack, int *index) {
  // Check if the stack is empty
  if (stack->top_index == -1) {
    return NULL;
  }

  // Allocate memory for the array
  Stack_token_t *resultArray = (Stack_token_t *)malloc(
      (stack->top_index + 1) * sizeof(Stack_token_t));  // +1 for the null terminator

  if (resultArray == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

  while (stackTop.precedence != L && stack->top_index >= 0) {
    resultArray[*index] = stackTop;
    stack_pop(stack);
    stackTop = *(Stack_token_t *)stack_top(stack);
    *index = *index + 1;
  }

  return resultArray;
}

Stack_token_t *extract_tokens_from_stack(Stack_token_t *stack_tokens, size_t size) {
  Stack_token_t *result = malloc(size * sizeof(Stack_token_t));

  if (result != NULL) {
    for (size_t i = 0; i < size; ++i) {
      result[i] = stack_tokens[i];
    }
  }

  return result;
}