//
// Created by padak on 10/26/23.
//

#ifndef _EXP_PARSER_H
#define _EXP_PARSER_H

#include "stack.h"

int ParseExpression(char *expressionToParse);

char *charArrayFromStack(Stack *stack);

#endif // _EXP_PARSER_H
