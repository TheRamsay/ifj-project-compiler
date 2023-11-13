extern "C" {
#include "../generator.h"
}

#include <filesystem>
#include <stdlib.h>
#include <strings.h>

#include "./test_utils.cpp"
#include "gtest/gtest.h"

void createTmpFile(str *content) {
  FILE *tmpFile = fopen("./tmp", "w");
  fwrite(content->data, 1, strlen(content->data), tmpFile);
  fclose(tmpFile);
}

void deleteTmpFile() {
  std::filesystem::remove("./tmp");
}

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

TEST(GeneratorTest, FunAndGames) {
  gen_t *gen = generator_new();

  generator_header(gen);

  str *name = str_new_from("a");
  generator_var_create(gen, name);

  generator_if_begin(gen, str_new_from("int@1"), true, str_new_from("int@1"));
  generator_if_else(gen);
  generator_var_create(gen, name);
  generator_if_end(gen);

  generator_function_begin(gen, str_new_from("test1"), NULL);
  generator_var_create(gen, name);
  generator_var_set(gen, name, str_new_from("int@1"));
  generator_if_begin(gen, str_new_from("a"), true, str_new_from("int@1"));
  generator_var_create(gen, name);
  generator_var_set(gen, name, str_new_from("int@1"));
  generator_if_else(gen);
  generator_var_create(gen, name);
  generator_if_end(gen);
  generator_if_begin(gen, str_new_from("a"), true, str_new_from("int@1"));
  generator_if_else(gen);
  generator_if_end(gen);
  generator_function_end(gen, NULL);

  void_stack_t *args = stack_new(20);
  stack_push(args, str_new_from("arg3"));
  stack_push(args, str_new_from("arg2"));
  stack_push(args, str_new_from("arg1"));

  generator_function_begin(gen, str_new_from("test4s"), args);
  generator_var_set(gen, str_new_from("a"), str_new_from("int@1"));
  generator_var_set(gen, str_new_from("arg1"), str_new_from("int@1"));
  stack_push(args, str_new_from("arg1"));
  generator_function_call(gen, str_new_from("test4s"), args, NULL);
  generator_function_end(gen, NULL);

  generator_var_create(gen, str_new_from("arg1"));
  generator_var_set(gen, str_new_from("arg1"), str_new_from("int@1"));

  generator_function_call(gen, str_new_from("test1"), NULL, NULL);



  stack_push(args, str_new_from("arg1"));
  generator_function_begin(gen, str_new_from("test-return"), args);
  generator_if_begin(gen, str_new_from("arg1"), false, str_new_from("int@1"));
  generator_function_return(gen, str_new_from("arg1"));
  generator_if_else(gen);
  generator_function_return(gen, str_new_from("int@0"));
  generator_if_end(gen);
  generator_function_end(gen, str_new_from("arg1"));

  generator_var_create(gen, str_new_from("returnvar"));
  stack_push(args, str_new_from("arg1"));
  generator_function_call(gen, str_new_from("test-return"), args, str_new_from("returnvar"));

  generator_var_set(gen, str_new_from("arg1"), str_new_from("int@5"));
  stack_push(args, str_new_from("arg1"));
  stack_push(args, str_new_from("+"));
  stack_push(args, str_new_from("*"));
  stack_push(args, str_new_from("arg1"));
  stack_push(args, str_new_from("arg1"));
  stack_push(args, str_new_from("arg1"));
  generator_expr(gen, args);

  stack_push(args, str_new_from("<"));
  stack_push(args, str_new_from("int@10"));
  stack_push(args, str_new_from("arg1"));

  generator_if_begin_stack(gen, true, args);
  generator_if_else(gen);
  generator_if_end(gen);

  generator_footer(gen);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  // deleteTmpFile();

  EXPECT_EQ(returnCode, 0);
  EXPECT_STREQ(output, "");

  generator_dispose(gen);
}