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
    {R, R, L, L, R, R, R, R, R, R, L, R, R, L, L, R, E}, // +
    {R, R, L, L, R, R, R, R, R, R, L, R, R, L, L, R, E}, // -
    {R, R, R, R, R, R, R, R, R, R, L, R, R, L, L, R, E}, // *
    {R, R, R, R, R, R, R, R, R, R, L, R, R, L, L, R, E}, // /
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // <
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // <=
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // >
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // >=
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // ==
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, E}, // !=
    {L, L, L, L, L, L, L, L, L, L, L, E, L, L, L, X, E}, // (
    {R, R, R, R, R, R, R, R, R, R, X, R, X, X, X, R, L}, // )
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, E, L}, // ??
    {R, R, R, R, X, X, X, X, X, X, X, R, X, X, R, L, X}, // !
    {R, R, R, R, R, R, R, R, R, R, X, R, R, L, L, R, X}, // i
    {L, L, L, L, L, L, L, L, L, L, L, X, X, L, L, X, L}, // $
    {E, E, E, E, E, E, E, E, E, E, L, R, L, L, X, R, X}  // E
};

Rule_t rules[] = {
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
    {TOKEN_EXPRESSION, 5, {TOKEN_LPAREN, TOKEN_EXPRESSION, TOKEN_PLUS, TOKEN_EXPRESSION, TOKEN_RPAREN}},
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
  case TOKEN_EXPRESSION:
    return 16;

  default:
    return -1; // Operator not found
  }
}

Stack_token_t get_precedence(Stack_token_t stack_top, Stack_token_t input) {
  int stack_index = get_operator_index(stack_top.token.type);
  int input_index = get_operator_index(input.token.type);

  if (stack_index == -1 || input_index == -1) {
    return (Stack_token_t){.precedence = X}; // Handle invalid operators
  }

  return (Stack_token_t){.precedence = precedence_table[stack_index][input_index]};
}

Stack_token_t evaluate_rule(Stack_token_t *tokens, int size) {
  Stack_token_t *rightSide = extract_tokens_from_stack(tokens, size);

  // Check each rule
  for (int i = 0; i < RULES_SIZE; ++i) {
    int ruleMatch = 1; // Assume a match until proven otherwise

    // Check each TokenType in the rule
    for (int j = 0; j < rules[i].lenght; ++j) {
      if (rightSide[j].token.type != rules[i].rule[j]) {
        ruleMatch = 0; // Not a match
        break;
      }
    }

    if (ruleMatch) {

      // Assign type
      if (rules[i].lenght == 1) {
        switch (rules[i].rule[0]) {
        case TOKEN_INTEGER_LITERAL:
          return (Stack_token_t){.token = rules[i].result, .precedence = None, .type = {.data_type = INT_TYPE, .nullable = false}};

        case TOKEN_DECIMAL_LITERAL:
          return (Stack_token_t){.token = rules[i].result, .precedence = None, .type = {.data_type = DOUBLE_TYPE, .nullable = false}};

        case TOKEN_STRING_LITERAL:
          return (Stack_token_t){.token = rules[i].result, .precedence = None, .type = {.data_type = STRING_TYPE, .nullable = false}};
        case TOKEN_IDENTIFIER: {
        }
        default:
          break;
        }
        return (Stack_token_t){.token = rules[i].result, .precedence = None, .type = {.data_type = UNKNOWN_TYPE, .nullable = false}};
      }
      if (rules[i].lenght == 3) {
        printf("%d %d %d\n", rightSide[0].type.data_type, rightSide[1].type.data_type, rightSide[2].type.data_type);
        if (rightSide[0].type.data_type != rightSide[2].type.data_type) {
          exit_with_error(SEMANTIC_ERR_EXPR, "Cannot use operator on different types");
        }
      }

      return (Stack_token_t){.token = rules[i].result, .precedence = None};
    }
  }

  return (Stack_token_t){.token = TOKEN_EOF, .precedence = None};
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

void handle_shift_case(void_stack_t *stack, Stack_token_t token, Stack_token_t action) {
  Stack_token_t *new_token = malloc(sizeof(Stack_token_t));
  Stack_token_t *new_action = malloc(sizeof(Stack_token_t));
  *new_token = token;
  *new_action = action;

  stack_push(stack, new_action);
  stack_push(stack, new_token);
}

int handle_reduce_case(void_stack_t *stack, Stack_token_t token) {
  int size = 0;
  Stack_token_t *result = arrayFromStack(stack, &size);
  Stack_token_t *ruleProduct = malloc(sizeof(Stack_token_t));

  *ruleProduct = evaluate_rule(result, size);

  if (ruleProduct->token.type == TOKEN_EOF) {
    return -1;
  }

  Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);
  Stack_token_t *action = malloc(sizeof(Stack_token_t));
  *action = get_precedence(stackTop, token);

  stack_push(stack, action);
  stack_push(stack, ruleProduct);
  return 0;
}

void handle_equals_case(void_stack_t *stack, Stack_token_t token) {

  Stack_token_t *new_token = malloc(sizeof(Stack_token_t));
  *new_token = token;
  stack_push(stack, new_token);
}
#ifdef PARSER_TEST
int parse_expression(Token *testExpressionToParse, int inputSize, Parser *parser) {
  int expIndex = 0;
#else
int parse_expression(Parser *parser) {
#endif

  void_stack_t *stack = stack_new(8192);
  stack_push(stack, &(Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None});

  Stack_token_t token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
  expIndex++;
  while (420 == 420) {
    Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

    if (token.token.type == TOKEN_STACK_BOTTOM && stackTop.token.type == TOKEN_EXPRESSION) {
      Stack_token_t *stackTempTop = malloc(sizeof(Stack_token_t));
      *stackTempTop = *(Stack_token_t *)stack_pop(stack);
      Stack_token_t stackTop2 = *(Stack_token_t *)stack_top(stack);
      if (stackTop2.token.type == TOKEN_STACK_BOTTOM) {
      stack_push(stack, &stackTempTop);

        break;
      }

      stack_push(stack, &stackTempTop);
    }

    // if (token.token.type == TOKEN_NOT) {
    //   Stack_token_t *lastItem = malloc(sizeof(Stack_token_t));
    //   lastItem = (Stack_token_t *)stack_top(stack);

    //   SymtableItem *item = symtable_get(parser->local_table, lastItem->token.val);
    //   if (item != NULL && item->data->variable.identifier_type.nullable) {
    //     exit_with_error(SEMANTIC_ERR_EXPR, "Cannot use ! operator on nullable type");
    //   }
    //   item = symtable_get(parser->global_table, lastItem->token.val);
    //   if (item != NULL && item->data->variable.identifier_type.nullable) {
    //     exit_with_error(SEMANTIC_ERR_EXPR, "Cannot use ! operator on nullable type");
    //   }
    //   token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
    // }

    Stack_token_t action = get_precedence(stackTop, token);
    if (action.precedence == X) {
      printf("Invalid relation between %d and %d\n", stackTop.token.val, token.token.val);
      exit_with_error(SYNTAX_ERR, "Invalid expression");
    }

    switch (action.precedence) {
    case L:
      handle_shift_case(stack, token, action);
      token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
      expIndex++;

      break;

    case R:
      // If there is no rule, expression is not valid
      if (handle_reduce_case(stack, token) == -1) {
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

  stack_dispose(stack);
  free(stack);
  return 1;
}

// Runs thought the stack until it finds an L precedence, returns the array of tokens to be reduced
Stack_token_t *arrayFromStack(void_stack_t *stack, int *index) {
  // Check if the stack is empty
  if (stack->top_index == -1) {
    return NULL;
  }

  // Allocate memory for the array
  Stack_token_t *resultArray = (Stack_token_t *)malloc((stack->top_index + 1) * sizeof(Stack_token_t)); // +1 for the null terminator

  if (resultArray == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

  while (stackTop.precedence != L && stack->top_index >= 0) {
    // if (stackTop.token.type == TOKEN_EXPRESSION) {
    //   Stack_token_t tempToken = *(Stack_token_t *)stack_pop(stack);
    //   stackTop = *(Stack_token_t *)stack_top(stack);

    //   resultArray[*index] = stackTop;
    //   stack_pop(stack);
    //   *index = *index + 1;

    //   stack_push(stack, &tempToken);
    //   continue;
    // }

    resultArray[*index] = stackTop;
    stack_pop(stack);
    stackTop = *(Stack_token_t *)stack_top(stack);
    *index = *index + 1;
  }
  // Get rid of the L
  stack_pop(stack);

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