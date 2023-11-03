extern "C" {
#include "../generator.h"
}

#include <stdlib.h>
#include <strings.h>

#include "./test_utils.cpp"
#include "gtest/gtest.h"

TEST(GeneratorTest, Header) {
  Generator *gen = generator_new();

  generator_header(gen);
  auto [p_stdin, p_stdout] = popen2();

  fwrite(gen->out_str->data, 1, strlen(gen->out_str->data), p_stdin);
  fclose(p_stdin);
  fclose(p_stdout);

  generator_dispose(gen);
}