#include "scanner.h"
#include "parser.h"

#define TOKEN_BUFFER_LEN 2

bool buffer_active = false;
Token token_buffer[TOKEN_BUFFER_LEN] = {0};

// Returns current token
Token *get_current_token()
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
    return get_current_token()->type == TOKEN_KEYWORD && get_current_token()->keyword == keyword;
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
    // printf("[PARSER] comparing token type %d with value %s (%d)\n", token_type, get_current_token()->val, get_current_token()->type);
    return get_current_token()->type == token_type;
}

// Check if current token is of the expected type and advance if it is, otherwise print error message and exit
void consume(TokenType token_type, char *error_msg)
{
    if (!check_type(token_type))
    {
        fprintf(stderr, "[PARSER ERROR] next token %d (value: %s) | error message: %s\n", peek()->type, peek()->val, error_msg);
        exit(69420);
    }

    advance();
}

bool is_datatype()
{
    Token *token = get_current_token();
    return token->type == TOKEN_KEYWORD &&
           (token->keyword == KW_INT ||
            token->keyword == KW_FLOAT || token->keyword == KW_DOUBLE ||
            token->keyword == KW_STRING);
}

void return_def()
{
    if (check_type(TOKEN_LBRACE))
    {
        return;
    }

    if (!match(TOKEN_ARROW))
    {
        fprintf(stderr, "[PARSER ERROR] missing '->' in function definition\n");
        exit(69420);
    }

    // Check if return type is valid datatype
    if (!is_datatype())
    {
        fprintf(stderr, "[PARSER ERROR] expected datatype after '->'\n");
        exit(69420);
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
    printf("[PARSER] Param type: %s\n", get_current_token()->val);
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
    consume(TOKEN_SEMICOLON, "Expected : ");

    // Just consume type, idk what to do with it yet
    advance();
    printf("[PARSER] Param type: %s\n", get_current_token()->val);
    func_params_n();
}

// function_def -> func FUNC_ID ( <func_params> ) <return_def> { <statement_list> }
void func_def()
{
    consume(TOKEN_IDENTIFIER, "Expected keyword 'func'");
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
    if (!check_type(TOKEN_COMMA))
    {
        return;
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

    expression();
    call_params_n();
}

void return_t()
{
    // if (!is_datatype())
}

void body()
{
    // body -> eps rule
    if (check_type(TOKEN_RBRACE))
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

void statement()
{
    // statement -> if <expression> { <statement_list> } else { <statement_list> } rule
    if (match_keyword(KW_IF))
    {
        expression();
        consume(TOKEN_LBRACKET, "Expected '{'");
        body();

        if (!match(TOKEN_RBRACE))
        {
            fprintf(stderr, "[PARSER ERROR] Expected '}'\n");
            exit(69420);
        }
    }
    // statement -> while <expression> { <statement_list> } else { <statement_list> } rule
    else if (match_keyword(KW_WHILE))
    {
        expression();
        consume(TOKEN_LBRACKET, "Expected '{'");
        // TODO: zmrd
        body();

        if (!match(TOKEN_RBRACE))
        {
            fprintf(stderr, "[PARSER ERROR] Expected '}'\n");
            exit(69420);
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
            advance();
        }

        if (!match(TOKEN_ASSIGN))
        {
            fprintf(stderr, "[PARSER ERROR] Expected '='\n");
            exit(69420);
        }

        expression();
    } // statement -> <func_id> ( <call_params> )
    else if (match(TOKEN_IDENTIFIER))
    {
        consume(TOKEN_LPAREN, "Expected '('");
        call_params();
        consume(TOKEN_RPAREN, "Expected ')'");
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
    advance();
    return;
}