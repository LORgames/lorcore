#include "lorTests.h"

TEST(BasicMath, SquareRoot)
{
  EXPECT_EQ(3, lorSqrt(9));
  EXPECT_FLOAT_EQ(3.f, lorSqrt(9.f));
  EXPECT_DOUBLE_EQ(3.0, lorSqrt(9.0));

  EXPECT_EQ(1, lorSqrt(1));
  EXPECT_FLOAT_EQ(1.f, lorSqrt(1.f));
  EXPECT_DOUBLE_EQ(1.0, lorSqrt(1.0));
}

TEST(BasicMath, Absolute)
{
  EXPECT_EQ(3, lorAbs(-3));
  EXPECT_EQ(3, lorAbs(3));
  EXPECT_EQ(0, lorAbs(0));

  EXPECT_FLOAT_EQ(3.f, lorAbs(-3.f));
  EXPECT_FLOAT_EQ(3.f, lorAbs(3.f));
  EXPECT_FLOAT_EQ(0.f, lorAbs(0.f));

  EXPECT_DOUBLE_EQ(3.0, lorAbs(-3.0));
  EXPECT_DOUBLE_EQ(3.0, lorAbs(3.0));
  EXPECT_DOUBLE_EQ(0.0, lorAbs(0.0));
}

TEST(BasicMath, Min)
{
  EXPECT_EQ(-3, lorMin(-3, 3));
  EXPECT_EQ(3, lorMin(3, 9));
  EXPECT_EQ(0, lorMin(0, 12));

  EXPECT_FLOAT_EQ(-3.f, lorMin(-3.f, 3.f));
  EXPECT_FLOAT_EQ(3.f, lorMin(3.f, 9.f));
  EXPECT_FLOAT_EQ(0.f, lorMin(0.f, 12.f));

  EXPECT_DOUBLE_EQ(-3.0, lorMin(-3.0, 3.0));
  EXPECT_DOUBLE_EQ(3.0, lorMin(3.0, 9.0));
  EXPECT_DOUBLE_EQ(0.0, lorMin(0.0, 12.0));
}

TEST(BasicMath, Max)
{
  EXPECT_EQ(3, lorMax(-3, 3));
  EXPECT_EQ(9, lorMax(3, 9));
  EXPECT_EQ(12, lorMax(0, 12));

  EXPECT_FLOAT_EQ(3.f, lorMax(-3.f, 3.f));
  EXPECT_FLOAT_EQ(9.f, lorMax(3.f, 9.f));
  EXPECT_FLOAT_EQ(12.f, lorMax(0.f, 12.f));

  EXPECT_DOUBLE_EQ(3.0, lorMax(-3.0, 3.0));
  EXPECT_DOUBLE_EQ(9.0, lorMax(3.0, 9.0));
  EXPECT_DOUBLE_EQ(12.0, lorMax(0.0, 12.0));
}
