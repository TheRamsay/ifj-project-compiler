extern "C"{

#include "../stack.h"
#include "../exp_parser.h"
#include "../scanner.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>


//Basic tests
TEST(ExpParserTest, BasicTests) { 
    //Should return true
    TokenType expresion[] = {TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL, TOKEN_PLUS , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion2));

    TokenType expresion3[] = {TOKEN_INTEGER_LITERAL,TOKEN_PLUS , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion3));

     TokenType expresion4[] = { TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion4));
}


//Minus tests
TEST(ExpParserTest, MinusTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_MINUS , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_MINUS , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2));
}

//Equals tests
TEST(ExpParserTest, EqualsTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_EQ , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_EQ , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2));
}

//Lower tests
TEST(ExpParserTest, LowerTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_LE , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_LE , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2));
}

//Lower tests
// TEST(ExpParserTest, LowerOrEqualsTests) {
//     TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_GE , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
//     ASSERT_TRUE(parse_expression(expresion1));

//     TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_GE , TOKEN_STACK_BOTTOM};
//     ASSERT_FALSE(parse_expression(expresion2));
// }


//Higher tests
TEST(ExpParserTest, HigherTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_GE , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_GE , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2));
}
