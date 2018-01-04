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


TEST(BasicMath, Round)
{
  EXPECT_EQ(-3, lorRound(-3));
  EXPECT_EQ(6, lorRound(6));
  EXPECT_EQ(0, lorRound(0));

  //Float tests
  EXPECT_FLOAT_EQ(-3.f, lorRound(-3.f));
  EXPECT_FLOAT_EQ(6.f, lorRound(6.f));
  EXPECT_FLOAT_EQ(0.f, lorRound(0.f));
  EXPECT_FLOAT_EQ(-3.f, lorRound(-3.1f));
  EXPECT_FLOAT_EQ(6.f, lorRound(5.9f));
  EXPECT_FLOAT_EQ(0.f, lorRound(0.4f));

  EXPECT_FLOAT_EQ(1.f, lorRound(0.51f));
  EXPECT_FLOAT_EQ(1.f, lorRound(0.50f));
  EXPECT_FLOAT_EQ(0.f, lorRound(0.49f));

  //Same for double
  EXPECT_DOUBLE_EQ(-3.0, lorRound(-3.0));
  EXPECT_DOUBLE_EQ(6.0, lorRound(6.0));
  EXPECT_DOUBLE_EQ(0.0, lorRound(0.0));
  EXPECT_DOUBLE_EQ(-3.0, lorRound(-3.1));
  EXPECT_DOUBLE_EQ(6.0, lorRound(5.9));
  EXPECT_DOUBLE_EQ(0.0, lorRound(0.4));

  EXPECT_DOUBLE_EQ(1.0, lorRound(0.51));
  EXPECT_DOUBLE_EQ(1.0, lorRound(0.50));
  EXPECT_DOUBLE_EQ(0.0, lorRound(0.49));
}

TEST(BasicMath, Random)
{
  const uint64_t randomStart = 64;
  uint64_t randomSeed = randomStart;
  uint32_t randomVal = lorRandom(randomSeed);

  EXPECT_NE(randomSeed, randomStart);
  EXPECT_NE(randomSeed, randomVal);
  EXPECT_NE(randomVal, randomStart);
}
