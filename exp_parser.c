//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"
#include "stack.h"

#define TABLE_SIZE 4

char expression[] = "1 + 1";

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

int get_precedence(char stack_top, char input) {
  switch (stack_top) {
  case '+':
    switch (input) {
    case '+':
      return precedence_table[0][0];
    case 'i':
      return precedence_table[0][1];
    case '$':
      return precedence_table[0][2];
    case 'E':
      return precedence_table[0][3];
    }
  case 'i':
    switch (input) {
    case '+':
      return precedence_table[1][0];
    case 'i':
      return precedence_table[1][1];
    case '$':
      return precedence_table[1][2];
    case 'E':
      return precedence_table[1][3];
    }
  case '$':
    switch (input) {
    case '+':
      return precedence_table[2][0];
    case 'i':
      return precedence_table[2][1];
    case '$':
      return precedence_table[2][2];
    case 'E':
      return precedence_table[2][3];
    }
  case 'E':
    switch (input) {
    case '+':
      return precedence_table[3][0];
    case 'i':
      return precedence_table[3][1];
    case '$':
      return precedence_table[3][2];
    case 'E':
      return precedence_table[3][3];
    }
  }

  return precedence_table[stack_top][input];
}

char evaluateRule(char *rightSide) {

  if (strcmp(rightSide, "E+E") == 0) {
    return 'E';
  } else if (strcmp(rightSide, "i") == 0) {
    return 'E';
  }
  return ' ';
}

char getNextChar(char *str, int index) {
  if (index >= 0 && index <= strlen(str)) {
    // Return the character at the specified index
    return str[index];
  }
  return '\0';
}

int ParseExpression(char *expressionToParse) {

  int eIndex = 0;

  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack_init(stack, 100);

  stack_push(stack, (int)'$');

  // TODO
  // token = next_token();
  char token = getNextChar(expressionToParse, eIndex);
  eIndex++;
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

      char ruleProduct = evaluateRule(result);

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
      // token = getNextChar(expressionToParse, eIndex);
      // eIndex++;
      // if (token == '\0') {
      //   return 0;
      // }
      break;
    case E:
      stack_push(stack, token);
      token = getNextChar(expressionToParse, eIndex);
      eIndex++;
      if (token == '\0') {
        return 0;
      }
      break;

    case L:
      stack_push(stack, action);
      stack_push(stack, token);
      token = getNextChar(expressionToParse, eIndex);
      eIndex++;

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
