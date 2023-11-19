//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"
#include "stack.h"

const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //+ i  $  E
    {R, L, R, E},     //+
    {R, Err, R, Err}, // i
    {L, L, Err, L},   //$
    {E, Err, R, Err}  // E
};

Rule_t rules[] = {{TOKEN_EXPRESSION, 3, {TOKEN_EXPRESSION, TOKEN_PLUS, TOKEN_EXPRESSION}}, {TOKEN_EXPRESSION, 1, {TOKEN_INTEGER_LITERAL}}};

int get_operator_index(TokenType op) {
  switch (op) {
  case TOKEN_PLUS:
    return 0;
  case TOKEN_IDENTIFIER:
  case TOKEN_INTEGER_LITERAL:
  case TOKEN_DECIMAL_LITERAL:
    return 1;
  case TOKEN_STACK_BOTTOM:
    return 2;
  case TOKEN_EXPRESSION:
    return 3;

  default:
    return -1; // Operator not found
  }
}

Stack_token_t get_precedence(Stack_token_t stack_top, Stack_token_t input) {
  int stack_index = get_operator_index(stack_top.token);
  int input_index = get_operator_index(input.token);

  if (stack_index == -1 || input_index == -1) {
    return (Stack_token_t){.precedence = Err}; // Handle invalid operators
  }

  return (Stack_token_t){.precedence = precedence_table[stack_index][input_index]};
}

Stack_token_t evaluate_rule(Stack_token_t *tokens) {
  TokenType *rightSide = extract_tokens_from_stack(tokens, sizeof(tokens) / sizeof(tokens[0]));

  // Check each rule
  for (int i = 0; i < RULES_SIZE; ++i) {
    int ruleMatch = 1; // Assume a match until proven otherwise

    // Check each TokenType in the rule
    for (int j = 0; j < rules[i].lenght; ++j) {
      if (rightSide[j] != rules[i].rule[j]) {
        ruleMatch = 0; // Not a match
        break;
      }
    }

    if (ruleMatch) {
      return (Stack_token_t){.token = rules[i].result, .precedence = None};
    }
  }

  return (Stack_token_t){.token = TOKEN_EOF, .precedence = None};
}

Stack_token_t get_next_token_temp(TokenType array[], int index) {
  if (index >= 0 && index <= sizeof(array) / sizeof(array[0])) {
    // Return the character at the specified index
    return (Stack_token_t){.precedence = None, .token = array[index]};
  }
  return (Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None};
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
  Stack_token_t *result = arrayFromStack(stack);
  Stack_token_t *ruleProduct = malloc(sizeof(Stack_token_t));

  *ruleProduct = evaluate_rule(result);

  if (ruleProduct->token == TOKEN_EOF) {
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

int parse_expression(TokenType *expressionToParse) {
  int expIndex = 0;

  void_stack_t *stack = stack_new(8192);
  stack_push(stack, &(Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None});

  Stack_token_t token = get_next_token_temp(expressionToParse, expIndex);
  expIndex++;
  while (420 == 420) {
    Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

    if (token.token == TOKEN_STACK_BOTTOM && stackTop.token == TOKEN_EXPRESSION) {
      break;
    }

    Stack_token_t action = get_precedence(stackTop, token);
    if (action.precedence == Err) {
      return 0;
    }

    switch (action.precedence) {
    case L:
      handle_shift_case(stack, token, action);
      token = get_next_token_temp(expressionToParse, expIndex);
      expIndex++;

      break;

    case R:
      // If there is no rule, expression is not valid
      if (handle_reduce_case(stack, token) == -1) {
        return 0;
      }
      break;

    case E:
      handle_equals_case(stack, token);
      token = get_next_token_temp(expressionToParse, expIndex);
      expIndex++;

      break;
    }
  }

  stack_dispose(stack);
  free(stack);
  return 1;
}

// Runs thought the stack until it finds an L precedence, returns the array of tokens to be reduced
Stack_token_t *arrayFromStack(void_stack_t *stack) {
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

  int currentIndex = 0;
  Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

  while (stackTop.precedence != L && stack->top_index >= 0) {
    resultArray[currentIndex++] = stackTop;
    stack_pop(stack);
    stackTop = *(Stack_token_t *)stack_top(stack);
  }
  // Get rid of the L
  stack_pop(stack);

  return resultArray;
}

TokenType *extract_tokens_from_stack(Stack_token_t *stack_tokens, size_t size) {
  TokenType *result = malloc(size * sizeof(TokenType));

  if (result != NULL) {
    for (size_t i = 0; i < size; ++i) {
      result[i] = stack_tokens[i].token;
    }
  }

  return result;
}