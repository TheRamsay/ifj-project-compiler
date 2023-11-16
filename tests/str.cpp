extern "C" {
#include "../str.h"
}

#include <string.h>

#include "gtest/gtest.h"

TEST(StrTest, New) {
  str *s = str_new(10);

  ASSERT_EQ(s->alloc_size, 10);
  ASSERT_NE(s->data, nullptr);

  str_dispose(s);

  s = str_new(0);

  ASSERT_EQ(s, nullptr);
}

TEST(StrTest, NewFrom) {
  str *s = str_new_from_cstr("");

  // Null terminator
  ASSERT_EQ(s->alloc_size, 1);
  ASSERT_EQ(strcmp(s->data, ""), 0);

  str_dispose(s);

  s = str_new_from_cstr("Hello, World!");

  ASSERT_EQ(s->alloc_size, 14);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_dispose(s);

  s = str_new_from_cstr(NULL);
  
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

  str_dispose(s);
}

TEST(StrTest, SetStr) {
  str *s = str_new(20);
  str *c = str_new(20);

  str_set_cstr(s, "Hello, World!");
  str_set_str(c, s);

  ASSERT_EQ(strcmp(c->data, "Hello, World!"), 0);

  str_dispose(s);
  str_dispose(c);
}

TEST(StrTest, SetResize) {
  str *s = str_new(5);

  ASSERT_EQ(s->alloc_size, 5);

  str_set_cstr(s, "Hello, World!");

  ASSERT_EQ(s->alloc_size, 14);
  ASSERT_EQ(strcmp(s->data, "Hello, World!"), 0);

  str_dispose(s);
}

TEST(StrTest, AppendCstr) {
  str *s = str_new(5);

  str_set_cstr(s, "1");
  
  ASSERT_EQ(strcmp(s->data, "1"), 0);

  str_append_cstr(s, "2");

  ASSERT_EQ(strcmp(s->data, "12"), 0);

  str_append_cstr(s, "3");

  ASSERT_EQ(strcmp(s->data, "123"), 0);

  str_dispose(s);
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

  str_dispose(s);
  str_dispose(c);
}

TEST(StrTest, AppendInt) {
  str *s = str_new(5);
  int c = 123;

  str_set_cstr(s, "0");

  str_append_int(s, c);
  ASSERT_EQ(strcmp(s->data, "0123"), 0);

  str_append_int(s, c);
  ASSERT_EQ(strcmp(s->data, "0123123"), 0);

  str_set_cstr(s, "");
  c = 2147483647;

  str_append_int(s, c);
  ASSERT_EQ(strcmp(s->data, "2147483647"), 0);

  str_dispose(s);
}

TEST(StrTest, AppendCstrResize) {
  str *s = str_new(5);

  str_set_cstr(s, "123");
  
  ASSERT_EQ(strcmp(s->data, "123"), 0);

  str_append_cstr(s, "45678");

  ASSERT_EQ(s->alloc_size, 9);
  ASSERT_EQ(strcmp(s->data, "12345678"), 0);

  str_dispose(s);
}


TEST(StrTest, AppendCstrDestroy) {
  str *s = str_new(10);

  str_set_cstr(s, "123");

  char *c = (char *)malloc(sizeof(char) * 20);
  strcpy(c, "456");

  str_append_cstr_dispose(s, &c);

  ASSERT_EQ(strcmp(s->data, "123456"), 0);
  ASSERT_EQ(c, nullptr);

  str_dispose(s);
}

TEST(StrTest, AppendStrDestroy) {
  str *s = str_new_from_cstr("123");
  str *c = str_new_from_cstr("456");

  str_append_str_dispose(s, &c);

  ASSERT_EQ(strcmp(s->data, "123456"), 0);
  ASSERT_EQ(c, nullptr);

  str_dispose(s);
}