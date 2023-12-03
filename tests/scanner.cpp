extern "C" {
#include "../scanner.h"
}
#include <stdlib.h>

#include "../scanner.c"
#include "gtest/gtest.h"

class ScannerTest : public testing::Test {
protected:
  FILE *input_file;
  Token token;
  void SetUp() override {
    input_file = fopen("../tests/test.swift", "r");
    if (input_file == NULL) {
      fprintf(stderr, "Error opening file\n");
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
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_LBRACE);
  EXPECT_EQ(token.type, TOKEN_LBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_LPAREN);
  EXPECT_EQ(token.type, TOKEN_LPAREN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_STRING_LITERAL);
  EXPECT_EQ(token.type, TOKEN_STRING_LITERAL);
  EXPECT_STREQ(token.val, "Hello, World!");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_RPAREN);
  EXPECT_EQ(token.type, TOKEN_RPAREN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_RBRACE);
  EXPECT_EQ(token.type, TOKEN_RBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_ELSE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_LBRACE);
  EXPECT_EQ(token.type, TOKEN_LBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_WHILE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "i");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_LT);
  EXPECT_EQ(token.type, TOKEN_LT);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_INTEGER_LITERAL);
  EXPECT_EQ(token.type, TOKEN_INTEGER_LITERAL);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_LBRACE);
  EXPECT_EQ(token.type, TOKEN_LBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "i");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_ASSIGN);
  EXPECT_EQ(token.type, TOKEN_ASSIGN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "i");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_PLUS);
  EXPECT_EQ(token.type, TOKEN_PLUS);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_INTEGER_LITERAL);
  EXPECT_EQ(token.type, TOKEN_INTEGER_LITERAL);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_RBRACE);
  EXPECT_EQ(token.type, TOKEN_RBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_RBRACE);
  EXPECT_EQ(token.type, TOKEN_RBRACE);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_VAR);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "number");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_ASSIGN);
  EXPECT_EQ(token.type, TOKEN_ASSIGN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_INTEGER_LITERAL);
  EXPECT_EQ(token.type, TOKEN_INTEGER_LITERAL);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_LET);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "pi");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_ASSIGN);
  EXPECT_EQ(token.type, TOKEN_ASSIGN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_DECIMAL_LITERAL);
  EXPECT_EQ(token.type, TOKEN_DECIMAL_LITERAL);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_VAR);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);
  EXPECT_STREQ(token.val, "name");

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_ASSIGN);
  EXPECT_EQ(token.type, TOKEN_ASSIGN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_STRING_LITERAL);
  EXPECT_EQ(token.type, TOKEN_STRING_LITERAL);
  EXPECT_STREQ(token.val, "Debil");

  // // Test multi-line comment

  // // test single line comment

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_KEYWORD);
  EXPECT_EQ(token.type, TOKEN_KEYWORD);
  EXPECT_EQ(token.keyword, KW_LET);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_IDENTIFIER);
  EXPECT_EQ(token.type, TOKEN_IDENTIFIER);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_ASSIGN);
  EXPECT_EQ(token.type, TOKEN_ASSIGN);

  result = get_next_token(&token);
  EXPECT_EQ(result, TOKEN_STRING_LITERAL);
  EXPECT_EQ(token.type, TOKEN_STRING_LITERAL);
}

TEST_F(ScannerTest, Init_Scanner_Success) {
  // The scanner should initialize successfully
  EXPECT_NO_THROW(scanner_init(input_file));
}

TEST_F(ScannerTest, Destroy_Scanner) { EXPECT_NO_THROW(scanner_destroy()); }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
