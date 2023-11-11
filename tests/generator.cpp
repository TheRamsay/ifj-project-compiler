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
  Generator *gen = generator_new();

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
  Generator *gen = generator_new();

  generator_header(gen);

  str *name = str_new_from("a");
  generator_var_create(gen, name);
  str_dispose(name);
  generator_footer(gen);

  createTmpFile(gen->out_str);
  auto [output, returnCode] = run_interpreter("./tmp");
  // deleteTmpFile();

  EXPECT_EQ(returnCode, 0);
  EXPECT_STREQ(output, "");

  generator_dispose(gen);
}

void generator_function_begin(Generator *gen, str *name) {
  stack_push(gen->label_stack, (char *)name->data);

  str_append_cstr(gen->out_str, "LABEL ");
};
