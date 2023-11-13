#include "scanner.h"

Token current_token, prev_token;

bool match(TokenType token_type)
{
    
}

void function()
{
}

void statement()
{
}

void rule_program()
{
    if (current_token.type == TOKEN_EOF)
    {
        return;
    }
    else if (current_token.type == TOKEN_KEYWORD && current_token.keyword == KW_FUNC)
    {
        function();
    }
    else
    {
        statement();
    }

    rule_program();
}

void parse()
{
    get_next_token(&current_token);
    rule_program();
}
