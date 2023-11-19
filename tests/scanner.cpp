extern "C" {
#include "../scanner.h"
#include "../scanner.c"
}
#include "gtest/gtest.h"
#include <stdlib.h>

class ScannerTest : public testing::Test
{
protected:
  FILE *input_file;
  Token token;
  void SetUp() override
  {
    input_file = fopen("test.swift", "r");
    if (input_file == NULL)
    {
      printf("Error opening file\n");
      exit(1);
    }
    scanner_init(input_file);
  }
};

TEST_F(ScannerTest, Scan_Tokens_KW)
{
  int result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_IF);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_ELSE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_WHILE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_INT);
}
#include "gtest/gtest.h"
#include <stdlib.h>

class ScannerTest : public testing::Test {
protected:
  FILE *input_file;
  Token token;
  void SetUp() override {
    input_file = fopen("../tests/test.swift", "r");
    if (input_file == NULL) {
      printf("Error opening file\n");
      exit(1);
    }
    scanner_init(input_file);
  }
};

TEST_F(ScannerTest, Scan_Tokens_KW) {
  int result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_IF);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_ELSE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_WHILE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_INT);
}

TEST_F(ScannerTest, Scan_Tokens_Arithmetic_Test) {
  // int result = get_next_token(&token);
  // EXPECT_EQ(result, TOKEN_PLUS);
  // EXPECT_EQ(token.type, TOKEN_PLUS);

  // result = get_next_token(&token);
  // EXPECT_EQ(result, TOKEN_MINUS);
  // EXPECT_EQ(token.type, TOKEN_MINUS);

  // result = get_next_token(&token);
  // EXPECT_EQ(result, TOKEN_MULTIPLY);
  // EXPECT_EQ(token.type, TOKEN_MULTIPLY);

  // result = get_next_token(&token);
  // EXPECT_EQ(result, TOKEN_DIV);
  // EXPECT_EQ(token.type, TOKEN_DIV);

  // result = get_next_token(&token);
  // EXPECT_EQ(result, TOKEN_MOD);
  // EXPECT_EQ(token.type, TOKEN_MOD);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
