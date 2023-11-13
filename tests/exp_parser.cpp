extern "C"{

#include "../stack.h"
#include "../exp_parser.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>

TEST(ExpParserTest, Test1) {
    char *exp = "i+i$";

    int result = ParseExpression(exp);

    EXPECT_EQ(1, result);
}
