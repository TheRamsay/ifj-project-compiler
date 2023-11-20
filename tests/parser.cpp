
extern "C"
{
#include "../parser.h"
}

#define MAX_INPUT_TOKENS 1000

#include <gtest/gtest.h>
#include <stdlib.h>

class ParserTest : public testing::Test
{
protected:
    void SetUp() override
    {
        init_sucess_ = parser_init(&parser_);
    }

    // void TearDown() override {}

    Parser parser_;
    Token *input_tokens_;
    bool init_sucess_;
};

void check_tokens(Token *input_tokens, Token *output_tokens, int length)
{
    int i;
    for (i = 0; output_tokens[i].type != TOKEN_EOF; i++)
    {
        EXPECT_EQ(output_tokens[i].type, input_tokens[i].type);
        EXPECT_EQ(output_tokens[i].keyword, input_tokens[i].keyword);
        EXPECT_TRUE(strcmp(output_tokens[i].val, input_tokens[i].val) == 0);
        EXPECT_EQ(output_tokens[i].length, input_tokens[i].length);
    }

    EXPECT_EQ(i + 1, length);
}

TEST_F(ParserTest, Init)
{
    if (!init_sucess_)
    {
        return;
    }

    Token tokens[] = {{TOKEN_EOF, KW_UNKNOWN, "", 0}};
    Token *outpout = parse(&parser_, tokens);

    EXPECT_EQ(parser_.input_tokens, tokens);
    EXPECT_EQ(parser_.input_index, 1);
    EXPECT_EQ(parser_.buffer_active, false);
    EXPECT_NE(parser_.local_table, nullptr);
    EXPECT_NE(parser_.global_table, nullptr);
    EXPECT_NE(parser_.token_buffer, nullptr);

    for (int i = 0; i < 1; i++)
    {
        EXPECT_EQ(outpout[i].type, tokens[i].type);
        EXPECT_EQ(outpout[i].keyword, tokens[i].keyword);
        EXPECT_TRUE(strcmp(outpout[i].val, tokens[i].val) == 0);
        EXPECT_EQ(outpout[i].length, tokens[i].length);
    }
}

TEST_F(ParserTest, LetVariableDeclaration)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_LET, "let", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 5);
}

TEST_F(ParserTest, VarVariableDeclaration)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_VAR, "var", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 5);
}

TEST_F(ParserTest, LetVariableDeclarationWithType)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_LET, "let", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 7);
}

TEST_F(ParserTest, VarVariableDeclarationWithType)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_VAR, "var", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 7);
}

TEST_F(ParserTest, LetVariableDeclarationWithTypeWithoutInit)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_LET, "let", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 5);
}

TEST_F(ParserTest, VarVariableDeclarationWithTypeWithoutInit)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_VAR, "var", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 5);
}

TEST_F(ParserTest, LetVariableDeclarationWithoutInit)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_LET, "let", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 3);
}

TEST_F(ParserTest, VarVariableDeclarationWithoutInit)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_VAR, "var", 3},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 3);
}

TEST_F(ParserTest, FuncDeclaration)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 7);
}

TEST_F(ParserTest, FuncDeclarationWithReturnType)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "Double", 3},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 9);
}

TEST_F(ParserTest, FuncDeclarationWithReturnTypeNullable)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 9);
}

TEST_F(ParserTest, FuncDeclarationWithParams)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 16);
}

TEST_F(ParserTest, FuncDeclarationWithParamsWithReturnType)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 18);
}

TEST_F(ParserTest, FuncDeclarationWithBody)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 19);
}

TEST_F(ParserTest, FuncDeclarationWithEmptyReturn)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_KEYWORD, KW_RETURN, "return", 6},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 19);
}

TEST_F(ParserTest, FuncDeclarationWithReturn)
{
    Token tokens[] = {
        {TOKEN_KEYWORD, KW_FUNC, "func", 4},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
        {TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
        {TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
        {TOKEN_COLON, KW_UNKNOWN, ":", 1},
        {TOKEN_KEYWORD, KW_INT, "Int", 3},
        {TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
        {TOKEN_ARROW, KW_UNKNOWN, "->", 2},
        {TOKEN_KEYWORD, KW_INT, "String?", 7},
        {TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
        {TOKEN_KEYWORD, KW_RETURN, "return", 6},
        {TOKEN_COMMA, KW_UNKNOWN, ",", 1},
        {TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
        {TOKEN_EOF, KW_UNKNOWN, "", 0},
    };

    check_tokens(tokens, parse(&parser_, tokens), 20);
}