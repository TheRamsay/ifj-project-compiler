extern "C" {
#include "../stack.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>

void delete_stack(void_stack_t *stack) {
  stack_dispose(stack);
  free(stack);
}

TEST(StackTest, Init) {
  void_stack_t *stack = stack_new(20);
  ASSERT_NE(stack, nullptr);

  ASSERT_EQ(stack->size, 20);
  ASSERT_EQ(stack->top_index, -1);
  ASSERT_EQ(stack->error_code, 0);
  ASSERT_NE(stack->items, nullptr);

  delete_stack(stack);
}

TEST(StackTest, Push) {
  void_stack_t *stack = stack_new(2);

  stack_push(stack, (void *)1);
  ASSERT_EQ(stack_top(stack), (void *)1);
  ASSERT_EQ(stack->top_index, 0);
  ASSERT_EQ(stack->error_code, 0);

  stack_push(stack, (void *)2);
  ASSERT_EQ(stack_top(stack), (void *)2);
  ASSERT_EQ(stack->top_index, 1);
  ASSERT_EQ(stack->error_code, 0);

  delete_stack(stack);
}

TEST(StackTest, PushResize) {
  void_stack_t *stack = stack_new(2);

  stack_push(stack, (void *)1);
  ASSERT_EQ(stack_top(stack), (void *)1);
  ASSERT_EQ(stack->top_index, 0);
  ASSERT_EQ(stack->error_code, 0);

  stack_push(stack, (void *)2);
  ASSERT_EQ(stack_top(stack), (void *)2);
  ASSERT_EQ(stack->top_index, 1);
  ASSERT_EQ(stack->error_code, 0);

  stack_push(stack, (void *)3);
  ASSERT_EQ(stack_top(stack), (void *)3);
  ASSERT_EQ(stack->top_index, 2);
  ASSERT_EQ(stack->size, 4);
  ASSERT_EQ(stack->error_code, 0);

  stack_push(stack, (void *)4);
  ASSERT_EQ(stack_top(stack), (void *)4);
  ASSERT_EQ(stack->top_index, 3);
  ASSERT_EQ(stack->size, 4);
  ASSERT_EQ(stack->error_code, 0);

  stack_push(stack, (void *)5);
  ASSERT_EQ(stack_top(stack), (void *)5);
  ASSERT_EQ(stack->top_index, 4);
  ASSERT_EQ(stack->size, 8);
  ASSERT_EQ(stack->error_code, 0);

  delete_stack(stack);
}

TEST(StackTest, Pop) {
  void_stack_t *stack = stack_new(2);

  stack_push(stack, (void *)1);
  ASSERT_EQ(stack_top(stack), (void *)1);
  ASSERT_EQ(stack->top_index, 0);
  ASSERT_EQ(stack->error_code, 0);

  ASSERT_EQ(stack_pop(stack), (void *)1);
  ASSERT_EQ(stack->top_index, -1);
  ASSERT_EQ(stack->error_code, 0);

  ASSERT_EQ(stack_pop(stack), (void *)0);
  ASSERT_EQ(stack->top_index, -1);
  ASSERT_EQ(stack->error_code, STACK_SERR_POP);

  delete_stack(stack);
}

TEST(StackTest, EmptyFull) {
  void_stack_t *stack = stack_new(1);

  ASSERT_TRUE(stack_is_empty(stack));
  ASSERT_FALSE(stack_is_full(stack));

  stack_push(stack, (void *)1);
  ASSERT_EQ(stack_top(stack), (void *)1);
  ASSERT_EQ(stack->top_index, 0);
  ASSERT_EQ(stack->error_code, 0);

  ASSERT_TRUE(stack_is_full(stack));
  ASSERT_FALSE(stack_is_empty(stack));

  stack_pop(stack);

  ASSERT_TRUE(stack_is_empty(stack));
  ASSERT_FALSE(stack_is_full(stack));

  delete_stack(stack);
}

TEST(StackTest, Dispose) {
  void_stack_t *stack = stack_new(1);

  stack_dispose(stack);

  ASSERT_EQ(stack->items, nullptr);
  ASSERT_EQ(stack->size, 0);
  ASSERT_EQ(stack->top_index, -1);

  stack_dispose(stack);
  free(stack);
}