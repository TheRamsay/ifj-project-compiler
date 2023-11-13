#include "scanner.h"
#include "parser.h"

#define TOKEN_BUFFER_LEN 2

bool buffer_active = false;
Token token_buffer[TOKEN_BUFFER_LEN] = {0};

Token *get_current_token()
{
    return token_buffer;
}

bool match_peek(TokenType token_type)
{
    if (peek()->type == token_type)
    {
        // advance();
        return true;
    }

    return false;
}

Token *peek()
{
    get_next_token(token_buffer + 1);
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
    return get_current_token()->type == token_type;
}

// Consumes next token, if it is not of the expected type, prints error message and exits
void consume(TokenType token_type, char *error_msg)
{
    advance();
    if (get_current_token()->type != token_type)
    {
        fprintf(stderr, "[PARSER ERROR] %s\n", error_msg);
        exit(69420);
    }
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
    advance();

    // Func has no return type
    // func <func_id> () {}
    if (match(TOKEN_LBRACKET))
    {
        return;
    }

    if (!match(TOKEN_ARROW))
    {
        fprintf(stderr, "[PARSER ERROR] missing '->' in function definition\n");
        exit(69420);
    }

    // Check if return type is valid datatype
    advance();
    if (!is_datatype())
    {
        fprintf(stderr, "[PARSER ERROR] expected datatype after '->'\n");
        exit(69420);
    }
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
    if (match_peek(TOKEN_RPAREN))
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
    consume(TOKEN_LBRACKET, "Expected '{'");
    statement();
    consume(TOKEN_RBRACKET, "Expected '}'");
}

// call_params_n -> , <call_params_kw> <term> <call_params_n>
void call_params_n()
{
    if (!match(TOKEN_COMMA))
    {
        return;
    }

    expression();
    call_params_n();
}

// call_params -> <call_params_kw> <term> <call_params_n>
void call_params()
{
    if (peek(TOKEN_RPAREN))
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

void statement()
{
    // statement -> if <expression> { <statement_list> } else { <statement_list> } rule
    if (check_keyword(KW_IF))
    {
        advance();
        expression();
        consume(TOKEN_LBRACKET, "Expected '{'");
        statement();
        consume(TOKEN_RBRACKET, "Expected '}'");
    }
    // statement -> while <expression> { <statement_list> } else { <statement_list> } rule
    else if (check_keyword(KW_WHILE))
    {
        advance();
        expression();
        consume(TOKEN_LBRACKET, "Expected '{'");
        // TODO: zmrd
        statement();
        consume(TOKEN_RBRACKET, "Expected '}'");
    }
    // statement -> return <return_t>
    else if (check_keyword(KW_RETURN))
    {
        // TODO: idk
        return_t();
    }
    // statement -> <var_definition_kw> <identifier> <var_definition_value>
    else if (check_keyword(KW_LET) || check_keyword(KW_VAR))
    {
        advance();
        consume(TOKEN_IDENTIFIER, "Expected identifier");
        // Variable type definition
        advance();
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

        advance();
        expression();
    } // statement -> <func_id> ( <call_params> )
    else if (match(TOKEN_IDENTIFIER))
    {
        consume(TOKEN_LPAREN, "Expected '('");
        call_params();
        consume(TOKEN_RPAREN, "Expected ')'");
    }

    statement();
}

void program()
{
    // End of file
    if (check_type(TOKEN_EOF))
    {
        return;
    }
    // prog -> 'func' FUNC_ID '(' func_params ')' return_def '{' statement_list '}' rule
    else if (check_keyword(KW_FUNC))
    {
        func_def();
    }
    // prog -> statement rule
    else
    {
        statement();
    }

    program();
}

void parse()
{
    // prog rule
    advance();
    program();
}

void expression()
{
}