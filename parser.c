#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"

#define TOKEN_BUFFER_LEN 2

bool parser_init(Parser *parser)
{
    parser->global_table = NULL;
    parser->local_table = NULL;
    parser->buffer_active = false;
    parser->token_buffer = calloc(TOKEN_BUFFER_LEN, sizeof(Token));

    if (parser->token_buffer == NULL)
    {
        return false;
    }

    return true;
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

    get_next_token(parser->token_buffer + 1);
    parser->buffer_active = true;
    return parser->token_buffer + 1;
}

// Check if current token is of the expected type and advance if it is
bool match(Parser *parser, TokenType token_type)
{
    if (check_type(parser, token_type))
    {
        advance(parser);
        return true;
    }

    return false;
}

bool match_keyword(Parser *parser, KeywordType keyword)
{
    if (check_keyword(parser, keyword))
    {
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
        get_next_token(parser->token_buffer);
    }
}

// Returns true if current token is of the expected type
bool check_type(Parser *parser, TokenType token_type)
{
    return current_token(parser)->type == token_type;
}

// Check if current token is of the expected type and advance if it is, otherwise print error message and exit
Token *consume(Parser *parser, TokenType token_type, char *error_msg)
{
    if (!check_type(parser, token_type))
    {

        exit_custom(SYNTAX_ERR, "current_token %d (value: %s) | next token %d (value: %s) | error message: %s\n", current_token(parser)->keyword, current_token(parser)->val, peek(parser)->type, peek(parser)->val, error_msg);
    }

    Token *token = current_token(parser);
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

SymtableDataType keyword_to_datatype(KeywordType keyword)
{
    switch (keyword)
    {
    case KW_INT:
        return INT_TYPE;
    case KW_FLOAT:
        return DOUBLE_TYPE;
    case KW_DOUBLE:
        return DOUBLE_TYPE;
    case KW_STRING:
        return STRING_TYPE;
    default:
        return UNKNOWN_TYPE;
    }
}

bool is_term(Parser *parser)
{
    return check_type(parser, TOKEN_IDENTIFIER) ||
           check_type(parser, TOKEN_STRING_LITERAL) ||
           check_type(parser, TOKEN_INTEGER_LITERAL) ||
           check_type(parser, TOKEN_DECIMAL_LITERAL) ||
           check_keyword(parser, KW_NIL);
}

void return_def(Parser *parser)
{
    if (check_type(parser, TOKEN_LBRACE))
    {
        return;
    }
    
    printf("Before is token %s %d\n", current_token(parser)->val, current_token(parser)->type);

    if (!match(parser, TOKEN_ARROW))
    {
        printf("Current token: %d %s, next token %d %s\n", current_token(parser)->type, current_token(parser)->val, peek(parser)->type, peek(parser)->val);
        exit_custom(SYNTAX_ERR, "missing '->' in function definition\n");
    }

    // Check if return type is valid datatype
    if (!is_datatype(parser))
    {
        exit_custom(SYNTAX_ERR, "expected datatype after '->'\n");
    }

    // TODO: this is temporary
    advance(parser);
}

void func_params_n(Parser *parser, SymtableItem *item)
{
    if (!match(parser, TOKEN_COMMA))
    {
        return;
    }

    Token *out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier");
    // IN identifier for function param
    Token *in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier");
    // symtable_add_param(item, out_param_id->val, in_param_id->val, keyword_to_datatype(out_param_id->keyword));

    consume(parser, TOKEN_COLON, "Expected : ");

    if (!is_datatype(parser))
    {
        exit_custom(SYNTAX_ERR, "Expected datatype after ':'\n");
    }

    Token *param_type = current_token(parser);
    (void)param_type;
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
    Token *out_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected out identifier");
    // IN identifier for function param
    Token *in_param_id = consume(parser, TOKEN_IDENTIFIER, "Expected in identifier");
    // symtable_add_param(item, out_param_id->val, in_param_id->val, keyword_to_datatype(out_param_id->keyword));

    consume(parser, TOKEN_COLON, "Expected : ");

    // Just consume type, idk what to do with it yet

    if (!is_datatype(parser))
    {
        exit_custom(SYNTAX_ERR, "Expected datatype after ':'\n");
    }
    Token *param_type = current_token(parser);
    (void)param_type;

    advance(parser);

    func_params_n(parser, item);
}

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def(Parser *parser)
{
    Token *func_id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
    // SymtableItem *item = symtable_insert(parser->global_table, func_id->val, SYMTABLE_FUNCTION, true);

    consume(parser, TOKEN_LPAREN, "Expected '('");
    func_params(parser, NULL);
    consume(parser, TOKEN_RPAREN, "Expected ')'");
    return_def(parser);
    consume(parser, TOKEN_LBRACE, "Expected '{'");
    body(parser);
    consume(parser, TOKEN_RBRACE, "Expected '}'");
}

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n(Parser *parser)
{
    if (check_type(parser, TOKEN_RPAREN))
    {
        return;
    }

    consume(parser, TOKEN_COMMA, "Expected ','");
    if (match(parser, TOKEN_IDENTIFIER))
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

    if (match(parser, TOKEN_IDENTIFIER))
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
        exit_custom(SYNTAX_ERR, "Expected term in function call parameter\n");
    }

    call_params_n(parser);
}

void return_t(Parser *parser)
{
    // return_t -> eps rule
    if (check_type(parser, TOKEN_RBRACE))
    {
        return;
    }

    expression(parser);
    // if (!is_datatype())
}

void body(Parser *parser)
{
    // body -> eps rule
    if (check_type(parser, TOKEN_RBRACE) || check_type(parser, TOKEN_EOF))
    {
        return;
    }
    // body -> <statement> <body> rule
    else
    {
        statement(parser);
    }

    body(parser);
}

// if_statement -> 'if' if_cond '{' body '} else {' body '}' .
void if_statement(Parser *parser)
{
    // if_cond -> <expr> | VAR_DEFINITION_KW IDENTIFIER '=' <expr>

    if (match_keyword(parser, KW_LET) || match_keyword(parser, KW_VAR))
    {
        consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
        consume(parser, TOKEN_ASSIGN, "Expected '='");
    }

    expression(parser);
    consume(parser, TOKEN_LBRACE, "Expected '{'");
    body(parser);

    if (!match(parser, TOKEN_RBRACE))
    {
        exit_custom(SYNTAX_ERR, "Expected '}'\n");
    }
}

void statement(Parser *parser)
{
    // statement -> if <expression> { <statement_list> } else { <statement_list> } rule
    if (match_keyword(parser, KW_IF))
    {
        if_statement(parser);
    }
    // statement -> while <expression> { <statement_list> } else { <statement_list> } rule
    else if (match_keyword(parser, KW_WHILE))
    {
        expression(parser);
        consume(parser, TOKEN_LBRACE, "Expected '{'");
        // TODO: zmrd
        body(parser);

        if (!match(parser, TOKEN_RBRACE))
        {
            exit_custom(SYNTAX_ERR, "Expected '}'\n");
        }
    }
    // statement -> return <return_t>
    else if (match_keyword(parser, KW_RETURN))
    {
        // TODO: idk
        return_t(parser);
    }
    // statement -> <var_definition_kw> <identifier> <var_definition_value>
    else if (match_keyword(parser, KW_LET) || match_keyword(parser, KW_VAR))
    {
        consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
        // Variable type definition
        if (match(parser, TOKEN_COLON))
        {
            // TODO: Check if type is valid
            if (!is_datatype(parser))
            {
                exit_custom(SYNTAX_ERR, "Expected datatype after ':'\n");
            }
            advance(parser);
        }

        // Variable initialization
        if (match(parser, TOKEN_ASSIGN))
        {
            expression(parser);
        }
    }
    else if (check_type(parser, TOKEN_IDENTIFIER))
    {
        // statement -> <identifier> <var_definition_value>
        if (peek(parser)->type == TOKEN_ASSIGN)
        {
            consume(parser, TOKEN_IDENTIFIER, "Expected identifier");
            consume(parser, TOKEN_ASSIGN, "Expected '='");
            expression(parser);
        }
        // statemenet -> expression that starts with identifier
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

    body(parser);
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
    else if (match_keyword(parser, KW_FUNC))
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

void parse(Parser *parser)
{
    // Get first token
    advance(parser);
    // prog rule
    program(parser);
}

void expression(Parser *parser)
{
    consume(parser, TOKEN_COMMA, "Expected an expression");
    return;
}