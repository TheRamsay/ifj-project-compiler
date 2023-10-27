extern "C"
{
#include "../stack.h"
}

#include <gtest/gtest.h>
#include <stdlib.h>

Stack *create_stack(int size)
{
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack_init(stack, size);
  return stack;
}

void delete_stack(Stack *stack) {
  stack_dispose(stack);
  free(stack);
}

TEST(StackTest, Init)
{
  Stack *stack = NULL;
  ASSERT_FALSE(stack_init(stack, 20));

  stack = create_stack(20);
  
  ASSERT_EQ(stack->size, 20);
  ASSERT_EQ(stack->topIndex, -1);
  ASSERT_EQ(stack->errorCode, 0);
  ASSERT_NE(stack->items, nullptr);

  delete_stack(stack);
}

TEST(StackTest, Push)
{
  Stack *stack = create_stack(2);

  stack_push(stack, 1);
  ASSERT_EQ(stack_top(stack), 1);
  ASSERT_EQ(stack->topIndex, 0);
  ASSERT_EQ(stack->errorCode, 0);

  stack_push(stack, 2);
  ASSERT_EQ(stack_top(stack), 2);
  ASSERT_EQ(stack->topIndex, 1);
  ASSERT_EQ(stack->errorCode, 0);

  stack_push(stack, 3);
  ASSERT_EQ(stack_top(stack), 2);
  ASSERT_NE(stack->topIndex, 2);
  ASSERT_EQ(stack->errorCode, STACK_SERR_PUSH);

  delete_stack(stack);
}

TEST(StackTest, Pop)
{
  Stack *stack = create_stack(2);

  stack_push(stack, 1);
  ASSERT_EQ(stack_top(stack), 1);
  ASSERT_EQ(stack->topIndex, 0);
  ASSERT_EQ(stack->errorCode, 0);

  ASSERT_EQ(stack_pop(stack), 1);
  ASSERT_EQ(stack->topIndex, -1);
  ASSERT_EQ(stack->errorCode, 0);

  ASSERT_EQ(stack_pop(stack), 0);
  ASSERT_EQ(stack->topIndex, -1);
  ASSERT_EQ(stack->errorCode, STACK_SERR_POP);

  delete_stack(stack);
}

TEST(StackTest, EmptyFull)
{
  Stack *stack = create_stack(1);

  ASSERT_TRUE(stack_is_empty(stack));
  ASSERT_FALSE(stack_is_full(stack));

  stack_push(stack, 1);
  ASSERT_EQ(stack_top(stack), 1);
  ASSERT_EQ(stack->topIndex, 0);
  ASSERT_EQ(stack->errorCode, 0);

  ASSERT_TRUE(stack_is_full(stack));
  ASSERT_FALSE(stack_is_empty(stack));

  stack_pop(stack);

  ASSERT_TRUE(stack_is_empty(stack));
  ASSERT_FALSE(stack_is_full(stack));

  delete_stack(stack);
}

TEST(StackTest, Dispose)
{
  Stack *stack = create_stack(1);

  stack_dispose(stack);
  
  ASSERT_EQ(stack->items, nullptr);
  ASSERT_EQ(stack->size, 0);
  ASSERT_EQ(stack->topIndex, -1);

  stack_dispose(stack);
  free(stack);
}