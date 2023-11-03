extern "C" {
#include "../str.h"
}

#include <string.h>

#include "gtest/gtest.h"

TEST(StrTest, New) {
  str *s = str_new(10);

  ASSERT_EQ(s->alloc_size, 10);
  ASSERT_NE(s->data, nullptr);

  str_destroy(s);

  s = str_new(0);

  ASSERT_EQ(s->alloc_size, 0);
  ASSERT_NE(s->data, nullptr);

  str_destroy(s);
}

TEST(StrTest, NewFrom) {
  str *s = str_new_from("");

  // Null terminator
  ASSERT_EQ(s->alloc_size, 1);
  ASSERT_EQ(strcmp(s->data, ""), 0);

  str_destroy(s);

  s = str_new_from("Hello, World!");

  ASSERT_EQ(s->alloc_size, 14);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_destroy(s);

  s = str_new_from(NULL);
  
  ASSERT_EQ(s, nullptr);
}

TEST(StrTest, SetCstr) {
  str *s = str_new(20);

  str_set_cstr(s, "Hello, World!");

  ASSERT_EQ(s->alloc_size, 20);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_set_cstr(s, NULL);
  
  ASSERT_NE(s, nullptr);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_set_cstr(s, "Hello, World! 2");

  ASSERT_EQ(s->alloc_size, 20);
  ASSERT_EQ(strcmp(s->data, "Hello, World! 2"), 0);

  str_destroy(s);
}

TEST(StrTest, SetStr) {
  str *s = str_new(20);
  str *c = str_new(20);

  str_set_cstr(s, "Hello, World!");
  str_set_str(c, s);

  ASSERT_EQ(strcmp(c->data, "Hello, World!"), 0);

  str_destroy(s);
  str_destroy(c);
}

TEST(StrTest, SetResize) {
  str *s = str_new(5);

  ASSERT_EQ(s->alloc_size, 5);

  str_set_cstr(s, "Hello, World!");

  ASSERT_EQ(s->alloc_size, 14);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_destroy(s);
}

TEST(StrTest, AppendCstr) {
  str *s = str_new(5);

  str_set_cstr(s, "1");
  
  ASSERT_EQ(strcmp(s->data, "1"), 0);

  str_append_cstr(s, "2");

  ASSERT_EQ(strcmp(s->data, "12"), 0);

  str_append_cstr(s, "3");

  ASSERT_EQ(strcmp(s->data, "123"), 0);

  str_destroy(s);
}

TEST(StrTest, AppendStr) {
  str *s = str_new(5);
  str *c = str_new(5);

  str_set_cstr(s, "1");
  str_set_cstr(c, "2");

  str_append_str(s, c);
  
  ASSERT_EQ(strcmp(s->data, "12"), 0);

  str_append_str(s, c);

  ASSERT_EQ(strcmp(s->data, "122"), 0);

  str_destroy(s);
  str_destroy(c);
}

TEST(StrTest, AppendCstrResize) {
  str *s = str_new(5);

  str_set_cstr(s, "123");
  
  ASSERT_EQ(strcmp(s->data, "123"), 0);

  str_append_cstr(s, "45678");

  ASSERT_EQ(s->alloc_size, 9);
  ASSERT_EQ(strcmp(s->data, "12345678"), 0);

  str_destroy(s);
}


TEST(StrTest, AppendCstrDestroy) {
  str *s = str_new(10);

  str_set_cstr(s, "123");

  char *c = (char *)malloc(sizeof(char) * 20);
  strcpy(c, "456");

  str_append_cstr_destroy(s, &c);

  ASSERT_EQ(strcmp(s->data, "123456"), 0);
  ASSERT_EQ(c, nullptr);

  str_destroy(s);
}

TEST(StrTest, AppendStrDestroy) {
  str *s = str_new_from("123");
  str *c = str_new_from("456");

  str_append_str_destroy(s, &c);

  ASSERT_EQ(strcmp(s->data, "123456"), 0);
  ASSERT_EQ(c, nullptr);

  str_destroy(s);
}