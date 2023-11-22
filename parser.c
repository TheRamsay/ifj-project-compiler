#include "parser.h"

#define TOKEN_BUFFER_LEN 2

bool parser_init(Parser *parser)
{
    parser->global_table = symtable_new(100);
    parser->local_table = symtable_new(100);
    parser->buffer_active = false;
    parser->token_buffer = calloc(TOKEN_BUFFER_LEN, sizeof(Token));
    parser->in_function = false;

    if (parser->token_buffer == NULL)
    {
        return false;
    }

#ifndef PARSER_TEST
    parser->gen = generator_new();

    if (parser->gen == NULL)
    {
        return false;
    }

#else
    parser->input_tokens = NULL;
    parser->input_index = 0;
    parser->output_tokens = calloc(1000, sizeof(Token));

    if (parser->output_tokens == NULL)
    {
        return false;
    }

    parser->output_index = 0;
#endif

    return true;
}

// Checks if current statement stars on new line
void is_valid_statement(Parser *parser)
{
    if (current_token(parser)->after_newline)
    {
        return;
    }

    exit_with_error(SYNTAX_ERR, "Multiple statements must be on separate lines");
}

// Returns current token
Token *current_token(Parser *parser)
{
    return parser->token_buffer;
}

Token *peek(Parser *parser)
{
    if (parser->buffer_active)
    {
        return parser->token_buffer + 1;
    }

#ifndef PARSER_TEST
    get_next_token(parser->token_buffer + 1);
#else
    *(parser->token_buffer + 1) = parser->input_tokens[parser->input_index++];
    parser->output_tokens[parser->output_index++] = *(parser->token_buffer + 1);
#endif
    parser->buffer_active = true;
    return parser->token_buffer + 1;
}

// Check if current token is of the expected type and advance if it is
bool match(Parser *parser, TokenType token_type, bool check_new_line)
{
    if (check_type(parser, token_type))
    {
        if (check_new_line && !current_token(parser)->after_newline)
        {
            exit_with_error(SYNTAX_ERR, "Multiple statements must be on separate lines");
        }

        advance(parser);
        return true;
    }

    return false;
}

bool match_keyword(Parser *parser, KeywordType keyword, bool check_new_line)
{
    if (check_keyword(parser, keyword))
    {
        if (check_new_line && !current_token(parser)->after_newline)
        {
            exit_with_error(SYNTAX_ERR, "Multiple statements must be on separate lines");
        }

        advance(parser);
        return true;
    }

    return false;
}

// Check if value of keyword is equal to the expected keyword
bool check_keyword(Parser *parser, KeywordType keyword)
{
    return current_token(parser)->type == TOKEN_KEYWORD && current_token(parser)->keyword == keyword;
}

// Advance to next token
void advance(Parser *parser)
{
    if (parser->buffer_active)
    {
        parser->token_buffer[0] = parser->token_buffer[1];
        parser->buffer_active = false;
    }
    else
    {
#ifndef PARSER_TEST
        get_next_token(parser->token_buffer);
#else
        *(parser->token_buffer) = parser->input_tokens[parser->input_index++];
        parser->output_tokens[parser->output_index++] = *(parser->token_buffer);
#endif
    }
}

// Returns true if current token is of the expected type
bool check_type(Parser *parser, TokenType token_type)
{
    return current_token(parser)->type == token_type;
}

// Check if current token is of the expected type and advance if it is, otherwise print error message and exit
Token consume(Parser *parser, TokenType token_type, char *error_msg)
{
    if (!check_type(parser, token_type))
    {

        // #ifdef PARSER_TEST
        exit_with_error(SYNTAX_ERR, "current_token %d (value: %s) | next token %d (value: %s) | %s", current_token(parser)->keyword, current_token(parser)->val, peek(parser)->type, peek(parser)->val, error_msg);
        // #else
        // exit_with_error(SYNTAX_ERR, "%s", error_msg);
        // #endif
    }

    Token token = *current_token(parser);
    advance(parser);
    return token;
}

bool is_datatype(Parser *parser)
{
    return check_keyword(parser, KW_INT) ||
           check_keyword(parser, KW_FLOAT) ||
           check_keyword(parser, KW_DOUBLE) ||
           check_keyword(parser, KW_STRING);
}

SymtableIdentifierType keyword_to_datatype(Parser *parser, KeywordType keyword)
{
    SymtableDataType type;
    switch (keyword)
    {
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

    bool nullable = match(parser, TOKEN_OPTIONAL_TYPE, false);

    return (SymtableIdentifierType){.data_type = type, .nullable = nullable};
}

bool is_term(Parser *parser)
{
    return check_type(parser, TOKEN_IDENTIFIER) ||
           check_type(parser, TOKEN_STRING_LITERAL) ||
           check_type(parser, TOKEN_INTEGER_LITERAL) ||
           check_type(parser, TOKEN_DECIMAL_LITERAL) ||
           check_keyword(parser, KW_NIL);
}

void return_def(Parser *parser, SymtableItem *item)
{
    if (check_type(parser, TOKEN_LBRACE))
    {
        symtable_add_return(item, (SymtableIdentifierType){.data_type = VOID_TYPE, .nullable = false});
        return;
    }

    if (!match(parser, TOKEN_ARROW, false))
    {
        exit_with_error(SYNTAX_ERR, "missing '->' in function definition");
    }

    // Check if return type is valid datatype
    if (!is_datatype(parser))
    {
        exit_with_error(SYNTAX_ERR, "expected datatype after '->'");
    }

    KeywordType return_type = current_token(parser)->keyword;
    symtable_add_return(item, keyword_to_datatype(parser, return_type));

    // TODO: this is temporary
    advance(parser);
}

void func_params_n(Parser *parser, SymtableItem *item)
{
    if (!match(parser, TOKEN_COMMA, false))
    {
        return;
    }

    char *out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier").val;
    // IN identifier for function param
    char *in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier").val;

    consume(parser, TOKEN_COLON, "Expected : ");

    if (!is_datatype(parser))
    {
        exit_with_error(SYNTAX_ERR, "Expected datatype after ':'");
    }

    KeywordType param_type = current_token(parser)->keyword;
    symtable_add_param(item, out_param_id, in_param_id, keyword_to_datatype(parser, param_type));
    // Just consume type, idk what to do with it yet
    advance(parser);
    func_params_n(parser, item);
}

void func_params(Parser *parser, SymtableItem *item)
{
    // If closing paren, then no params
    if (check_type(parser, TOKEN_RPAREN))
    {
        // func <func_id> ()
        return;
    }

    // OUT identifier for function param
    char *out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier").val;
    // IN identifier for function param
    char *in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier").val;

    consume(parser, TOKEN_COLON, "Expected : ");

    // Just consume type, idk what to do with it yet
    if (!is_datatype(parser))
    {
        exit_with_error(SYNTAX_ERR, "Expected datatype after ':'");
    }
    KeywordType param_type = current_token(parser)->keyword;
    symtable_add_param(item, out_param_id, in_param_id, keyword_to_datatype(parser, param_type));

    advance(parser);

    func_params_n(parser, item);
}

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def(Parser *parser)
{
    parser->in_function = true;
    char *key = consume(parser, TOKEN_IDENTIFIER, "Expected identifier").val;
    SymtableItem *item = symtable_add_symbol(parser->global_table, key, SYMTABLE_FUNCTION, true, false);

    consume(parser, TOKEN_LPAREN, "Expected '('");
    func_params(parser, item);
    consume(parser, TOKEN_RPAREN, "Expected ')'");
    return_def(parser, item);
    consume(parser, TOKEN_LBRACE, "Expected '{'");

    if (!body(parser) && item->data->function._return->data_type != VOID_TYPE)
    {
        exit_with_error(SEMANTIC_ERR_CALL, "Function control flow does lead to invalid return");
    }

    consume(parser, TOKEN_RBRACE, "Expected '}'");

    symtable_clear(parser->local_table);
    parser->in_function = false;
}

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n(Parser *parser)
{
    if (check_type(parser, TOKEN_RPAREN))
    {
        return;
    }

    consume(parser, TOKEN_COMMA, "Expected ','");
    if (match(parser, TOKEN_IDENTIFIER, false))
    {
        consume(parser, TOKEN_COLON, "Expected ':'");
    }

    expression(parser);
    call_params_n(parser);
}

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params(Parser *parser)
{
    if (check_type(parser, TOKEN_RPAREN))
    {
        return;
    }

    if (match(parser, TOKEN_IDENTIFIER, false))
    {
        consume(parser, TOKEN_COLON, "Expected ':'");
    }

    // Checks if call params are valid terms
    if (is_term(parser))
    {
        advance(parser);
    }
    else
    {
        exit_with_error(SYNTAX_ERR, "Expected term in function call parameter");
    }

    call_params_n(parser);
}

bool return_t(Parser *parser)
{
    if (!parser->in_function)
    {
        exit_with_error(SYNTAX_ERR, "Return statement outside of function");
    }

    // return_t -> eps rule
    if (check_type(parser, TOKEN_RBRACE))
    {
        return true;
    }

    expression(parser);
    return true;
    // if (!is_datatype())
}

bool body(Parser *parser)
{
    bool valid_return;

    // body -> eps rule
    if (check_type(parser, TOKEN_RBRACE) || check_type(parser, TOKEN_EOF) || check_keyword(parser, KW_FUNC))
    {
        return false;
    }
    // body -> <statement> <body> rule

    valid_return = statement(parser);
    return valid_return | body(parser);
}

// if_statement -> 'if' if_cond '{' body '} else {' body '}' .
bool if_statement(Parser *parser)
{
    bool valid_return;

    // if_cond -> <expr> | VAR_DEFINITION_KW IDENTIFIER '=' <expr>
    if (match_keyword(parser, KW_LET, false) || match_keyword(parser, KW_VAR, false))
    {
        consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
        consume(parser, TOKEN_ASSIGN, "Expected '='");
    }

    expression(parser);
    consume(parser, TOKEN_LBRACE, "Expected '{'");
    valid_return = body(parser);

    if (!match(parser, TOKEN_RBRACE, false))
    {
        exit_with_error(SYNTAX_ERR, "Expected '}'");
    }

    if (!match_keyword(parser, KW_ELSE, false))
    {
        exit_with_error(SYNTAX_ERR, "Expected 'else'");
    }

    consume(parser, TOKEN_LBRACE, "Expected '{'");

    valid_return &= body(parser);

    if (!match(parser, TOKEN_RBRACE, false))
    {
        exit_with_error(SYNTAX_ERR, "Expected '}'");
    }

    return valid_return;
}

bool statement(Parser *parser)
{
    bool valid_return = false;
    bool is_constant = false;

    if (check_keyword(parser, KW_LET))
    {
        is_constant = true;
    }

    // statement -> if <expression> { <statement_list> } else { <statement_list> } rule
    if (match_keyword(parser, KW_IF, true))
    {
        valid_return |= if_statement(parser);
    }
    // statement -> while <expression> { <statement_list> } else { <statement_list> } rule
    else if (match_keyword(parser, KW_WHILE, true))
    {
        expression(parser);
        consume(parser, TOKEN_LBRACE, "Expected '{'");
        // TODO: zmrd
        body(parser);

        if (!match(parser, TOKEN_RBRACE, false))
        {
            exit_with_error(SYNTAX_ERR, "Expected '}'");
        }
    }
    // statement -> return <return_t>
    else if (match_keyword(parser, KW_RETURN, true))
    {
        valid_return |= return_t(parser);
    }
    // statement -> <var_definition_kw> <identifier> <var_definition_value>
    else if (match_keyword(parser, KW_LET, true) || match_keyword(parser, KW_VAR, true))
    {
        Token data_type = (Token){.type = TOKEN_UNKNOWN, .keyword = KW_UNKNOWN};
        bool var_initialized = false;

        char *variable_id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier").val;

        // Variable type definition
        if (match(parser, TOKEN_COLON, false))
        {
            // TODO: Check if type is valid
            if (!is_datatype(parser))
            {
                exit_with_error(SYNTAX_ERR, "Expected datatype after ':'");
            }
            else
            {
                // Save data type token for later
                data_type = *current_token(parser);
            }

            advance(parser);
        }

        // Variable initialization
        if (match(parser, TOKEN_ASSIGN, false))
        {
            var_initialized = true;
            expression(parser);
        }
        else
        {
            var_initialized = false;

            // Type was not defined
            if (data_type.type == TOKEN_UNKNOWN)
            {
                exit_with_error(SEMANTIC_ERR_INFER, "Cannot infer type of variable '%s'", variable_id);
            }
        }

        SymtableItem *item = symtable_add_symbol(parser->in_function ? parser->local_table : parser->global_table, variable_id, SYMTABLE_VARIABLE, var_initialized, is_constant);
        if (var_initialized)
        {
            item->data->variable.identifier_type = keyword_to_datatype(parser, data_type.keyword);
        }
    }
    else if (check_type(parser, TOKEN_IDENTIFIER))
    {
        // if (current_token(parser)->after_newline)
        // {
        //     exit_with_error(SYNTAX_ERR, "Multiple statements must be on separate lines");
        // }

        // statement -> <identifier> <var_definition_value>
        if (peek(parser)->type == TOKEN_ASSIGN)
        {
            SymtableItem *item = symtable_get(parser->in_function ? parser->local_table : parser->global_table, current_token(parser)->val);

            // Modifing constant variable
            if (item->data->variable.constant && item->data->variable.initialized)
            {
                exit_with_error(SEMANTIC_ERR, "Cannot reassign constant variable '%s'", item->key);
            }

            consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
            consume(parser, TOKEN_ASSIGN, "Expected '='");
            expression(parser);
        }
        // statement -> expression that starts with identifier
        else
        {
            expression(parser);
        }
    }
    // statement -> <expression> rule
    else
    {
        expression(parser);
    }

    // If current paths leads to valid return or if rest of the body leads to valid return, return true
    return valid_return | body(parser);
}

void program(Parser *parser)
{
    // program -> eps
    // End of file
    if (check_type(parser, TOKEN_EOF))
    {
        return;
    }
    // prog -> 'func' FUNC_ID '(' <func_params> ')' <return_def> '{' <body> '}' rule
    // Func definition
    else if (match_keyword(parser, KW_FUNC, true))
    {
        func_def(parser);
    }
    // prog -> body rule
    // Parse statements
    else
    {
        body(parser);
    }

    program(parser);
}

#ifndef PARSER_TEST
void parse(Parser *parser)
#else
Token *parse(Parser *parser, Token *input_tokens)
#endif
{

#ifdef PARSER_TEST
    parser->input_tokens = input_tokens;
#endif

    // Get first token
    advance(parser);
    // prog rule
    program(parser);

#ifdef PARSER_TEST
    parser->output_tokens[parser->output_index] = (Token){.type = TOKEN_EOF};
    return parser->output_tokens;
#endif
}

void expression(Parser *parser)
{
    consume(parser, TOKEN_COMMA, "Expected an expression");
    return;
}