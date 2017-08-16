#include "lorTests.h"

TEST(MathMatrix, BasicIdentyTest)
{
  lorMat4 a = lorMat4::Identity();
  lorMat4 b = lorMat4::Identity();

  EXPECT_EQ(a, b);
  EXPECT_EQ(a*b, b);
  EXPECT_EQ(a, a*b);
  EXPECT_EQ(a*b, b*a);
  EXPECT_EQ(a*a, b*b);
  EXPECT_EQ(a*a*a, b*b*b);
}

TEST(MathMatrix, BasicScalarTest)
{
  lorMat4 a = lorMat4::Identity() * 2;

  EXPECT_EQ(a.a[2], lorVec4::Create(0, 0, 2, 0));
}

TEST(MathMatrix, MatrixCreateFunctionCompileTest)
{
  lorMat4 a = lorMat4::PerspectiveFov(lorPI / 3, 1920, 1080, 1, 10000);
  lorMat4 b = lorMat4::LookAt(lorVec3::Zero(), lorVec3::Create(0, 1, 0), lorVec3::Create(0, 0, 1));
}
