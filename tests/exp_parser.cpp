extern "C"{

#include "../stack.h"
#include "../exp_parser.h"
#include "../scanner.h"
#include "../parser.h"
#include "../symtable.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>


class ExpParserTest : public testing::Test
{
protected:

    void SetUp() override
    {
        parser_init(&parser_);
    }

    Parser parser_;
};

// // Basic tests
// TEST_F(ExpParserTest, BasicTests) { 
//     Should return true

//     Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},    {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},};
//     ASSERT_TRUE(parse_expression(expresion1, 2, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_PLUS, KW_UNKNOWN, "+", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion2, 4, &parser_));

//     Token expresion3[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_PLUS, KW_UNKNOWN, "+", 0} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion3, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

//      Token expresion4[] = { {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion4, 1, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }


// //Minus tests
// TEST_F(ExpParserTest, MinusTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_MINUS, KW_UNKNOWN, "-", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_MINUS, KW_UNKNOWN, "-", 2} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// //Multiply tests
// TEST_F(ExpParserTest, MultiplyTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_MULTIPLY, KW_UNKNOWN, "*", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_MULTIPLY, KW_UNKNOWN, "*", 0} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// //Divide tests
// TEST_F(ExpParserTest, DivideTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},  {TOKEN_DIV, KW_UNKNOWN, "/", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},  {TOKEN_DIV, KW_UNKNOWN, "/", 0}  , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// //Equals tests
// TEST_F(ExpParserTest, EqualsTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},  {TOKEN_EQ, KW_UNKNOWN, "==", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_EQ, KW_UNKNOWN, "==", 0}  , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// //Lower tests
// TEST_F(ExpParserTest, LowerTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_LT, KW_UNKNOWN, "<", 0}  , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_LT, KW_UNKNOWN, "<", 0} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// // Lower or EQ tests
// TEST_F(ExpParserTest, LowerOrEqualsTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_LE, KW_UNKNOWN, "<=", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_LE, KW_UNKNOWN, "<=", 0} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }


// //Higher tests
// TEST_F(ExpParserTest, HigherTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_GT, KW_UNKNOWN, ">", 0} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//     Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_GT, KW_UNKNOWN, ">", 0} , {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }


// // Greater or EQ tests
// TEST_F(ExpParserTest, GreaterOrEqualsTests) {
//     Token expresion1[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_GE, KW_UNKNOWN, "<=", 1}, {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},};
//     ASSERT_TRUE(parse_expression(expresion1, 4, &parser_));

//         Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_GE, KW_UNKNOWN, "<=", 1},  {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},};

//     EXPECT_EXIT(parse_expression(expresion2, 3, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*"); 
// }

// Parenthesis  tests
// TEST_F(ExpParserTest, ParenthesisTests) {
//     Token expresion[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_PLUS, KW_UNKNOWN, "+", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2} , {TOKEN_RPAREN, KW_UNKNOWN, ")", 2} ,  {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion, 6, &parser_) );

    // Token expresion1[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_PLUS, KW_UNKNOWN, "+", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2} , {TOKEN_RPAREN, KW_UNKNOWN, ")", 2}, {TOKEN_RPAREN, KW_UNKNOWN, ")", 2} ,  {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
    // EXPECT_EXIT(parse_expression(expresion1, 7, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

    // Token expresion2[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2}, {TOKEN_LPAREN, KW_UNKNOWN, "(", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_PLUS, KW_UNKNOWN, "+", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2} , {TOKEN_RPAREN, KW_UNKNOWN, ")", 2} ,  {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
    // EXPECT_EXIT(parse_expression(expresion2, 7, &parser_), ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
// }

// // Otaznicky  tests
// TEST_F(ExpParserTest, OtaznickyTests) {
//     Token expresion[] = {  {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_NULL_COALESCING, KW_UNKNOWN, "??", 2} , {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2} ,  {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
//     ASSERT_TRUE(parse_expression(expresion, 4, &parser_));
// }

// // Vykricniky  tests
// TEST_F(ExpParserTest, VykricnikyTests) {
//      Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},{TOKEN_NOT, KW_UNKNOWN, "!", 1}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},};


//     ASSERT_TRUE(parse_expression(expresion1, 3, &parser_));
// }

//Basic tests
TEST_F(ExpParserTest, BasicTests2) { 
    //Should return true

    Token expresion2[] = {{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_PLUS, KW_UNKNOWN, "+", 0} , {TOKEN_STRING_LITERAL, KW_UNKNOWN, "42", 2}, {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0}};
    // EXPECT_EXIT(parse_expression(expresion2, 4, &parser_), ::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), ".*");
    parse_expression(expresion2, 4, &parser_);
}
