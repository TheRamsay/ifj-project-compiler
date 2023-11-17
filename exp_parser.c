//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"
#include "stack.h"

#define TABLE_SIZE 4

enum {
  L,   // < /SHIFT
  R,   // > /REDUCTION
  E,   // = /REDUCTION PREPARE
  Err, // Invalid
};

const int precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //+ i  $  E
    {R, L, R, E},     //+
    {R, Err, R, Err}, // i
    {L, L, Err, L},   //$
    {E, Err, R, Err}  // E
};

ExtendedToken rules[] = {
    {TOKEN_EXPRESSION_RULE, "E+E", 'E'}, {TOKEN_EXPRESSION_RULE, "i", 'E'},
};

int get_operator_index(char op) {
  for (int i = 0; i < TABLE_SIZE; ++i) {
    if (operators[i] == op) {
      return i;
    }
  }
  return -1; // Operator not found
}

int get_precedence(char stack_top, char input) {
  int stack_index = get_operator_index(stack_top);
  int input_index = get_operator_index(input);

  if (stack_index == -1 || input_index == -1) {
    return Err; // Handle invalid operators
  }

  return precedence_table[stack_index][input_index];
}

char evaluate_rule(char *rightSide) {

  // Check each rule
  for (int i = 0; i < sizeof(rules) / sizeof(rules[0]); ++i) {
    if (strcmp(rightSide, rules[i].rule) == 0) {
      return rules[i].result;
    }
  }

  return ' '; // No matching rule
}

char get_next_char(char *str, int index) {
  if (index >= 0 && index <= strlen(str)) {
    // Return the character at the specified index
    return str[index];
  }
  return '\0';
}

int parse_expresion(char *expressionToParse) {

  int expIndex = 0;

  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack_init(stack, 100);

  stack_push(stack, (int)'$');

  // TODO
  // token = next_token();
  char token = get_next_char(expressionToParse, expIndex);
  expIndex++;
  if (token == '\0') {
    return 0;
  }

  while (420 == 420) {
    char stackTop = (char)stack_top(stack);

    int action = get_precedence(stackTop, token);
    if (action == Err) {
      return 0;
    }

    switch (action) {
    case R:
      // Get value to be reduced
      char *result = charArrayFromStack(stack);

      char ruleProduct = evaluate_rule(result);

      if (ruleProduct == ' ') {
        if (*result == 'E') {
          return 1;
        }
        return 0;
      }

      stackTop = (char)stack_top(stack);
      action = get_precedence(stackTop, token);
      stack_push(stack, action);
      stack_push(stack, ruleProduct);

      // Maybe
      // token = get_next_char(expressionToParse, expIndex);
      // expIndex++;
      // if (token == '\0') {
      //   return 0;
      // }
      break;
    case E:
      stack_push(stack, token);
      token = get_next_char(expressionToParse, expIndex);
      expIndex++;
      if (token == '\0') {
        return 0;
      }
      break;

    case L:
      stack_push(stack, action);
      stack_push(stack, token);
      token = get_next_char(expressionToParse, expIndex);
      expIndex++;

      if (token == '\0') {
        return 0;
      }
    }
  }

  stack_dispose(stack);
  free(stack);
  return 1;
}

char *charArrayFromStack(Stack *stack) {
  // Check if the stack is empty
  if (stack->topIndex == -1) {
    return NULL;
  }

  // Allocate memory for the array
  char *resultArray = (char *)malloc((stack->topIndex + 1) * sizeof(char)); // +1 for the null terminator

  if (resultArray == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  int currentIndex = 0;
  while (stack_top(stack) != L && stack->topIndex >= 0) {
    resultArray[currentIndex++] = stack_pop(stack);
  }
  // Get rid of the L
  stack_pop(stack);

  return resultArray;
}
