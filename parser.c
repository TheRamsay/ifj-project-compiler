#include "parser.h"

#include "exp_parser.h"
#include "string.h"
#include "error.h"

#ifdef PARSER_TEST
#define DEFAULT_EXPRESION_TYPE \
  (SymtableIdentifierType) { .data_type = INT_TYPE, .nullable = true }

#define DEFAULT_IF_EXPRESION_TYPE \
  (SymtableIdentifierType) { .data_type = BOOL_TYPE, .nullable = false }
#endif

#define TOKEN_BUFFER_LEN 2
#define LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK 42069 + 2  // Special prime number that is optimal for this use case

void check_identifier(char* identifier) {
  if (strcmp(identifier, "_") == 0) {
    exit_with_error(SYNTAX_ERR, "Identifier cannot be '_'");
  }
}

bool parser_init(Parser* parser) {
  parser->global_table = symtable_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
  parser->local_tables_stack = stack_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
  parser->buffer_active = false;
  parser->token_buffer = calloc(TOKEN_BUFFER_LEN, sizeof(Token));
  parser->in_function = false;
  parser->in_scope = false;
  parser->after_return = false;
  parser->current_function_name = NULL;

  parser->semantic_enabled = false;
  parser->tokens = NULL;

  parser->line = 1;
  parser->col = 1;

  if (parser->token_buffer == NULL) {
    return false;
  }

  parser->gen = generator_new();

  if (parser->gen == NULL) {
    return false;
  }

  builtin_add_to_symtable(parser->global_table);

#ifdef PARSER_TEST
  parser->input_tokens = NULL;
  parser->input_index = 0;
  parser->output_tokens = calloc(1000, sizeof(Token));

  if (parser->output_tokens == NULL) {
    return false;
  }

  parser->output_index = 0;
#endif

  return true;
}

SymtableItem* search_var_in_tables(Parser* parser, char* key) {
  SymtableItem* result = NULL;

  int i = parser->local_tables_stack->top_index;
  while (i >= 0) {
    result = symtable_get(parser->local_tables_stack->items[i], key);
    if (result != NULL) {
      break;
    }
    i--;
  }

  if (result == NULL) {
    result = symtable_get(parser->global_table, key);
  }


  return result;
}

void push_param_to_stack(void_stack_t* params_stack, Token* first, Token* second) {
  // only val
  if (second == NULL) {
    if (first->type == TOKEN_IDENTIFIER) {
      stack_push(params_stack, str_new_from_cstr(first->val));
    }
    else if (first->type == TOKEN_DECIMAL_LITERAL) {
      stack_push(params_stack, str_new_float_const(first->val));
    }
    else if (first->type == TOKEN_INTEGER_LITERAL) {
      stack_push(params_stack, str_new_int_const(first->val));
    }
    else if (first->type == TOKEN_STRING_LITERAL) {
      stack_push(params_stack, str_new_string_const(first->val));
    }
    else if (first->type == TOKEN_KEYWORD && first->keyword == KW_NIL) {
      stack_push(params_stack, str_new_nil_const());
    }
  }
  // named param and val
  else {
    if (second->type == TOKEN_IDENTIFIER) {
      stack_push(params_stack, str_new_from_cstr(second->val));
    }
    else if (second->type == TOKEN_DECIMAL_LITERAL) {
      stack_push(params_stack, str_new_float_const(second->val));
    }
    else if (second->type == TOKEN_INTEGER_LITERAL) {
      stack_push(params_stack, str_new_int_const(second->val));
    }
    else if (second->type == TOKEN_STRING_LITERAL) {
      stack_push(params_stack, str_new_string_const(second->val));
    }
    else if (second->type == TOKEN_KEYWORD && second->keyword == KW_NIL) {
      stack_push(params_stack, str_new_nil_const());
    }
  }
}

bool is_term(Parser* parser) {
  return check_type(parser, TOKEN_IDENTIFIER) || check_type(parser, TOKEN_STRING_LITERAL) || check_type(parser, TOKEN_INTEGER_LITERAL)
    || check_type(parser, TOKEN_DECIMAL_LITERAL) || check_keyword(parser, KW_NIL);
}

bool is_term_(Token* token) {
  return token->type == TOKEN_IDENTIFIER || token->type == TOKEN_STRING_LITERAL || token->type == TOKEN_INTEGER_LITERAL || token->type == TOKEN_DECIMAL_LITERAL
    || (token->type == TOKEN_KEYWORD && token->keyword == KW_NIL);
}

bool is_literal_(Token* token) {
  return token->type == TOKEN_STRING_LITERAL || token->type == TOKEN_INTEGER_LITERAL || token->type == TOKEN_DECIMAL_LITERAL
    || (token->type == TOKEN_KEYWORD && token->keyword == KW_NIL);
}

bool is_literal(Parser* parser) {
  return check_type(parser, TOKEN_STRING_LITERAL) || check_type(parser, TOKEN_INTEGER_LITERAL) || check_type(parser, TOKEN_DECIMAL_LITERAL)
    || check_keyword(parser, KW_NIL);
}

bool compare_symtable_item_with_token(Token* token, SymtableIdentifierType identifier_type) {
  if (identifier_type.nullable) {
    if (identifier_type.data_type == INT_TYPE && (token->type == TOKEN_INTEGER_LITERAL || token->keyword == KW_NIL)) {
      return true;
    }

    if (identifier_type.data_type == STRING_TYPE && (token->type == TOKEN_STRING_LITERAL || token->keyword == KW_NIL)) {
      return true;
    }

    if (identifier_type.data_type == DOUBLE_TYPE && (token->type == TOKEN_DECIMAL_LITERAL || token->keyword == KW_NIL)) {
      return true;
    }

    return false;
  }

  if (identifier_type.data_type == INT_TYPE && (token->type == TOKEN_INTEGER_LITERAL)) {
    return true;
  }

  if (identifier_type.data_type == STRING_TYPE && (token->type == TOKEN_STRING_LITERAL)) {
    return true;
  }

  if (identifier_type.data_type == DOUBLE_TYPE && (token->type == TOKEN_DECIMAL_LITERAL)) {
    return true;
  }

  return false;
}

bool compare_symtable_item_types(SymtableIdentifierType left, SymtableIdentifierType right) {
  if (left.data_type == DOUBLE_TYPE && right.data_type == INT_TYPE && !right.nullable) {
    return true;
  }

  if (left.nullable) {
    return left.data_type == right.data_type || right.data_type == VOID_TYPE;
  }
  else {
    return left.data_type == right.data_type && right.nullable == left.nullable;
  }
}

// Checks if current statement stars on new line
void is_valid_statement(Parser* parser) {
  if (current_token(parser)->after_newline) {
    return;
  }

  pexit_with_error(parser, SYNTAX_ERR, "Multiple statements must be on separate lines");
}

// Returns current token
Token* current_token(Parser* parser) { return parser->tokens->token; }

Token* peek(Parser* parser) {
  if (parser->tokens->next == NULL) {
    return NULL;
  }

  return parser->tokens->next->token;
}

// Check if current token is of the expected type and advance if it is
bool match(Parser* parser, TokenType token_type, bool check_new_line) {
  if (check_type(parser, token_type)) {
    if (check_new_line && !current_token(parser)->after_newline) {
      pexit_with_error(parser, SYNTAX_ERR, "Multiple statements must be on separate lines");
    }

    advance(parser);
    return true;
  }

  return false;
}

bool match_keyword(Parser* parser, KeywordType keyword, bool check_new_line) {
  if (check_keyword(parser, keyword)) {
    if (check_new_line && !current_token(parser)->after_newline) {
      pexit_with_error(parser, SYNTAX_ERR, "Multiple statements must be on separate lines");
    }

    advance(parser);
    return true;
  }

  return false;
}

// Check if value of keyword is equal to the expected keyword
bool check_keyword(Parser* parser, KeywordType keyword) { return current_token(parser)->type == TOKEN_KEYWORD && current_token(parser)->keyword == keyword; }

// Advance to next token
Token* advance(Parser* parser) {
  if (parser->tokens->next == NULL) {
    return NULL;
  }

  parser->tokens = parser->tokens->next;

  if (parser->tokens->token->val != NULL) {
    parser->col += strlen(parser->tokens->token->val);
  }

  if (parser->tokens->token->after_newline) {
    parser->line++;
    parser->col = 1;
  }

  return parser->tokens->token;
}

// Returns true if current token is of the expected type
bool check_type(Parser* parser, TokenType token_type) { return current_token(parser)->type == token_type; }

// Check if current token is of the expected type and advance if it is,
// otherwise print error message and exit
Token consume(Parser* parser, TokenType token_type, char* error_msg) {
  if (!check_type(parser, token_type)) {
    // #ifdef PARSER_TEST
    // pexit_with_error(parser, SYNTAX_ERR, "current_token %d (value: %s) | next token %d
    // (value: %s) | %s",
    // current_token(parser)->keyword, current_token(parser)->val,
    // peek(parser)->type, peek(parser)->val, error_msg); #else
    pexit_with_error(parser, SYNTAX_ERR, "%s", error_msg);
    // #endif
  }

  Token token = *current_token(parser);
  advance(parser);
  return token;
}

bool is_datatype(Parser* parser) {
  return check_keyword(parser, KW_INT) || check_keyword(parser, KW_FLOAT) || check_keyword(parser, KW_DOUBLE) || check_keyword(parser, KW_STRING);
}

SymtableIdentifierType parse_datatype(Parser* parser) {
  SymtableDataType type;

  Token token = *current_token(parser);

  switch (token.keyword) {
  case KW_INT:
    type = INT_TYPE;
    break;
  case KW_FLOAT:
    type = DOUBLE_TYPE;
    break;
  case KW_DOUBLE:
    type = DOUBLE_TYPE;
    break;
  case KW_STRING:
    type = STRING_TYPE;
    break;
  default:
    type = UNKNOWN_TYPE;
    break;
  }

  SymtableIdentifierType identifier_type = (SymtableIdentifierType){ .data_type = type, .nullable = token.is_nullable };
  advance(parser);

  return identifier_type;
}

void return_def(Parser* parser, SymtableItem* item) {
  if (check_type(parser, TOKEN_LBRACE)) {
    symtable_add_return(item, (SymtableIdentifierType) { .data_type = VOID_TYPE, .nullable = false });
    return;
  }

  if (!match(parser, TOKEN_ARROW, false)) {
    pexit_with_error(parser, SYNTAX_ERR, "missing '->' in function definition");
  }

  // Check if return type is valid datatype
  if (!is_datatype(parser)) {
    pexit_with_error(parser, SYNTAX_ERR, "expected datatype after '->'");
  }

  symtable_add_return(item, parse_datatype(parser));
}

void func_params_n(Parser* parser, SymtableItem* item, Symtable* local_table, void_stack_t* stack) {
  if (!match(parser, TOKEN_COMMA, false)) {
    return;
  }

  char* out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier").val;
  // IN identifier for function param
  char* in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier").val;

  // stack_push(stack, in_param_id);

  consume(parser, TOKEN_COLON, "Expected : ");

  if (!is_datatype(parser)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected datatype after ':'");
  }

  SymtableIdentifierType data_type = parse_datatype(parser);

  symtable_add_param(item, out_param_id, in_param_id, data_type);
  SymtableItem* parameter_var_item = symtable_add_symbol(local_table, in_param_id, SYMTABLE_VARIABLE, true, false, true);
  parameter_var_item->data->variable.identifier_type = data_type;

  func_params_n(parser, item, local_table, stack);
}

void func_params(Parser* parser, SymtableItem* item, Symtable* local_table, void_stack_t* stack) {
  // If closing paren, then no params
  if (check_type(parser, TOKEN_RPAREN)) {
    // func <func_id> ()
    return;
  }

  // OUT identifier for function param
  char* out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier").val;
  // IN identifier for function param
  char* in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier").val;

  // stack_push(stack, in_param_id);

  consume(parser, TOKEN_COLON, "Expected : ");

  // Just consume type, idk what to do with it yet
  if (!is_datatype(parser)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected datatype after ':'");
  }

  SymtableIdentifierType data_type = parse_datatype(parser);

  symtable_add_param(item, out_param_id, in_param_id, data_type);
  SymtableItem* parameter_var_item = symtable_add_symbol(local_table, in_param_id, SYMTABLE_VARIABLE, true, false, true);
  parameter_var_item->data->variable.identifier_type = data_type;

  func_params_n(parser, item, local_table, stack);
}

// function_def -> func FUNC_ID ( <func_params> ) <return_def> {
// <statement_list> }
void func_def(Parser* parser) {
  Symtable* local_table = symtable_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
  stack_push(parser->local_tables_stack, local_table);

  parser->in_function = true;
  char* key = consume(parser, TOKEN_IDENTIFIER, "Expected identifier").val;
  parser->current_function_name = key;

  // If we are in semantic analysis, we don't parse the function header, it's
  // already parsed`
  if (parser->semantic_enabled) {
    Token* token = current_token(parser);

    while (token != NULL && token->type != TOKEN_EOF && token->type != TOKEN_LBRACE) {
      token = advance(parser);
    }

    if (current_token(parser)->type == TOKEN_LBRACE) {
      advance(parser);
    }

    SymtableItem* func_item = search_var_in_tables(parser, key);

    if (func_item == NULL) {
      pexit_with_error(parser, INTERNAL_ERROR, "Function wasn't parsed by first pass (🙃)");
    }

    void_stack_t* stack = stack_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);

    SymtableParam* param = func_item->data->function.params;

    while (param != NULL) {
      stack_push(stack, str_new_from_cstr(param->in_name));
      param = param->next;
    }

    generator_function_begin(parser->gen, str_new_from_cstr(key), stack);

  }
  else if (!parser->semantic_enabled) {
    SymtableItem* item = symtable_add_symbol(parser->global_table, key, SYMTABLE_FUNCTION, true, false, false);
    void_stack_t* stack = stack_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
    consume(parser, TOKEN_LPAREN, "Expected '('");
    func_params(parser, item, local_table, stack);
    consume(parser, TOKEN_RPAREN, "Expected ')'");
    return_def(parser, item);
    consume(parser, TOKEN_LBRACE, "Expected '{'");

    Token* token = current_token(parser);

    while (token != NULL && token->type != TOKEN_EOF && token->keyword != KW_FUNC) {
      token = advance(parser);
    }

    return;
  }

  SymtableItem* current_func = search_var_in_tables(parser, parser->current_function_name);

  if (current_func == NULL) {
    pexit_with_error(parser, INTERNAL_ERROR, "Current func is NULL");
  }

  if (!body(parser) && current_func->data->function._return->identifier_type.data_type != VOID_TYPE) {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "Function control flow does lead to invalid return");
  }

  consume(parser, TOKEN_RBRACE, "Expected '}'");

  stack_pop(parser->local_tables_stack);
  parser->in_function = false;
  parser->current_function_name = NULL;
  generator_function_end(parser->gen, NULL);
}

void check_call_param(Parser* parser, SymtableParam* param, Token* first, Token* second, void_stack_t* params_stack) {
  // if (!is_term(parser))
  // {
  // 	pexit_with_error(parser, SYNTAX_ERR, "Expected term in function call parameter");
  // }

  // Parameter is defined without out identifier, but is called with it
  if (param == NULL) {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "Too many parameters in function call");
  }

  if (strcmp(param->out_name, "_") == 0 && second != NULL) {
    pexit_with_error(parser, SEMANTIC_ERR, "Parameter %s doesn't have out definition", first->val);
  }
  // Parameter out identifier doesn't match the call
  else if (strcmp(param->out_name, "_") != 0 && second == NULL) {
    pexit_with_error(parser, SEMANTIC_ERR, "Parameter %s doesn't match the out definition", first->val);
  }
  else if (strcmp(param->out_name, "_") != 0 && strcmp(param->out_name, first->val) != 0) {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "Parameter %s doesn't match the out definition", first->val);
  }
  // Valid call with out identifier
  else if (second != NULL) {
    // Parameter value is a first now (HEHHEHHEHE)
    first = second;
  }

  // Parameter is a variable
  if (first->type == TOKEN_IDENTIFIER) {
    SymtableItem* param_variable = search_var_in_tables(parser, first->val);

    // If variable is not defined
    if (param_variable == NULL) {
      pexit_with_error(parser, SEMANTIC_ERR_VAR, "Variable %s is not defined", first->val);
    }
    else if (!param_variable->data->variable.initialized) {
      pexit_with_error(parser, SEMANTIC_ERR_VAR, "Variable %s is not initialized", first->val);
    }
    else if (!compare_symtable_item_types(param_variable->data->variable.identifier_type, param->identifier_type)) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Variable %s is not of the expected type", first->val);
    }

    // If variable is a function
    if (param_variable->data->type == SYMTABLE_FUNCTION) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Cannot pass function as parameter");
    }
  }
  else if (is_literal_(first)) {
    if (!compare_symtable_item_with_token(first, param->identifier_type)) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Parameter %s type doesn't match the definition", first->val);
    }
  }
  else {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "Parameter %s is weird", first->val);
  }

  push_param_to_stack(params_stack, first, second);
}

void check_call_param_write(Parser* parser, Token* first, Token* second, void_stack_t* params_stack) {
  if (second != NULL) {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "Write function doesn't have named parameters");
  }

  // Parameter is a variable
  if (first->type == TOKEN_IDENTIFIER) {
    SymtableItem* param_variable = search_var_in_tables(parser, first->val);

    // If variable is not defined
    if (param_variable == NULL) {
      pexit_with_error(parser, SEMANTIC_ERR_VAR, "Variable %s is not defined", first->val);
    }
    else if (!param_variable->data->variable.initialized) {
      pexit_with_error(parser, SEMANTIC_ERR_VAR, "Variable %s is not initialized", first->val);
    }

    // If variable is a function
    if (param_variable->data->type == SYMTABLE_FUNCTION) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Cannot pass function as parameter");
    }
  }

  push_param_to_stack(params_stack, first, second);
}

// call_params_n -> , <call_params_kw> <term> <call_params_n>
int call_params_n(Parser* parser, SymtableItem* item, SymtableParam* param, void_stack_t* params_stack) {
  if (check_type(parser, TOKEN_RPAREN)) {
    if (param != NULL) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Too few parameters in function call");
    }

    return 0;
  }

  consume(parser, TOKEN_COMMA, "Expected ','");

  Token first = *current_token(parser);
  char* first_temp = first.val;

  first.val = malloc(sizeof(char) * (strlen(first.val) + 1));

  if (first.val == NULL) {
    pexit_with_error(parser, INTERNAL_ERROR, "Malloc failed");
  }

  strcpy(first.val, first_temp);

  Token* second = NULL;

  // Func call param name or value
  if (!is_term(parser)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected term in function call parameter");
  }

  advance(parser);

  // Named func call param
  if (match(parser, TOKEN_COLON, false)) {
    second = &(*current_token(parser));
    advance(parser);
  }

  // Disable some semantic checks for write function
  if (strcmp(item->key, "write") != 0) {
    check_call_param(parser, param, &first, second, params_stack);
    return 1 + call_params_n(parser, item, param->next, params_stack);
  }
  else {
    check_call_param_write(parser, &first, second, params_stack);
    return 1 + call_params_n(parser, item, NULL, params_stack);
  }
}

// call_params -> <call_params_kw> <term> <call_params_n>
int call_params(Parser* parser, SymtableItem* item, SymtableParam* param, void_stack_t* params_stack) {
  if (check_type(parser, TOKEN_RPAREN)) {
    if (param != NULL) {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Too few parameters in function call");
    }

    return 0;
  }

  if (param == NULL && strcmp(item->key, "write") != 0) {
    pexit_with_error(parser, SEMANTIC_ERR_CALL, "No parameters in func signature");
  }

  Token first = *current_token(parser);
  char* first_temp = first.val;

  first.val = malloc(sizeof(char) * (strlen(first.val) + 1));
  if (first.val == NULL) {
    pexit_with_error(parser, INTERNAL_ERROR, "Malloc failed");
  }
  strcpy(first.val, first_temp);

  Token* second = NULL;

  // Func call param name or value
  if (!is_term(parser)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected term in function call parameter");
  }

  advance(parser);

  // Named func call param
  if (match(parser, TOKEN_COLON, false)) {
    second = &(*current_token(parser));
    advance(parser);
  }

  // Disable some semantic checks for write function
  if (strcmp(item->key, "write") != 0) {
    check_call_param(parser, param, &first, second, params_stack);
  }
  else {
    check_call_param_write(parser, &first, second, params_stack);
  }

  return 1 + call_params_n(parser, item, param == NULL ? NULL : param->next, params_stack);
}

SymtableIdentifierType func_call(Parser* parser, char* func_id, void_stack_t* params_stack) {
  SymtableItem* item = symtable_get(parser->global_table, func_id);

  if (item == NULL || item->data->type != SYMTABLE_FUNCTION) {
    pexit_with_error(parser, SEMANTIC_ERR_FUNC, "Function %s is not defined", func_id);
  }

  consume(parser, TOKEN_LPAREN, "Expected '('");
  int params_count = call_params(parser, item, item->data->function.params, params_stack);

  if (strcmp(func_id, "write") == 0) {
    stack_reverse(params_stack);
    str* params_count_str = str_new_int_const("");
    str_append_int(params_count_str, params_count);
    stack_push(params_stack, params_count_str);
  }

  consume(parser, TOKEN_RPAREN, "Expected ')'");

  return item->data->function._return->identifier_type;
}

bool return_t(Parser* parser) {
  if (!parser->in_function) {
    pexit_with_error(parser, SYNTAX_ERR, "Return statement outside of function");
  }

  if (parser->current_function_name == NULL) {
    pexit_with_error(parser, INTERNAL_ERROR, "Current function name is NULL");
  }

  SymtableItem* func = search_var_in_tables(parser, parser->current_function_name);

  if (func == NULL) {
    pexit_with_error(parser, INTERNAL_ERROR, "Current function is NULL (shouldn't be possible) ((⚠️))");
  }

  // return_t -> eps rule
  if (check_type(parser, TOKEN_RBRACE)) {
    if (func->data->function._return->identifier_type.data_type != VOID_TYPE) {
      pexit_with_error(parser, SEMANTIC_ERR_RETURN, "Non void function must have an <expression> in return statement");
    }

    return true;
  }


  // return is followed by a function call
  if (current_token(parser)->type == TOKEN_IDENTIFIER && peek(parser)->type == TOKEN_LPAREN) {
    // function call is on a new line
    if (current_token(parser)->after_newline) {
      // function call is not an expression, so non void function has invalid return
      if (func->data->function._return->identifier_type.data_type != VOID_TYPE) {
        pexit_with_error(parser, SEMANTIC_ERR_RETURN, "Non void function must have an <expression> in return statement");
      } else {
        parser->after_return = true;
        return true;
      }
    }
    else {
      // Statements must be separated by new line
      if (func->data->function._return->identifier_type.data_type == VOID_TYPE) {
        pexit_with_error(parser, SEMANTIC_ERR_RETURN, "Function can't have a function call in return statement"); 
      }
    }
  }

#ifdef PARSER_TEST
  SymtableIdentifierType expression_type = expression(parser, DEFAULT_EXPRESION_TYPE);
#else
  void_stack_t* expr_stack = stack_new(100);
  SymtableIdentifierType expression_type = expression(parser, expr_stack, func->data->function._return->identifier_type);
  generator_function_return_expr(parser->gen, expr_stack);
#endif

  if (!compare_symtable_item_types(func->data->function._return->identifier_type, expression_type)) {
    if (func->data->function._return->identifier_type.data_type == VOID_TYPE) {
      pexit_with_error(parser, SEMANTIC_ERR_RETURN, "Return type doesn't match the function definition");
    } else {
      pexit_with_error(parser, SEMANTIC_ERR_CALL, "Return type doesn't match the function definition");
    }
  }

  parser->after_return = true;
  return true;
}

bool body(Parser* parser) {
  bool valid_return;

  // body -> eps rule
  if (check_type(parser, TOKEN_RBRACE) || check_type(parser, TOKEN_EOF) || check_keyword(parser, KW_FUNC)) {
    parser->after_return = false;
    return false;
  }
  // body -> <statement> <body> rule

  valid_return = statement(parser);
  return valid_return | body(parser);
}

// if_statement -> 'if' if_cond '{' body '} else {' body '}' .
bool if_statement(Parser* parser) {
  parser->in_scope = true;

  Symtable* local_table = symtable_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
  stack_push(parser->local_tables_stack, local_table);

  bool valid_return;

  // If var is a little somarek, we don't support him
  if (match_keyword(parser, KW_VAR, false)) {
    pexit_with_error(parser, SYNTAX_ERR, "If with 'var' is not supported 😝, try to use 'let' instead");
  }

  // if_cond -> <expr> | VAR_DEFINITION_KW IDENTIFIER '=' <expr>
  if (match_keyword(parser, KW_LET, false)) {
    char* tmp = current_token(parser)->val;
    char* var_id = malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(var_id, tmp);
    check_identifier(tmp);

    consume(parser, TOKEN_IDENTIFIER, "Expected identifier");

    SymtableItem* result = search_var_in_tables(parser, var_id);

    if (!result) {
      pexit_with_error(parser, SEMANTIC_ERR_FUNC, "Variable %s has to be defined", var_id);
    }

    if (!result->data->variable.constant) {
      pexit_with_error(parser, SEMANTIC_ERR, "Variable %s has to be defined as constant", var_id);
    }

    SymtableItem* item = symtable_add_symbol(local_table, var_id, SYMTABLE_VARIABLE, true, true, true);
    item->data->variable.identifier_type = result->data->variable.identifier_type;
    item->data->variable.identifier_type.nullable = false;

    generator_if_begin(parser->gen, str_new_from_cstr(var_id), false, str_new_nil_const());
    goto else_branch;
  }

#ifdef PARSER_TEST
  SymtableIdentifierType expression_type = expression(parser, DEFAULT_IF_EXPRESION_TYPE);
#else
  void_stack_t* expr_stack = stack_new(100);
  SymtableIdentifierType expression_type = expression(parser, expr_stack, (SymtableIdentifierType) { BOOL_TYPE, false });
  // fprintf(stderr, "Padacek bahnacek vratil typ %d\n",
  //         expression_type.data_type);
#endif

  if (expression_type.data_type != BOOL_TYPE) {
    pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Expression in if statement must be boolean");
  }

#ifdef PARSER_TEST
  if (expression_type.data_type == VOID_TYPE) {
    pexit_with_error(parser, SYNTAX_ERR, "Only for testing purposes, expression cannot be void");
  }
#endif

#ifndef PARSER_TEST
  generator_if_begin_stack(parser->gen, true, expr_stack);
#endif

else_branch:
  consume(parser, TOKEN_LBRACE, "Expected '{'");
  valid_return = body(parser);

  if (!match(parser, TOKEN_RBRACE, false)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected '}'");
  }

  if (!match_keyword(parser, KW_ELSE, false)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected 'else'");
  }

  generator_if_else(parser->gen);

  stack_pop(parser->local_tables_stack);
  local_table = symtable_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
  stack_push(parser->local_tables_stack, local_table);

  consume(parser, TOKEN_LBRACE, "Expected '{'");

  valid_return &= body(parser);

  if (!match(parser, TOKEN_RBRACE, false)) {
    pexit_with_error(parser, SYNTAX_ERR, "Expected '}'");
  }

  generator_if_end(parser->gen);

  stack_pop(parser->local_tables_stack);
  parser->in_scope = false;
  return valid_return;
  }

bool statement(Parser* parser) {
  bool valid_return = false;
  bool is_constant = false;

  if (check_keyword(parser, KW_LET)) {
    is_constant = true;
  }

  // statement -> if <expression> { <statement_list> } else { <statement_list> }
  // rule
  if (match_keyword(parser, KW_IF, true)) {
    valid_return |= if_statement(parser);
  }
  // statement -> while <expression> { <statement_list> } else {
  // <statement_list> } rule
  else if (match_keyword(parser, KW_WHILE, true)) {
    parser->in_scope = true;

    Symtable* local_table = symtable_new(LUFAK_JE_PEPIK_TODO_PREPSAT_NA_DYNAMICKEJ_STACK);
    stack_push(parser->local_tables_stack, local_table);

#ifdef PARSER_TEST
    SymtableIdentifierType expression_type = expression(parser, DEFAULT_IF_EXPRESION_TYPE);
#else
    void_stack_t* expr_stack = stack_new(100);
    SymtableIdentifierType expression_type = expression(parser, expr_stack, (SymtableIdentifierType) { BOOL_TYPE, false });
    // stack_reverse(expr_stack);
#endif

    if (expression_type.data_type != BOOL_TYPE) {
      pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Expression in while statement must be boolean");
    }

#ifdef PARSER_TEST
    if (expression_type.data_type == VOID_TYPE) {
      pexit_with_error(parser, SYNTAX_ERR, "Only for testing purposes, expression cannot be void");
    }
#endif

#ifndef PARSER_TEST
    generator_loop_start(parser->gen, expr_stack);
#endif
    consume(parser, TOKEN_LBRACE, "Expected '{'");
    body(parser);

    if (!match(parser, TOKEN_RBRACE, false)) {
      pexit_with_error(parser, SYNTAX_ERR, "Expected '}'");
    }

    stack_pop(parser->local_tables_stack);
    parser->in_scope = false;
    generator_loop_end(parser->gen);
    }
  // statement -> return <return_t>
  else if (match_keyword(parser, KW_RETURN, true)) {
    valid_return |= return_t(parser);
  }
  // statement -> <var_definition_kw> <identifier> <var_definition_value>
  else if (match_keyword(parser, KW_LET, true) || match_keyword(parser, KW_VAR, true)) {
    SymtableIdentifierType identifier_type = (SymtableIdentifierType){ .data_type = UNKNOWN_TYPE, .nullable = false };
    bool var_initialized = false;

    char* tmp = current_token(parser)->val;
    char *variable_id = malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(variable_id, tmp);
    check_identifier(variable_id);

    consume(parser, TOKEN_IDENTIFIER, "Expected identifier");

    // Variable type definition
    if (match(parser, TOKEN_COLON, false)) {
      // TODO: Check if type is valid
      if (!is_datatype(parser)) {
        pexit_with_error(parser, SYNTAX_ERR, "Expected datatype after ':'");
      }
      else {
        // Save data type token for later
        identifier_type = parse_datatype(parser);
      }
    }

    Symtable* table;

    if (!parser->in_function && !parser->in_scope) {
      table = parser->global_table;
    }
    else {
      table = parser->local_tables_stack->items[parser->local_tables_stack->top_index];
    }

    // Variable initialization
    if (match(parser, TOKEN_ASSIGN, false)) {
      var_initialized = true;
      if (check_type(parser, TOKEN_IDENTIFIER) && peek(parser)->type == TOKEN_LPAREN) {
        void_stack_t* params_stack = stack_new(100);

        char* tmp = current_token(parser)->val;
        char* func_id = malloc(sizeof(char) * (strlen(tmp) + 1));
        strcpy(func_id, tmp);
        check_identifier(func_id);

        consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
        SymtableIdentifierType returned_type = func_call(parser, func_id, params_stack);

        if (returned_type.data_type == VOID_TYPE) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign void to variable");
        }

        if (identifier_type.data_type == UNKNOWN_TYPE) {
          identifier_type = returned_type;
        }
        else if (!compare_symtable_item_types(identifier_type, returned_type)) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign %s to %s", "<returned_type>", "<identifier_type>");
        }

        stack_reverse(params_stack);

        if (!parser->after_return) {
          generator_function_call(parser->gen, str_new_from_cstr(func_id), params_stack, str_new_from_cstr(variable_id));
        }
      }
      else {
#ifdef PARSER_TEST
        SymtableIdentifierType expression_type = expression(parser, DEFAULT_EXPRESION_TYPE);
#else
        void_stack_t* expr_stack = stack_new(128);

        SymtableIdentifierType expression_type = expression(parser, expr_stack, identifier_type);
        
        stack_reverse(expr_stack);
        stack_push(expr_stack, str_new_from_cstr("?"));
        stack_reverse(expr_stack);
#endif

        if (identifier_type.data_type == UNKNOWN_TYPE) {
          if (expression_type.data_type == VOID_TYPE && !expression_type.nullable) {
            pexit_with_error(parser, SYNTAX_ERR, "Expected type of variable or expression");
          }
          else if (expression_type.data_type == VOID_TYPE && expression_type.nullable) {
            pexit_with_error(parser, SEMANTIC_ERR_INFER, "Cannot infer type of variable from nil expression");
          }
          identifier_type = expression_type;
        }
        else if (!compare_symtable_item_types(identifier_type, expression_type)) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign %s to %s", "<expression_type>", "<identifier_type>");
        }

#ifndef PARSER_TEST
        generator_expr(parser->gen, expr_stack);
#endif
      }
    }
    else if (identifier_type.nullable) {
      generator_var_set(parser->gen, str_new_from_cstr("?"), str_new_nil_const());
      var_initialized = true;
    }
    // Variable definition without initialization
    else {
      var_initialized = false;

      // Type was not defined
      if (identifier_type.data_type == UNKNOWN_TYPE) {
        pexit_with_error(parser, SYNTAX_ERR, "You have to define type of variable or initialize it with value");
      }
    }

    SymtableItem *result = search_var_in_tables(parser, variable_id);

    if (result == NULL || !result->data->variable.param) {
      generator_var_create(parser->gen, str_new_from_cstr(variable_id));
    }

    if (var_initialized) {
      generator_var_set(parser->gen, str_new_from_cstr(variable_id), str_new_from_cstr("?"));
    }

    SymtableItem* item = symtable_add_symbol(table, variable_id, SYMTABLE_VARIABLE, false, false, false);

    // Set variable data
    item->data->variable.constant = is_constant;
    item->data->variable.initialized = var_initialized;

    item->data->variable.identifier_type = identifier_type;

    if (var_initialized) {
      if (identifier_type.nullable) {
        item->data->variable.initialized = true;
      }
    }

  }
  else if (check_type(parser, TOKEN_IDENTIFIER)) {
    // statement -> <identifier> <var_definition_value>
    if (peek(parser)->type == TOKEN_ASSIGN) {
      SymtableItem* identifier_item = search_var_in_tables(parser, current_token(parser)->val);
      char* variable_id = current_token(parser)->val;

      if (identifier_item == NULL) {
        pexit_with_error(parser, SEMANTIC_ERR_VAR, "Variable %s is not defined", variable_id);
      }

      if (identifier_item->data->variable.param) {
        pexit_with_error(parser, SEMANTIC_ERR, "Cannot reassign function parameter '%s'", identifier_item->key);
      }

      // Modifing constant variable
      if (identifier_item->data->variable.constant && identifier_item->data->variable.initialized) {
        pexit_with_error(parser, SEMANTIC_ERR, "Cannot reassign constant variable '%s'", identifier_item->key);
      }

      if (check_type(parser, TOKEN_IDENTIFIER) && !current_token(parser)->after_newline) {
        pexit_with_error(parser, SYNTAX_ERR, "Statements must be separated by new line");
      }

      consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
      consume(parser, TOKEN_ASSIGN, "Expected '='");

      if (check_type(parser, TOKEN_IDENTIFIER) && peek(parser)->type == TOKEN_LPAREN) {
        void_stack_t* params_stack = stack_new(100);

        char* tmp = current_token(parser)->val;
        char *func_id = malloc(sizeof(char) * (strlen(tmp) + 1));
        strcpy(func_id, tmp);
        check_identifier(func_id);

        consume(parser, TOKEN_IDENTIFIER, "Expected identifier");

        SymtableIdentifierType returned_type = func_call(parser, func_id, params_stack);

        if (returned_type.data_type == VOID_TYPE) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign void to variable");
        }

        if (!compare_symtable_item_types(identifier_item->data->variable.identifier_type, returned_type)) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign %s to %s", "<returned_type>", "<identifier_type>");
        }

        stack_reverse(params_stack);

        if (!parser->after_return) {
          generator_function_call(parser->gen, str_new_from_cstr(func_id), params_stack, str_new_from_cstr(variable_id));
        }

        identifier_item->data->variable.initialized = true;
      }
      else {
#ifdef PARSER_TEST
        SymtableIdentifierType expression_type = expression(parser, DEFAULT_EXPRESION_TYPE);
#else

        void_stack_t* expr_stack = stack_new(100);
        SymtableIdentifierType expression_type = expression(parser, expr_stack, identifier_item->data->variable.identifier_type);

        stack_reverse(expr_stack);
        stack_push(expr_stack, str_new_from_cstr(variable_id));
        stack_reverse(expr_stack);
#endif

        if (!compare_symtable_item_types(identifier_item->data->variable.identifier_type, expression_type)) {
          pexit_with_error(parser, SEMANTIC_ERR_EXPR, "Cannot assign %s to %s", "<expression_type>", "<identifier_type>");
        }

        identifier_item->data->variable.initialized = true;

#ifndef PARSER_TEST
        generator_expr(parser->gen, expr_stack);

        // TODO: toto robil lufak ale dame to dopice protoze to dela expr parser
        // if (identifier_item->data->variable.identifier_type.data_type == DOUBLE_TYPE && expression_type.data_type == INT_TYPE && !expression_type.nullable) {
        //   stack_push(expr_stack, str_new_from_cstr(variable_id));
        //   generator_function_call(parser->gen, str_new_float_const(), expr_stack, str_new_from_cstr(variable_id));
        // }
#endif
      }
    }
    // statement -> func_call
    else if (peek(parser) != NULL && peek(parser)->type == TOKEN_LPAREN) {
      void_stack_t* params_stack = stack_new(128);
      char* func_id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier").val;
      func_call(parser, func_id, params_stack);
      stack_reverse(params_stack);

      //   printf("generator_function_call %s \n", func_id);
      if (!parser->after_return) {
        generator_function_call(parser->gen, str_new_from_cstr(func_id), params_stack, NULL);
      }
    }
    // statement -> expression that starts with identifier
    else {
#ifdef PARSER_TEST
      expression(parser, DEFAULT_EXPRESION_TYPE);
#else
      void_stack_t* expr_stack = stack_new(100);
      stack_push(expr_stack, str_new_from_cstr("?"));
      expression(parser, expr_stack, (SymtableIdentifierType) { UNKNOWN_TYPE, false });
      generator_expr(parser->gen, expr_stack);
#endif
    }
  }
  // statement -> <expression> rule
  else {
#ifdef PARSER_TEST
    expression(parser, DEFAULT_EXPRESION_TYPE);
#else
    void_stack_t* expr_stack = stack_new(100);
    stack_push(expr_stack, str_new_from_cstr("?"));
    expression(parser, expr_stack, (SymtableIdentifierType) { UNKNOWN_TYPE, false });
    generator_expr(parser->gen, expr_stack);
#endif
  }

  // If current paths leads to valid return or if rest of the body leads to
  // valid return, return true
  return valid_return | body(parser);
  }

void program(Parser* parser) {
  // program -> eps
  // End of file
  if (check_type(parser, TOKEN_EOF)) {
    return;
  }
  // prog -> 'func' FUNC_ID '(' <func_params> ')' <return_def> '{' <body> '}'
  // rule Func definition
  else if (match_keyword(parser, KW_FUNC, true)) {
    func_def(parser);
  }
  else {
    if (parser->semantic_enabled) {
      body(parser);
    }
    // First pass only checkes function definitions
    else {
      Token* token = current_token(parser);

      // CHrhh mnam mnam zchroustal jsem vsecky tokeny ktere nejsou func
      while (token != NULL && token->type != TOKEN_EOF && token->keyword != KW_FUNC) {
        token = advance(parser);
      }
    }
  }

  program(parser);
}

void parse(Parser* parser) {
  // prog rule
  program(parser);
}

#ifndef PARSER_TEST
void parser_start(Parser* parser)
#else
Token* parser_start(Parser* parser, Token* input_tokens)
#endif
{

#ifdef PARSER_TEST
  parser->input_tokens = input_tokens;
#endif
  scanner_consume(parser);

  DLL_Token* first_token = parser->tokens;

  generator_header(parser->gen);

  // First pass
  parse(parser);
  parser->semantic_enabled = true;

  // Second pass
  parser->tokens = first_token;
  parser->line = 1;
  parser->col = 1;

  // prog rule
  parse(parser);

#ifndef PARSER_TEST
  generator_footer(parser->gen);
  generator_print(parser->gen);
  generator_dispose(parser->gen);
#endif

#ifdef PARSER_TEST
  parser->output_tokens[parser->output_index] = (Token){ .type = TOKEN_EOF };
  return parser->output_tokens;
#endif
}

#ifdef PARSER_TEST
SymtableIdentifierType expression(Parser* parser, SymtableIdentifierType return_type)
#else
SymtableIdentifierType expression(Parser* parser, void_stack_t* expr_stack, SymtableIdentifierType return_type)
#endif
{
#ifdef PARSER_TEST
  if (current_token(parser)->type != TOKEN_COMMA) {
    return (SymtableIdentifierType) { .data_type = VOID_TYPE, .nullable = false };
  }
  consume(parser, TOKEN_COMMA, "Expected an expression");
  return return_type;
#else
  return parse_expression(parser, expr_stack, return_type);
#endif
  }

void scanner_consume(Parser* parser) {
  Token token;
  DLL_Token* prev = NULL;

  while (true) {
#ifndef PARSER_TEST
    get_next_token(&token);
#else
    Token token = parser->input_tokens[parser->input_index++];
    parser->output_tokens[parser->output_index++] = token;
#endif

    DLL_Token* dll_token = dll_token_new(token);

    if (prev == NULL) {
      parser->tokens = dll_token;
    }
    else {
      prev->next = dll_token;
      dll_token->prev = prev;
    }

    if (token.type == TOKEN_EOF) {
      break;
    }

    prev = dll_token;
  }
}

DLL_Token* dll_token_new(Token token) {
  DLL_Token* dll_token = malloc(sizeof(DLL_Token));
  if (dll_token == NULL) {
    exit_with_error(INTERNAL_ERROR, "Malloc failed");
  }

  dll_token->token = malloc(sizeof(Token));

  if (dll_token->token == NULL) {
    exit_with_error(INTERNAL_ERROR, "Malloc failed");
  }

  *dll_token->token = token;

  dll_token->next = NULL;
  dll_token->prev = NULL;

  if (token.val == NULL) {
    return dll_token;
  }

  dll_token->token->val = malloc(sizeof(char) * (strlen(token.val) + 1));

  if (dll_token->token->val == NULL) {
    free(dll_token->token);
    exit_with_error(INTERNAL_ERROR, "Malloc failed");
  }

  strcpy(dll_token->token->val, token.val);

  return dll_token;
}