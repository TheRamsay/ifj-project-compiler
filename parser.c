#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"

#define TOKEN_BUFFER_LEN 2

// Buffer for tokens, max 1 lookahead
bool buffer_active = false;
Token token_buffer[TOKEN_BUFFER_LEN] = {0};

// Returns current token
Token *current_token()
{
    return token_buffer;
}

Token *peek()
{
    if (buffer_active)
    {
        return token_buffer + 1;
    }

    get_next_token(token_buffer + 1);
    buffer_active = true;
    return token_buffer + 1;
}

// Check if current token is of the expected type and advance if it is
bool match(TokenType token_type)
{
    if (check_type(token_type))
    {
        advance();
        return true;
    }

    return false;
}

bool match_keyword(KeywordType keyword)
{
    if (check_keyword(keyword))
    {
        advance();
        return true;
    }

    return false;
}

// Check if value of keyword is equal to the expected keyword
bool check_keyword(KeywordType keyword)
{
    return current_token()->type == TOKEN_KEYWORD && current_token()->keyword == keyword;
}

// Advance to next token
void advance()
{
    if (buffer_active)
    {
        token_buffer[0] = token_buffer[1];
        buffer_active = false;
    }
    else
    {
        get_next_token(token_buffer);
    }
}

// Returns true if current token is of the expected type
bool check_type(TokenType token_type)
{
    return current_token()->type == token_type;
}

// Check if current token is of the expected type and advance if it is, otherwise print error message and exit
void consume(TokenType token_type, char *error_msg)
{
    if (!check_type(token_type))
    {

        exit_custom(SYNTAX_ERR, "current_token %d (value: %s) | next token %d (value: %s) | error message: %s\n", current_token()->keyword, current_token()->val, peek()->type, peek()->val, error_msg);
    }

    advance();
}

bool is_datatype()
{
    return check_keyword(KW_INT) ||
           check_keyword(KW_FLOAT) ||
           check_keyword(KW_DOUBLE) ||
           check_keyword(KW_STRING);
}

bool is_term()
{
    return check_type(TOKEN_IDENTIFIER) ||
           check_type(TOKEN_STRING_LITERAL) ||
           check_type(TOKEN_INTEGER_LITERAL) ||
           check_type(TOKEN_DECIMAL_LITERAL) ||
           check_keyword(KW_NIL);
}

void return_def()
{
    if (check_type(TOKEN_LBRACE))
    {
        return;
    }

    if (!match(TOKEN_ARROW))
    {
        exit_custom(SYNTAX_ERR, "missing '->' in function definition\n");
    }

    // Check if return type is valid datatype
    if (!is_datatype())
    {
        exit_custom(SYNTAX_ERR, "expected datatype after '->'\n");
    }

    // TODO: this is temporary
    advance();
}

void func_params_n()
{
    if (!match(TOKEN_COMMA))
    {
        return;
    }

    consume(TOKEN_IDENTIFIER, "Expected out identifier");
    consume(TOKEN_IDENTIFIER, "Expected in identifier");
    consume(TOKEN_COLON, "Expected : ");

    // Just consume type, idk what to do with it yet
    advance();
    // printf("[PARSER] Param type: %s\n", current_token()->val);
    func_params_n();
}

void func_params()
{
    // If closing paren, then no params
    if (check_type(TOKEN_RPAREN))
    {
        // func <func_id> ()
        return;
    }

    // OUT identifier for function param
    consume(TOKEN_IDENTIFIER, "Expected out identifier");
    // IN identifier for function param
    consume(TOKEN_IDENTIFIER, "Expected in identifier");
    consume(TOKEN_COLON, "Expected : ");

    // Just consume type, idk what to do with it yet

    if (!is_datatype())
    {
        exit_custom(SYNTAX_ERR, "Expected datatype after ':'\n");
    }

    advance();

    // printf("[PARSER] Param type: %s\n", current_token()->val);
    func_params_n();
}

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def()
{
    consume(TOKEN_IDENTIFIER, "Expected function identifier");
    consume(TOKEN_LPAREN, "Expected '('");
    func_params();
    consume(TOKEN_RPAREN, "Expected ')'");
    return_def();
    consume(TOKEN_LBRACE, "Expected '{'");
    body();
    consume(TOKEN_RBRACE, "Expected '}'");
}

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n()
{
    if (check_type(TOKEN_RPAREN))
    {
        return;
    }

    consume(TOKEN_COMMA, "Expected ','");
    if (match(TOKEN_IDENTIFIER))
    {
        consume(TOKEN_COLON, "Expected ':'");
    }

    expression();
    call_params_n();
}

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params()
{
    if (check_type(TOKEN_RPAREN))
    {
        return;
    }

    if (match(TOKEN_IDENTIFIER))
    {
        consume(TOKEN_COLON, "Expected ':'");
    }

    // Checks if call params are valid terms
    if (is_term())
    {
        advance();
    }
    else
    {
        exit_custom(SYNTAX_ERR, "Expected term in function call parameter\n");
    }

    call_params_n();
}

void return_t()
{
    // return_t -> eps rule
    if (check_type(TOKEN_RBRACE))
    {
        return;
    }

    expression();
    // if (!is_datatype())
}

void body()
{
    // body -> eps rule
    if (check_type(TOKEN_RBRACE) || check_type(TOKEN_EOF))
    {
        return;
    }
    // body -> <statement> <body> rule
    else
    {
        statement();
    }

    body();
}

// if_statement -> 'if' if_cond '{' body '} else {' body '}' .
void if_statement()
{
    // if_cond -> <expr> | VAR_DEFINITION_KW IDENTIFIER '=' <expr>

    if (match_keyword(KW_LET) || match_keyword(KW_VAR))
    {
        consume(TOKEN_IDENTIFIER, "Expected identifier");
        consume(TOKEN_ASSIGN, "Expected '='");
    }

    expression();
    consume(TOKEN_LBRACE, "Expected '{'");
    body();

    if (!match(TOKEN_RBRACE))
    {
        exit_custom(SYNTAX_ERR, "Expected '}'\n");
    }
}

void statement()
{
    // statement -> if <expression> { <statement_list> } else { <statement_list> } rule
    if (match_keyword(KW_IF))
    {
        if_statement();
    }
    // statement -> while <expression> { <statement_list> } else { <statement_list> } rule
    else if (match_keyword(KW_WHILE))
    {
        expression();
        consume(TOKEN_LBRACE, "Expected '{'");
        // TODO: zmrd
        body();

        if (!match(TOKEN_RBRACE))
        {
            exit_custom(SYNTAX_ERR, "Expected '}'\n");
        }
    }
    // statement -> return <return_t>
    else if (match_keyword(KW_RETURN))
    {
        // TODO: idk
        return_t();
    }
    // statement -> <var_definition_kw> <identifier> <var_definition_value>
    else if (match_keyword(KW_LET) || match_keyword(KW_VAR))
    {
        consume(TOKEN_IDENTIFIER, "Expected identifier");
        // Variable type definition
        if (match(TOKEN_COLON))
        {
            // TODO: Check if type is valid
            if (!is_datatype())
            {
                exit_custom(SYNTAX_ERR, "Expected datatype after ':'\n");
            }
            advance();
        }

        // Variable initialization
        if (match(TOKEN_ASSIGN))
        {
            expression();
        }
    }
    else if (check_type(TOKEN_IDENTIFIER))
    {
        // statement -> <identifier> <var_definition_value>
        if (peek()->type == TOKEN_ASSIGN)
        {
            consume(TOKEN_IDENTIFIER, "Expected identifier");
            consume(TOKEN_ASSIGN, "Expected '='");
            expression();
        }
        // statemenet -> expression that starts with identifier
        else
        {
            expression();
        }
    }
    // statement -> <expression> rule
    else
    {
        expression();
    }

    body();
}

void program()
{
    // program -> eps
    // End of file
    if (check_type(TOKEN_EOF))
    {
        return;
    }
    // prog -> 'func' FUNC_ID '(' <func_params> ')' <return_def> '{' <body> '}' rule
    // Func definition
    else if (match_keyword(KW_FUNC))
    {
        func_def();
    }
    // prog -> body rule
    // Parse statements
    else
    {
        body();
    }

    program();
}

void parse()
{
    // Get first token
    advance();
    // prog rule
    program();
}

void expression()
{
    consume(TOKEN_COMMA, "Expected an expression");
    return;
}