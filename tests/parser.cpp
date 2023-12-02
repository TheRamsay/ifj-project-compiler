
extern "C"
{
#include "../parser.h"
}

#define MAX_INPUT_TOKENS 1000
#define INPUT_ARR_SIZE sizeof(tokens) / sizeof(*tokens)

#include <gtest/gtest.h>
#include <stdlib.h>

#include "vector"

class ParserTest : public testing::Test
{
protected:
	void SetUp() override { init_success_ = parser_init(&parser_); }

	Parser parser_;
	Token *input_tokens_;
	bool init_success_;
};

void check_tokens(std::vector<Token> input_tokens, Token *output_tokens)
{
	int i;
	for (i = 0; output_tokens[i].type != TOKEN_EOF; i++)
	{
		EXPECT_EQ(output_tokens[i].type, input_tokens[i].type);
		EXPECT_EQ(output_tokens[i].keyword, input_tokens[i].keyword);
		EXPECT_TRUE(strcmp(output_tokens[i].val, input_tokens[i].val) == 0);
		EXPECT_EQ(output_tokens[i].length, input_tokens[i].length);
	}

	EXPECT_EQ(i + 1, input_tokens.size());
}

KeywordType symtable_identifier_type_to_kw(SymtableIdentifierType type)
{
	switch (type.data_type)
	{
	case INT_TYPE:
		return KW_INT;
	case DOUBLE_TYPE:
		return KW_DOUBLE;
	case STRING_TYPE:
		return KW_STRING;
	case VOID_TYPE:
		return KW_VOID;
	default:
		return KW_UNKNOWN;
	}
}

std::string symtable_identifier_type_to_str(SymtableIdentifierType type)
{
	switch (type.data_type)
	{
	case INT_TYPE:
		return type.nullable ? "Int?" : "Int";
	case DOUBLE_TYPE:
		return type.nullable ? "Double?" : "Double";
	case STRING_TYPE:
		return type.nullable ? "String?" : "String";
	case VOID_TYPE:
		return "Void";
	default:
		return "";
	}
}

std::vector<Token> generate_function(SymtableData func,
									 std::vector<Token> body)
{
	if (func.type != SYMTABLE_FUNCTION)
	{
		return {};
	}

	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, func.identifier, strlen(func.identifier)},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
	};

	auto current_param = func.function.params;

	while (current_param != NULL)
	{
		auto str_type =
			symtable_identifier_type_to_str(current_param->identifier_type);
		tokens.push_back((Token){TOKEN_IDENTIFIER, KW_UNKNOWN,
								 current_param->out_name,
								 strlen(current_param->out_name)});
		tokens.push_back((Token){TOKEN_IDENTIFIER, KW_UNKNOWN,
								 current_param->in_name,
								 strlen(current_param->in_name)});
		tokens.push_back((Token){TOKEN_COLON, KW_UNKNOWN, ":", 1});
		tokens.push_back(
			(Token){TOKEN_KEYWORD,
					symtable_identifier_type_to_kw(current_param->identifier_type),
					str_type.data(), str_type.length(),
					current_param->identifier_type.nullable});

		if (current_param->next != NULL)
		{
			tokens.push_back((Token){TOKEN_COMMA, KW_UNKNOWN, ",", 1});
		}

		current_param = current_param->next;
	}

	tokens.push_back((Token){TOKEN_RPAREN, KW_UNKNOWN, ")", 1});

	if (func.function._return->identifier_type.data_type != VOID_TYPE)
	{
		auto str_type =
			symtable_identifier_type_to_str(func.function._return->identifier_type);
		tokens.push_back((Token){TOKEN_ARROW, KW_UNKNOWN, "->", 2});
		tokens.push_back((Token){
			TOKEN_KEYWORD,
			symtable_identifier_type_to_kw(func.function._return->identifier_type),
			str_type.data(), str_type.length(),
			func.function._return->identifier_type.nullable});
	}

	tokens.push_back((Token){TOKEN_LBRACE, KW_UNKNOWN, "{", 1});
	tokens.insert(tokens.end(), body.begin(), body.end());
	tokens.push_back((Token){TOKEN_RBRACE, KW_UNKNOWN, "}", 1});
}

TEST_F(ParserTest, Init)
{
	if (!init_success_)
	{
		return;
	}

	std::vector<Token> tokens = {{TOKEN_EOF, KW_UNKNOWN, "", false, 0, false}};

	Token *output = parser_start(&parser_, tokens.data());

	EXPECT_EQ(parser_.input_tokens, tokens.data());
	EXPECT_EQ(parser_.input_index, 1);
	EXPECT_EQ(parser_.buffer_active, false);
	EXPECT_NE(parser_.local_tables_stack, nullptr);
	EXPECT_NE(parser_.global_table, nullptr);
	EXPECT_NE(parser_.token_buffer, nullptr);

	for (int i = 0; i < tokens.size(); i++)
	{
		EXPECT_EQ(output[i].type, tokens[i].type);
		EXPECT_EQ(output[i].keyword, tokens[i].keyword);
		EXPECT_TRUE(strcmp(output[i].val, tokens[i].val) == 0);
		EXPECT_EQ(output[i].length, tokens[i].length);
	}
}

TEST_F(ParserTest, LetVariableDeclaration)
{
	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VarVariableDeclaration)
{
	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, LetVariableDeclarationWithType)
{
	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VarVariableDeclarationWithType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, LetVariableDeclarationWithTypeWithoutInit)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VarVariableDeclarationWithTypeWithoutInit)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, LetVariableDeclarationWithoutInit)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_INFER),
				"Semantic error.*");
}

TEST_F(ParserTest, VarVariableDeclarationWithoutInit)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_INFER),
				"Semantic error.*");
}

TEST_F(ParserTest, FuncDeclaration)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithReturnType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithReturnTypeNullable)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithParams)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "aihoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7, 1},
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

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithParamsWithReturnType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
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
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1, 0},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithBody)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1, 0, 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncDeclarationWithEmptyReturn)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "xx", 4},
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
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_RETURN),
				"Semantic error.*");
}

TEST_F(ParserTest, VoidFuncDeclarationWithNonEmptyReturn)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "xx", 4},
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
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_RETURN),
				"Semantic error.*");
}

TEST_F(ParserTest, FuncDeclarationWithReturn)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
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
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, TwoStatementsTwoLines)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4, 0},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 0, 1},
		{TOKEN_KEYWORD, KW_LET, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4, 0},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 0, 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0, 0, 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, TwoStatementsOnSameLine)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4, 0},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 0, 1},
		{TOKEN_KEYWORD, KW_LET, "var", 3, 0, 0},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4, 0},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 0, 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0, 0, 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, NoReturnInBranch)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_CALL), "Semantic error.*");
}

TEST_F(ParserTest, NoReturnInBranchNested)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_CALL), "Semantic error.*");
}

TEST_F(ParserTest, NoReturnInBranchButInFuncBody)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, NoReturnInBranchButInFuncBody2)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, NoReturnInBranchNestedVoidFunc)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VariableRedeclaration)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, MultipleVariables)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj2", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncRedeclaration)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, MultipleFuncDeclaration)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj2", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, FuncParamsInOutSameName)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, FuncParamsInvalidName)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 4},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, TopLevelReturn)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, SimpleIf)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, NestedIf)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, SimpleIfWithoutElseBranch)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, NestedIfWithoutElseBranch)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, IfLet)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},

		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_KEYWORD, KW_LET, "let", 3},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, IfLetWithoutExpr)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_KEYWORD, KW_LET, "let", 3},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, IfWithoutExpr)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, IfVar)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, SimpleWhile)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_WHILE, "while", 5, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, WhileWithBody)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_WHILE, "while", 5, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, WhileWithoutExpression)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_WHILE, "while", 5, 0, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, FuncDeclarMissingParameterType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepik", 5},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SYNTAX_ERR), "Syntax error.*");
}

TEST_F(ParserTest, FuncDeclarationMultipleParamsWithSameIdentifier)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepicek", 7},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, FuncDeclarationMultipleParamsWithSameName)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, ConstantModification)
{
	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_LET, "let", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4, 0, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR), "Semantic error.*");
}

TEST_F(ParserTest, MutableVariableModification)
{
	std::vector<Token> tokens = {
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4, 0, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, SameVariableNameDifferentScopes)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj2", 5},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, SameVariableNameDifferentScopesGlobal)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "aho1j", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "aho1j", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, RedeclarationInDifferentScope)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, RedeclarationInDifferentScope2)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, DeclareVariableWithSameNameAsFuncParam)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "pepa", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "x", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "String?", 7},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, IfLetWithoutDefinition)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_IF, "if", 2, 0, 1},
		{TOKEN_KEYWORD, KW_LET, "let", 3},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "ahoj", 4},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_ELSE, "else", 4},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, VariableAssignmentFuncCallWithoutType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VariableAssignmentFuncCallWithType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VariableAssignmentFuncCallWithWrongType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_STRING, "String?", 7},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3, 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, VariableAssignmentFuncCallVoidFunc)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},

		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, FuncCallWithoutAssignment)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 7, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1, 0, 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, UndefinedFuncCallWithoutAssignment)
{
	std::vector<Token> tokens{
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1, 0, 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()), ::testing::ExitedWithCode(SEMANTIC_ERR_FUNC), "Semantic error.*");
}

TEST_F(ParserTest, VariableAssignmentFuncCallAfterDefinition)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}

TEST_F(ParserTest, VariableAssignmentFuncCallAfterDefinitionWrongReturnType)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_STRING, "String?", 7, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	EXPECT_EXIT(parser_start(&parser_, tokens.data()),
				::testing::ExitedWithCode(SEMANTIC_ERR_EXPR), "Semantic error.*");
}

TEST_F(ParserTest, VariableAssignmentFuncCallAfterDefinitionWithParams)
{
	std::vector<Token> tokens{
		{TOKEN_KEYWORD, KW_VAR, "var", 3, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "a", 1},
		{TOKEN_ASSIGN, KW_UNKNOWN, "=", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 1},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_INTEGER_LITERAL, KW_UNKNOWN, "1", 1},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_KEYWORD, KW_FUNC, "func", 4, 0, 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "b", 4},
		{TOKEN_LPAREN, KW_UNKNOWN, "(", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "_", 1},
		{TOKEN_IDENTIFIER, KW_UNKNOWN, "y", 1},
		{TOKEN_COLON, KW_UNKNOWN, ":", 1},
		{TOKEN_KEYWORD, KW_INT, "Int", 3},
		{TOKEN_RPAREN, KW_UNKNOWN, ")", 1},
		{TOKEN_ARROW, KW_UNKNOWN, "->", 2},
		{TOKEN_KEYWORD, KW_INT, "Int?", 4, 1},
		{TOKEN_LBRACE, KW_UNKNOWN, "{", 1},
		{TOKEN_KEYWORD, KW_RETURN, "return", 6, 0, 1},
		{TOKEN_COMMA, KW_UNKNOWN, ",", 1},
		{TOKEN_RBRACE, KW_UNKNOWN, "}", 1},
		{TOKEN_EOF, KW_UNKNOWN, "", 0},
	};

	check_tokens(tokens, parser_start(&parser_, tokens.data()));
}