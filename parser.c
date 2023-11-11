#include "scanner.h"

Token current_token;

void rule_program()
{
    switch (current_token.type)
    {
    case:
        break;
    case TOKEN_EOF:
        return;
    default:
        break;
    }

    rule_program();
}

void parse()
{
    get_next_token(&current_token);
    rule_program();
}
