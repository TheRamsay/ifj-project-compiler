extern "C"{

#include "../stack.h"
#include "../exp_parser.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>


//Basic tests
TEST(ExpParserTest, BasicTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i$"));

}

//Add tests
TEST(ExpParserTest, AddTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i+i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i+$"));
}

//Minus tests
TEST(ExpParserTest, MinusTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i-i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i-$"));
}

//Equals tests
TEST(ExpParserTest, EqualsTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i=i$"));
    ASSERT_TRUE(ParseExpression("i+i=i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i=$"));
}

//Lower tests
TEST(ExpParserTest, LowerTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i<i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i<$"));
}

//Lower tests
TEST(ExpParserTest, LowerOrEqualsTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i=<i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i=<$"));
}


//Higher tests
TEST(ExpParserTest, HigherTests) {
    //Should return true
    ASSERT_TRUE(ParseExpression("i>i$"));

    //Should return false
    ASSERT_FALSE(ParseExpression("i>$"));
}
