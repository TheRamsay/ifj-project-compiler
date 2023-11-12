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

  generator_function_begin(gen, str_new_from("test1"), NULL);
  generator_var_create(gen, name);
  generator_function_end(gen);

  void_stack_t *args = stack_new(4);
  stack_push(args, str_new_from("arg1"));

  generator_function_begin(gen, str_new_from("test4s"), args);
  stack_push(args, str_new_from("arg1"));
  generator_function_call(gen, str_new_from("test4s"), args);
  generator_function_end(gen);

  generator_var_create(gen, str_new_from("arg1"));
  generator_var_set(gen, str_new_from("arg1"), str_new_from("int@1"));

  stack_push(args, str_new_from("arg1"));
  generator_function_call(gen, str_new_from("test4s"), args);

  generator_footer(gen);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  // deleteTmpFile();

  EXPECT_EQ(returnCode, 0);
  EXPECT_STREQ(output, "");

  generator_dispose(gen);
}