extern "C" {
#include "../generator.h"
}

#include <stdlib.h>
#include <strings.h>

#include <filesystem>

#include "./test_utils.cpp"
#include "gtest/gtest.h"

void createTmpFile(str *content) {
  FILE *tmpFile = fopen("./tmp", "w");
  fwrite(content->data, 1, strlen(content->data), tmpFile);
  fclose(tmpFile);
}

void deleteTmpFile() { std::filesystem::remove("./tmp"); }

TEST(GeneratorTest, HeaderFooter) {
  gen_t *gen = generator_new();

  generator_header(gen);
  generator_footer(gen);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  EXPECT_EQ(returnCode, 0);
  EXPECT_STREQ(output, "");

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInWrite) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str *expected_output = str_new(50);

  str_set_cstr(str2, "int@3");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "var_string");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "string@Hello\\032World!");
  generator_var_set(gen, str1, str2);
  stack_push(args, str_new_from_str(str1));

  str_append_cstr(expected_output, "Hello World!");

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@420");
  generator_var_set(gen, str1, str2);
  stack_push(args, str_new_from_str(str1));

  str_append_cstr(expected_output, "420");

  str_set_cstr(str1, "var_float");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "float@0x1.a4b0a3d70a3d7p+8");
  generator_var_set(gen, str1, str2);
  stack_push(args, str_new_from_str(str1));

  str_append_cstr(expected_output, "0x1.a4b0a3d70a3d7p+8");

  str_set_cstr(str1, "write");
  generator_function_call(gen, str_new_from_cstr("write"), args, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  EXPECT_EQ(strcmp(output, expected_output->data), 0);

  str_dispose(expected_output);

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInConversions) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@123");
  generator_var_set(gen, str1, str2);
  stack_push(args, str_new_from_str(str1));

  generator_function_call(gen, str_new_from_cstr("Int2Double"), args, str1);

  str_set_cstr(str1, "var_double");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "float@0x1.1p+7");
  generator_var_set(gen, str1, str2);

  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str2, "+");
  stack_push(args, str_new_from_str(str2));
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "var_int");
  stack_push(args, str_new_from_str(str1));

  generator_expr(gen, args);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  // no output expected
  EXPECT_EQ(strcmp(output, ""), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInLength) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_string");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "string@Hello\\032World!");
  generator_var_set(gen, str1, str2);

  str_set_cstr(str2, "var_out");
  generator_var_create(gen, str2);

  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "length");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str1, "int@1");
  stack_push(args, str_new_from_str(str1));
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  // |Hello World!| = 12
  EXPECT_EQ(strcmp(output, "12"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInOrd) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_string");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "string@Hello\\032World!");
  generator_var_set(gen, str1, str2);

  str_set_cstr(str2, "var_out");
  generator_var_create(gen, str2);

  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "ord");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_out");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  // 'H' = 72
  EXPECT_EQ(strcmp(output, "72"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInChr) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@72");
  generator_var_set(gen, str1, str2);

  str_set_cstr(str2, "var_out");
  generator_var_create(gen, str2);

  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "chr");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_out");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  // 72 = 'H'
  EXPECT_EQ(strcmp(output, "H"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, BuiltInSubstring) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_string");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "string@abcdefgh");
  generator_var_set(gen, str1, str2);

  str_set_cstr(str2, "var_substring");
  generator_var_create(gen, str2);

  // valid
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@2");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@5");
  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "substring");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_substring");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  // startsAt < 0
  str_set_cstr(str1, "var_string");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@-5");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@5");
  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "substring");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_substring");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);
  //

  // endsBefore < 0
  str_set_cstr(str1, "var_string");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@2");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@-5");
  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "substring");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_substring");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);
  //

  // startsAt >= length
  str_set_cstr(str1, "var_string");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@8");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@2");
  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "substring");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_substring");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);
  //

  // endsBefore > length
  str_set_cstr(str1, "var_string");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@9");
  stack_push(args, str_new_from_str(str1));
  str_set_cstr(str1, "int@5");
  stack_push(args, str_new_from_str(str1));

  str_set_cstr(str1, "substring");
  generator_function_call(gen, str1, args, str2);

  str_set_cstr(str2, "int@1");
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str2, "var_substring");
  stack_push(args, str_new_from_str(str2));

  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);
  //

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  // ab|cde|fgh, rest should be nil
  // 01 234 567
  EXPECT_EQ(strcmp(output, "cde"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, WhileLoop) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@9");
  generator_var_set(gen, str1, str2);

  stack_push(args, str_new_from_cstr("<"));
  stack_push(args, str_new_from_cstr("var_int"));
  stack_push(args, str_new_from_cstr("int@0"));

  generator_loop_start(gen, args);

  generator_var_create(gen, str_new_from_cstr("var_int1"));

  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("var_int"));
  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  stack_push(args, str_new_from_cstr("var_int"));
  stack_push(args, str_new_from_cstr("-"));
  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("var_int"));
  generator_expr(gen, args);

  generator_loop_end(gen);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  // deleteTmpFile();

  EXPECT_EQ(strcmp(output, "987654321"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, WhileLoopInFn) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  generator_function_begin(gen, str_new_from_cstr("test"), NULL);

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@9");
  generator_var_set(gen, str1, str2);

  stack_push(args, str_new_from_cstr("<"));
  stack_push(args, str_new_from_cstr("var_int"));
  stack_push(args, str_new_from_cstr("int@0"));

  generator_loop_start(gen, args);

  generator_var_create(gen, str_new_from_cstr("var_int1"));

  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("var_int"));
  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  stack_push(args, str_new_from_cstr("var_int"));
  stack_push(args, str_new_from_cstr("-"));
  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("var_int"));
  generator_expr(gen, args);

  generator_loop_end(gen);

  generator_function_end(gen, NULL);

  str_set_cstr(str1, "test");
  generator_function_call(gen, str1, NULL, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  EXPECT_EQ(strcmp(output, "987654321"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, ReturnExpr) {
  gen_t *gen = generator_new();
  generator_header(gen);

  void_stack_t *args = stack_new(20);

  str *str1 = str_new(50);
  str *str2 = str_new(50);

  str_set_cstr(str1, "var_ret");
  generator_var_create(gen, str1);

  str_set_cstr(str1, "test");
  generator_function_begin(gen, str1, NULL);

  str_set_cstr(str1, "var_int");
  generator_var_create(gen, str1);
  str_set_cstr(str2, "int@9");
  generator_var_set(gen, str1, str2);

  stack_push(args, str_new_from_cstr("var_int"));
  stack_push(args, str_new_from_cstr("-"));
  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("var_int"));
  generator_function_return_expr(gen, args);

  generator_function_end(gen, NULL);

  str_set_cstr(str1, "test");
  str_set_cstr(str2, "var_ret");
  generator_function_call(gen, str1, NULL, str2);

  stack_push(args, str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_str(str2));
  str_set_cstr(str1, "write");
  generator_function_call(gen, str1, args, NULL);

  generator_footer(gen);

  str_dispose(str2);
  str_dispose(str1);

  stack_dispose(args);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  EXPECT_EQ(strcmp(output, "9"), 0);

  generator_dispose(gen);
}

TEST(GeneratorTest, FunAndGames) {
  return;
  gen_t *gen = generator_new();

  generator_header(gen);

  str *name = str_new_from_cstr("a");
  generator_var_create(gen, name);

  generator_if_begin(gen, str_new_from_cstr("int@1"), true, str_new_from_cstr("int@1"));
  generator_if_else(gen);
  generator_var_create(gen, name);
  generator_if_end(gen);

  generator_function_begin(gen, str_new_from_cstr("test1"), NULL);
  generator_var_create(gen, name);
  generator_var_set(gen, name, str_new_from_cstr("int@1"));
  generator_if_begin(gen, str_new_from_cstr("a"), true, str_new_from_cstr("int@1"));
  generator_var_create(gen, name);
  generator_var_set(gen, name, str_new_from_cstr("int@1"));
  generator_if_else(gen);
  generator_var_create(gen, name);
  generator_if_end(gen);
  generator_if_begin(gen, str_new_from_cstr("a"), true, str_new_from_cstr("int@1"));
  generator_if_else(gen);
  generator_if_end(gen);
  generator_function_end(gen, NULL);

  void_stack_t *args = stack_new(20);
  stack_push(args, str_new_from_cstr("arg3"));
  stack_push(args, str_new_from_cstr("arg2"));
  stack_push(args, str_new_from_cstr("arg1"));

  generator_function_begin(gen, str_new_from_cstr("test4s"), args);
  generator_var_set(gen, str_new_from_cstr("a"), str_new_from_cstr("int@1"));
  generator_var_set(gen, str_new_from_cstr("arg1"), str_new_from_cstr("int@1"));
  stack_push(args, str_new_from_cstr("arg1"));
  generator_function_call(gen, str_new_from_cstr("test4s"), args, NULL);
  generator_function_end(gen, NULL);

  generator_var_create(gen, str_new_from_cstr("arg1"));
  generator_var_set(gen, str_new_from_cstr("arg1"), str_new_from_cstr("int@1"));

  generator_function_call(gen, str_new_from_cstr("test1"), NULL, NULL);

  stack_push(args, str_new_from_cstr("arg1"));
  generator_function_begin(gen, str_new_from_cstr("test-return"), args);
  generator_if_begin(gen, str_new_from_cstr("arg1"), false, str_new_from_cstr("int@1"));
  generator_function_return(gen, str_new_from_cstr("arg1"));
  generator_if_else(gen);
  generator_function_return(gen, str_new_from_cstr("int@0"));
  generator_if_end(gen);
  generator_function_end(gen, str_new_from_cstr("arg1"));

  generator_var_create(gen, str_new_from_cstr("returnvar"));
  stack_push(args, str_new_from_cstr("arg1"));
  generator_function_call(gen, str_new_from_cstr("test-return"), args,
  str_new_from_cstr("returnvar"));

  generator_var_set(gen, str_new_from_cstr("arg1"), str_new_from_cstr("int@5"));
  // arg1 = (arg1 * arg1) + arg1
  stack_push(args, str_new_from_cstr("arg1"));
  stack_push(args, str_new_from_cstr("+"));
  stack_push(args, str_new_from_cstr("*"));
  stack_push(args, str_new_from_cstr("arg1"));
  stack_push(args, str_new_from_cstr("arg1"));
  stack_push(args, str_new_from_cstr("arg1"));
  generator_expr(gen, args);

  stack_push(args, str_new_from_cstr("<"));
  stack_push(args, str_new_from_cstr("int@10"));
  stack_push(args, str_new_from_cstr("arg1"));

  generator_if_begin_stack(gen, true, args);
  generator_if_else(gen);
  generator_if_end(gen);

  generator_footer(gen);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  deleteTmpFile();

  EXPECT_EQ(returnCode, 0);
  EXPECT_STREQ(output, "");

  generator_dispose(gen);
}