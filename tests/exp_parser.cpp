extern "C"{

#include "../stack.h"
#include "../exp_parser.h"
#include "../scanner.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>


class ExpParserTest : public testing::Test
{
protected:

    virtual void SetUp()
    {
    }
};

//Basic tests
TEST(ExpParserTest, BasicTests) { 
    //Should return true

    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL,   TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL, TOKEN_PLUS , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion2, 4));

    TokenType expresion3[] = {TOKEN_INTEGER_LITERAL,TOKEN_PLUS , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion3, 3));

     TokenType expresion4[] = { TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion4, 1));
}


//Minus tests
TEST(ExpParserTest, MinusTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_MINUS , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL, TOKEN_MINUS , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

//Multiply tests
TEST(ExpParserTest, MultiplyTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_MULTIPLY , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_MULTIPLY , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

//Divide tests
TEST(ExpParserTest, DivideTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_DIV , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_DIV , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

//Equals tests
TEST(ExpParserTest, EqualsTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_EQ , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_EQ , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

//Lower tests
TEST(ExpParserTest, LowerTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_LT , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_LT , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

// Lower or EQ tests
TEST(ExpParserTest, LowerOrEqualsTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_LE , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_LE , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}


//Higher tests
TEST(ExpParserTest, HigherTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_GT , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_GT , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}


// Greater or EQ tests
TEST(ExpParserTest, GreaterOrEqualsTests) {
    TokenType expresion1[] = {TOKEN_INTEGER_LITERAL, TOKEN_GE , TOKEN_INTEGER_LITERAL, TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 4));

    TokenType expresion2[] = {TOKEN_INTEGER_LITERAL,TOKEN_GE , TOKEN_STACK_BOTTOM};
    ASSERT_FALSE(parse_expression(expresion2, 3));
}

// Parenthesis  tests
TEST(ExpParserTest, ParenthesisTests) {
    TokenType expresion[] = { TOKEN_LPAREN , TOKEN_INTEGER_LITERAL, TOKEN_PLUS , TOKEN_INTEGER_LITERAL , TOKEN_RPAREN ,  TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion, 6));

    TokenType expresion1[] = { TOKEN_LPAREN , TOKEN_INTEGER_LITERAL, TOKEN_PLUS , TOKEN_INTEGER_LITERAL , TOKEN_RPAREN, TOKEN_RPAREN ,  TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion1, 7));

    TokenType expresion2[] = { TOKEN_LPAREN, TOKEN_LPAREN , TOKEN_INTEGER_LITERAL, TOKEN_PLUS , TOKEN_INTEGER_LITERAL , TOKEN_RPAREN ,  TOKEN_STACK_BOTTOM};
    ASSERT_TRUE(parse_expression(expresion2, 7));
}