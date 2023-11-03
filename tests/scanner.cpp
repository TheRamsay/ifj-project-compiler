extern "C" {
#include "../scanner.h"
#include "../scanner.c"
}
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(ScannerTest, Scan_Tokens_KW) {
  FILE *input_file = stdin;
  Token token;
  scanner_init(input_file);
  int result = get_next_token(&token);

  EXPECT_EQ(result, TOKEN_STRING);
  EXPECT_EQ(token.type, TOKEN_STRING);
  EXPECT_STREQ(token.val, "\"123\"");
  EXPECT_EQ(token.length, 5);

  result = get_next_token(&token);
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

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_PLUS);
  EXPECT_EQ(token.type, TOKEN_PLUS);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_MINUS);
  EXPECT_EQ(token.type, TOKEN_MINUS);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_MULTIPLY);
  EXPECT_EQ(token.type, TOKEN_MULTIPLY);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_DIV);
  EXPECT_EQ(token.type, TOKEN_DIV);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_MOD);
  EXPECT_EQ(token.type, TOKEN_MOD);

  scanner_destroy();
  fclose(input_file);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
