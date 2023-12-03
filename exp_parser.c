//
// Created by padak on 10/26/23.
//

#include "exp_parser.h"

#include <string.h>

#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"
#include "str.h"
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
    {L, L, L, L, X, X, X, X, X, X, L, R, X, X, L, R, L}, // ??
    {R, R, R, R, X, X, X, X, X, X, X, R, X, X, R, R, X}, // !
    {R, R, R, R, R, R, R, R, R, R, X, R, R, R, R, R, X}, // i
    {L, L, L, L, L, L, L, L, L, L, L, X, L, L, L, X, L}, // $
    {E, E, E, E, E, E, E, E, E, E, L, R, L, L, X, R, X}  // E
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
    {TOKEN_EXPRESSION, 5, {TOKEN_LPAREN, TOKEN_EXPRESSION, TOKEN_PLUS, TOKEN_EXPRESSION, TOKEN_RPAREN}},

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
  case TOKEN_KEYWORD:
    return 14;
  case TOKEN_STACK_BOTTOM:
    return 15;

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

Stack_token_t evaluate_rule(Stack_token_t token, SymtableIdentifierType type) {
  switch (token.token.type) {
  case TOKEN_INTEGER_LITERAL:
    return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = {.data_type = INT_TYPE, .nullable = false}};

  case TOKEN_DECIMAL_LITERAL:
    return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = {.data_type = DOUBLE_TYPE, .nullable = false}};

  case TOKEN_STRING_LITERAL:
    return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = {.data_type = STRING_TYPE, .nullable = false}};
  case TOKEN_IDENTIFIER: {
    break;
  }
  case TOKEN_KEYWORD: {
    if (token.token.keyword == KW_NIL) {
      return (Stack_token_t){.token = {TOKEN_EXPRESSION, token.token.keyword, "E", 1}, .precedence = None, .type = {.data_type = VOID_TYPE, .nullable = true}};
    }
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
    return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = type};
  }
  case TOKEN_NULL_COALESCING: {
    // TODO type check
    return (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = type};
  }

  default:
    break;
  }
  return (Stack_token_t){.token = {TOKEN_EOF, KW_UNKNOWN, "Eof", 3}, .precedence = None, .type = {.data_type = UNKNOWN_TYPE, .nullable = false}};
}

void pushToStack(void_stack_t *expresionStack, Stack_token_t action, Stack_token_t token1, Stack_token_t token2) {
  switch (action.token.type) {
  case TOKEN_PLUS:
    stack_push(expresionStack, str_new_from_cstr("+"));
    break;
  case TOKEN_MINUS:
    stack_push(expresionStack, str_new_from_cstr("-"));
    break;
  case TOKEN_MULTIPLY:
    stack_push(expresionStack, str_new_from_cstr("*"));
    break;
  case TOKEN_DIV:
    if (token1.type.data_type == INT_TYPE && token2.type.data_type == INT_TYPE) {
      stack_push(expresionStack, str_new_from_cstr(":"));
    } else if (token1.type.data_type == DOUBLE_TYPE && token2.type.data_type == DOUBLE_TYPE) {
      stack_push(expresionStack, str_new_from_cstr("/"));
    }
    break;
  case TOKEN_LT:
    stack_push(expresionStack, str_new_from_cstr("<"));
    break;
  case TOKEN_LE:
    stack_push(expresionStack, str_new_from_cstr("<="));
    break;
  case TOKEN_GT:
    stack_push(expresionStack, str_new_from_cstr(">"));
    break;
  case TOKEN_GE:
    stack_push(expresionStack, str_new_from_cstr(">="));
    break;
  case TOKEN_EQ:
    stack_push(expresionStack, str_new_from_cstr("=="));
    break;
  case TOKEN_NEQ:
    stack_push(expresionStack, str_new_from_cstr("!="));
    break;
  case TOKEN_NULL_COALESCING:
    stack_push(expresionStack, str_new_from_cstr("??"));
    break;
    // TODO NOT
    // case TOKEN_NOT:
    //   stack_push(expresionStack, str_new_from_cstr("!"));
    //   break;

  default:
    break;
  }
  if (token1.token.type != TOKEN_EXPRESSION) {
    if (token1.token.type == TOKEN_IDENTIFIER) {
      stack_push(expresionStack, str_new_from_cstr(token1.token.val));
    } else if (token1.token.type == TOKEN_INTEGER_LITERAL) {
      stack_push(expresionStack, str_new_int_const(token1.token.val));
    } else if (token1.token.type == TOKEN_DECIMAL_LITERAL) {
      stack_push(expresionStack, str_new_double_const(token1.token.val));
    } else if (token1.token.type == TOKEN_STRING_LITERAL) {
      stack_push(expresionStack, str_new_string_const(token1.token.val));
    } else if (token1.token.type == TOKEN_KEYWORD && token1.token.keyword == KW_NIL) {
      stack_push(expresionStack, str_new_nil_const());
    }
  }

  if (token2.token.type != TOKEN_EXPRESSION) {
    if (token2.token.type == TOKEN_IDENTIFIER) {
      stack_push(expresionStack, str_new_from_cstr(token2.token.val));
    } else if (token2.token.type == TOKEN_INTEGER_LITERAL) {
      stack_push(expresionStack, str_new_int_const(token2.token.val));
    } else if (token2.token.type == TOKEN_DECIMAL_LITERAL) {
      stack_push(expresionStack, str_new_double_const(token2.token.val));
    } else if (token2.token.type == TOKEN_STRING_LITERAL) {
      stack_push(expresionStack, str_new_string_const(token2.token.val));
    } else if (token2.token.type == TOKEN_KEYWORD && token2.token.keyword == KW_NIL) {
      stack_push(expresionStack, str_new_nil_const());
    }
  }
}

#ifdef PARSER_TEST
Stack_token_t get_next_token_wrap(Token array[], int index, int size) {
#else
Stack_token_t get_next_token_wrap(TokenType previousToken, Parser *parser) {
#endif

#ifdef PARSER_TEST
  if (index >= 0 && index < size) {
    // Return the character at the specified index

    // Check if the token is valid
    int tokenIndex = get_operator_index(array[index].type);

    if (tokenIndex == -1) {
      // exit_with_error(SYNTAX_ERR, "bad token");

      return (Stack_token_t){.token = {TOKEN_STACK_BOTTOM, KW_UNKNOWN, "$", 1}, .precedence = None};
    }

    Token token = array[index];
    return (Stack_token_t){.precedence = None, .token = token};
  }
  return (Stack_token_t){.token = TOKEN_STACK_BOTTOM, .precedence = None};
#else
  Token *peakToken = peek(parser);

  // Check if the token is valid
  int tokenIndex = get_operator_index(peakToken->type);

  if (tokenIndex == -1) {
    // exit_with_error(SYNTAX_ERR, "bad token");

    return (Stack_token_t){.token = {TOKEN_STACK_BOTTOM, KW_UNKNOWN, "$", 1}, .precedence = None};
  }

  // Identifier check
  if (peakToken->type == TOKEN_IDENTIFIER && previousToken == TOKEN_IDENTIFIER) {
    return (Stack_token_t){.token = {TOKEN_STACK_BOTTOM, KW_UNKNOWN, "$", 1}, .precedence = None};
  }
  // Peaked token is valid, consume it
  advance(parser);

  if (check_type(parser, TOKEN_IDENTIFIER)) {
    // Get type
    SymtableItem *result = search_var_in_tables(parser, peakToken->val);

    if (result == NULL) {
      exit_with_error(SEMANTIC_ERR_VAR, "Variable not declared");
    }

    // int result = get_next_token(token);
    return (Stack_token_t){.token = *peakToken, .precedence = None, .type = result->data->variable.identifier_type};
  }
  return (Stack_token_t){.token = *peakToken, .precedence = None, .type = {.data_type = UNKNOWN_TYPE, .nullable = false}};

#endif
}

#ifndef PARSER_TEST
Stack_token_t get_current_token_wrap(Parser *parser) {
  Token *token = current_token(parser);

  if (check_type(parser, TOKEN_IDENTIFIER)) {
    // Get type
    SymtableItem *result = search_var_in_tables(parser, token->val);

    if (result == NULL) {
      exit_with_error(SEMANTIC_ERR_VAR, "Variable not declared");
    }

    return (Stack_token_t){.token = *token, .precedence = None, .type = result->data->variable.identifier_type};
  }
  return (Stack_token_t){.token = *token, .precedence = None, .type = {.data_type = UNKNOWN_TYPE, .nullable = false}};
}

#endif

void handle_shift_case(void_stack_t *stack, Stack_token_t token) {
  Stack_token_t *new_token = malloc(sizeof(Stack_token_t));

  *new_token = token;
  stack_push(stack, new_token);
}

int handle_reduce_case(void_stack_t *stack, Stack_token_t token, Stack_token_t precedence, void_stack_t *expresionStack) {
  while (precedence.precedence == R) {
    Stack_token_t firstToken = *(Stack_token_t *)stack_pop(stack);

    Stack_token_t *ruleProduct = malloc(sizeof(Stack_token_t));

    *ruleProduct = evaluate_rule(firstToken, firstToken.type);

    // Not a simple reduction, find in the rest of the rules
    if (ruleProduct->token.type == TOKEN_EOF) {
      Stack_token_t secondToken = *(Stack_token_t *)stack_pop(stack);
      Stack_token_t thirdToken = *(Stack_token_t *)stack_pop(stack);

      // E+E
      if (firstToken.token.type == TOKEN_EXPRESSION && thirdToken.token.type == TOKEN_EXPRESSION) {
        // Check nil type and ??
        if (firstToken.token.keyword == KW_NIL && secondToken.token.type == TOKEN_NULL_COALESCING) {
          *ruleProduct = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = thirdToken.type};

        } else if (thirdToken.token.keyword == KW_NIL && secondToken.token.type == TOKEN_NULL_COALESCING) {
          *ruleProduct = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = firstToken.type};

        } else if (firstToken.type.data_type != thirdToken.type.data_type) {
          if (firstToken.type.data_type == INT_TYPE && thirdToken.type.data_type == DOUBLE_TYPE || firstToken.type.data_type == DOUBLE_TYPE && thirdToken.type.data_type == INT_TYPE) {
            *ruleProduct = evaluate_rule(secondToken, (SymtableIdentifierType){.data_type = DOUBLE_TYPE, .nullable = false});
          } else {
            exit_with_error(SYNTAX_ERR, "Cannot use operator on different types");
          }
        }
        *ruleProduct = evaluate_rule(secondToken, firstToken.type);
        pushToStack(expresionStack, secondToken, firstToken, thirdToken);
      }
      //(E)
      if (firstToken.token.type == TOKEN_RPAREN && thirdToken.token.type == TOKEN_LPAREN) {
        if (secondToken.token.type == TOKEN_EXPRESSION) {
          *ruleProduct = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = {.data_type = INT_TYPE, .nullable = false}};
        }
      }
      // E!
      if (firstToken.token.type == TOKEN_NOT && (secondToken.token.type == TOKEN_EXPRESSION)) {
        *ruleProduct = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = firstToken.type};
        // Return third token
        stack_push(stack, &thirdToken);
      }
    }

    if (ruleProduct->token.type == TOKEN_EOF) {
      exit_with_error(SYNTAX_ERR, "No rule found");
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
Token parse_expression(Token *testExpressionToParse, int inputSize, Parser *parser, void_stack_t *expresionStack) {
  int expIndex = 0;
#else
SymtableIdentifierType parse_expression(Parser *parser, void_stack_t *expresionStack) {
#endif

  void_stack_t *stack = stack_new(8192);
  stack_push(stack, &(Stack_token_t){.token = {TOKEN_STACK_BOTTOM, KW_UNKNOWN, "$", 1}, .precedence = None, .type = {.data_type = UNKNOWN_TYPE, .nullable = false}});

#ifdef PARSER_TEST
  Stack_token_t token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
  expIndex++;
#else
  Stack_token_t token = get_current_token_wrap(parser);
#endif

  while (420 == 420) {
    Stack_token_t stackTop = *(Stack_token_t *)stack_top(stack);

    // Empty expresion
    if (token.token.type == TOKEN_STACK_BOTTOM && stackTop.token.type == TOKEN_STACK_BOTTOM) {
      Stack_token_t *token = malloc(sizeof(Stack_token_t));

      *token = (Stack_token_t){.token = {TOKEN_EXPRESSION, KW_UNKNOWN, "E", 1}, .precedence = None, .type = {.data_type = VOID_TYPE, .nullable = false}};

      stack_push(stack, token);

      break;
    }

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

    Stack_token_t action = get_precedence(stackTop, token);

    switch (action.precedence) {
    case L:
      handle_shift_case(stack, token);
#ifdef PARSER_TEST
      token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
      expIndex++;
#else
      token = get_next_token_wrap(token.token.type, parser);
#endif

      break;

    case R:
      // If there is no rule, expression is not valid
      if (handle_reduce_case(stack, token, action, expresionStack) == -1) {
        fprintf(stderr, "No rule for token %d\n", token.token.type);
        exit_with_error(SYNTAX_ERR, "Invalid expression");
      }
      break;

    case E:
      handle_equals_case(stack, token);
#ifdef PARSER_TEST
      token = get_next_token_wrap(testExpressionToParse, expIndex, inputSize);
      expIndex++;
#else
      token = get_next_token_wrap(token.token.type, parser);
#endif

      break;

    case X: {
      if (stackTop.token.type == TOKEN_STACK_BOTTOM && token.token.type == TOKEN_STACK_BOTTOM) {
        break;
        // Stack_token_t tempToken = *(Stack_token_t *)stack_top(stack);
        // ;
        // if (tempToken.token.type == TOKEN_EXPRESSION) {
        //   break;
        // }
      }
      fprintf(stderr, "Invalid relation between %s and %s\n", stackTop.token.val, token.token.val);
      exit_with_error(SYNTAX_ERR, "Invalid expression");
    } break;

    case None:
      break;
    }
  }

#ifdef PARSER_TEST
  Token *result = malloc(sizeof(Token));
  *result = ((Stack_token_t *)stack_top(stack))->token;

  y_eet(expresionStack);
  stack_dispose(stack);
  free(stack);
  return *result;
#else
  SymtableIdentifierType *result = malloc(sizeof(SymtableIdentifierType));
  *result = ((Stack_token_t *)stack_top(stack))->type;

  // Advance to next token to end the expreison
  advance(parser);

  y_eet(expresionStack);
  stack_dispose(stack);
  free(stack);
  return *result;
#endif
}

void y_eet(void_stack_t *stack) {
  void_stack_t *temp_stack = stack_new(stack->size);

  while (stack->top_index != -1) {
    str *el = stack_pop(stack);
    stack_push(temp_stack, el);
  }

  while (temp_stack->top_index != -1) {
    str *el = stack_pop(temp_stack);

    if (strcmp(el->data, "<=") == 0 || strcmp(el->data, ">=") == 0) {
      stack_push(stack, str_new_from_cstr("?1-"));
      stack_push(stack, str_new_from_cstr("?2-"));

      stack_push(stack, str_new_from_cstr("?1"));
      stack_push(stack, str_new_from_cstr("?2"));

      stack_push(stack, str_new_from_cstr(el->data[0] == '<' ? "<" : ">"));

      stack_push(stack, str_new_from_cstr("?1"));
      stack_push(stack, str_new_from_cstr("?2"));

      stack_push(stack, str_new_from_cstr("="));

      stack_push(stack, str_new_from_cstr("|"));
    }

    stack_push(stack, el);
  }

  stack_dispose(temp_stack);
}
