extern "C" {

#include "../exp_parser.h"
#include "../parser.h"
#include "../scanner.h"
#include "../stack.h"
#include "../symtable.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>

class ExpParserTest : public testing::Test {
protected:
  void SetUp() override { parser_init(&parser_); }

  Parser parser_;
};

// Basic tests
TEST_F(ExpParserTest, BasicTests) {
  // Should return true
  void_stack_t* stack1 = stack_new(50);
  Token expresion1[] = {
      {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };
  ASSERT_EQ(parse_expression(expresion1, 2, &parser_, stack1).type,
    TOKEN_EXPRESSION);

  char* result0 = ((str*)stack_top(stack1))->data;
  ASSERT_EQ(strcmp(result0, "int@42"), 0);

  void_stack_t* stack2 = stack_new(50);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion2, 4, &parser_, stack2).type,
    TOKEN_EXPRESSION);

  // char *result = ((str*)stack_pop(stack1))->data;
  // ASSERT_EQ(strcmp(result, "int@42"), 0);
  // char *result1 = ((str*)stack_pop(stack1))->data;
  // ASSERT_EQ(strcmp(result1, "int@42"), 0);
  // char *result2 = ((str*)stack_pop(stack1))->data;
  // ASSERT_EQ(strcmp(result2, "+"), 0);

  Token expresion3[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion3, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

  Token expresion4[] = { {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  // EXPECT_EXIT(parse_expression(expresion4, 1, &parser_),
  // ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
  ASSERT_EQ(parse_expression(expresion4, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  stack_dispose(stack1);
  stack_dispose(stack2);
}

// Minus tests
TEST_F(ExpParserTest, MinusTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_MINUS, KW_UNKNOWN, "-", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_MINUS, KW_UNKNOWN, "-", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Multiply tests
TEST_F(ExpParserTest, MultiplyTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_MULTIPLY, KW_UNKNOWN, "*", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_MULTIPLY, KW_UNKNOWN, "*", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Divide tests
TEST_F(ExpParserTest, DivideTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_DIV, KW_UNKNOWN, "/", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_DIV, KW_UNKNOWN, "/", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Equals tests
TEST_F(ExpParserTest, EqualsTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_EQ, KW_UNKNOWN, "==", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_EQ, KW_UNKNOWN, "==", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Lower tests
TEST_F(ExpParserTest, LowerTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_LT, KW_UNKNOWN, "<", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_LT, KW_UNKNOWN, "<", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Lower or EQ tests
TEST_F(ExpParserTest, LowerOrEqualsTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_LTE, KW_UNKNOWN, "<=", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_LTE, KW_UNKNOWN, "<=", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Higher tests
TEST_F(ExpParserTest, HigherTests) {
  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_GT, KW_UNKNOWN, ">", 0},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_GT, KW_UNKNOWN, ">", 0},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Greater or EQ tests
TEST_F(ExpParserTest, GreaterOrEqualsTests) {
  Token expresion1[] = {
      {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
      {TOKEN_GTE, KW_UNKNOWN, "<=", 1},
      {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };
  ASSERT_EQ(parse_expression(expresion1, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = {
      {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
      {TOKEN_GTE, KW_UNKNOWN, "<=", 1},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };

  EXPECT_EXIT(parse_expression(expresion2, 3, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Parenthesis  tests
TEST_F(ExpParserTest, ParenthesisTests) {
  Token expresion[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2},
                       {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                       {TOKEN_PLUS, KW_UNKNOWN, "+", 2},
                       {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                       {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                       {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion, 6, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion1[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                        {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion1, 7, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

  Token expresion2[] = { {TOKEN_LPAREN, KW_UNKNOWN, "(", 2},
                        {TOKEN_LPAREN, KW_UNKNOWN, "(", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 7, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");
}

// Otaznicky  tests
TEST_F(ExpParserTest, OtaznickyTests) {
  Token expresion[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                       {TOKEN_NULL_COALESCING, KW_UNKNOWN, "??", 2},
                       {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                       {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  ASSERT_EQ(parse_expression(expresion, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);
}

// Vykricniky  tests
TEST_F(ExpParserTest, VykricnikyTests) {
  Token expresion1[] = {
      {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
      {TOKEN_NOT, KW_UNKNOWN, "!", 1},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };

  ASSERT_EQ(parse_expression(expresion1, 3, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);
}

// Nil  tests
TEST_F(ExpParserTest, NilTests) {
  Token expresion1[] = {
      {TOKEN_KEYWORD, KW_NIL, "nil", 2},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };

  ASSERT_EQ(parse_expression(expresion1, 2, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion2[] = {
      {TOKEN_KEYWORD, KW_NIL, "nil", 2},
      {TOKEN_NOT, KW_UNKNOWN, "!", 1},
      {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0},
  };

  ASSERT_EQ(parse_expression(expresion2, 3, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);
  Token expresion3[] = { {TOKEN_KEYWORD, KW_NIL, "nil", 2},
                        {TOKEN_NULL_COALESCING, KW_UNKNOWN, "??", 2},
                        {TOKEN_STRING_LITERAL, KW_UNKNOWN, "ahoj", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  ASSERT_EQ(parse_expression(expresion3, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);

  Token expresion4[] = { {TOKEN_STRING_LITERAL, KW_UNKNOWN, "ahoj", 2},
                        {TOKEN_NULL_COALESCING, KW_UNKNOWN, "??", 2},
                        {TOKEN_KEYWORD, KW_NIL, "nil", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  ASSERT_EQ(parse_expression(expresion4, 4, &parser_, stack_new(50)).type,
    TOKEN_EXPRESSION);
}

// Basic tests
TEST_F(ExpParserTest, BasicTests2) {
  // Should return true

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 0},
                        {TOKEN_STRING_LITERAL, KW_UNKNOWN, "ahoj", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };
  EXPECT_EXIT(parse_expression(expresion2, 4, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SEMANTIC_ERR), ".*");

  void_stack_t* stack1 = stack_new(50);


  Token expresion3[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
          {TOKEN_PLUS, KW_UNKNOWN, "+", 0},
          {TOKEN_DECIMAL_LITERAL, KW_UNKNOWN, "0.4", 2},
          {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  Token result = parse_expression(expresion3, 4, &parser_, stack1);
  ASSERT_EQ(result.type, TOKEN_EXPRESSION);

  Token expresion4[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
        {TOKEN_PLUS, KW_UNKNOWN, "+", 0},
        {TOKEN_DECIMAL_LITERAL, KW_UNKNOWN, "0.4", 2},
        {TOKEN_MULTIPLY, KW_UNKNOWN, "*", 0},
        {TOKEN_DECIMAL_LITERAL, KW_UNKNOWN, "0.1", 2},
        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  Token result2 = parse_expression(expresion4, 6, &parser_, stack1);
  ASSERT_EQ(result.type, TOKEN_EXPRESSION);


  // parse_expression(expresion2, 4, &parser_);
}

// Jarda tests
TEST_F(ExpParserTest, JardaPepik) {
  // Should return true

  Token expresion1[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_LPAREN, KW_UNKNOWN, "(", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_PLUS, KW_UNKNOWN, "+", 2},
                        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
                        {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                        {TOKEN_RPAREN, KW_UNKNOWN, ")", 2},
                        {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  EXPECT_EXIT(parse_expression(expresion1, 8, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

  Token expresion2[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
          {TOKEN_GT, KW_UNKNOWN, ">", 2},
          {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
          {TOKEN_LT, KW_UNKNOWN, "<", 2},
          {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "1", 2},
          {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  EXPECT_EXIT(parse_expression(expresion2, 6, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");


  Token expresion3[] = { {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
          {TOKEN_PLUS, KW_UNKNOWN, "+", 2},
          {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
          {TOKEN_LPAREN, KW_UNKNOWN, ")", 2},
          {TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  EXPECT_EXIT(parse_expression(expresion3, 5, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

  Token expresion4[] = {
{TOKEN_RPAREN, KW_UNKNOWN, "(", 2},
        {TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
{TOKEN_PLUS, KW_UNKNOWN, "+", 2},
{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "42", 2},
{TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  EXPECT_EXIT(parse_expression(expresion4, 5, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");

  Token expresion5[] = {
{TOKEN_PLUS, KW_UNKNOWN, "+", 2},
{TOKEN_STACK_BOTTOM, KW_UNKNOWN, NULL, 0} };

  EXPECT_EXIT(parse_expression(expresion5, 2, &parser_, stack_new(50)),
    ::testing::ExitedWithCode(SYNTAX_ERR), ".*");


  // parse_expression(expresion1, 4, &parser_);
}
