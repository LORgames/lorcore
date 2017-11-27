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

  {
    lorMat4 inverse = lorMat4::AffineInverse(a);
    EXPECT_EQ(a, inverse);
  }

  {
    lorMat4 inverse = lorMat4::Inverse(a);
    EXPECT_EQ(a, inverse);
  }
}

TEST(MathMatrix, BasicScalarTest)
{
  lorMat4 a = lorMat4::Identity() * 2;

  EXPECT_EQ(a.a[2], lorVec4::Create(0, 0, 2, 0));
}

TEST(MathMatrix, MatrixCreateFunctionCompileTest)
{
  ///*lorMat4 a = */lorMat4::PerspectiveFov(lorPI / 3, 1920, 1080, 1, 10000);
  ///*lorMat4 b = */lorMat4::LookAt(lorVec3::Zero(), lorVec3::Create(0, 1, 0), lorVec3::Create(0, 0, 1));
}

TEST(MathMatrix, MatrixMultiplyTest)
{
  lorMat4 a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  lorMat4 b = { 1, 10, 100, 1000, 2, 20, 200, 2000, 3, 30, 300, 3000, 4, 40, 400, 4000 };

  lorMat4 c = a * b;

  //Calculated by hand
  lorMat4 expected = { 12840, 13951, 15062, 16173, 25680, 27902, 30124, 32346, 38520, 41853, 45186, 48519, 51360, 55804, 60248, 64692 };

  EXPECT_TRUE(c == expected);
}

TEST(MathMatrix, MatrixFullMultTest)
{
  lorVec2 i0 = lorVec2::Create(0, 0);
  lorVec2 i1 = lorVec2::Create(1, 1);

  lorVec3 cameraPosition = lorVec3::Create(1.f, 0.f, 1.f);
  lorVec3 lookPosition = cameraPosition + lorVec3::WorldForward();

  //Experimental Matrix Version
  lorMat4 viewport = lorMat4::Viewport(0, 0, 100, 100);
  lorMat4 perspective = lorMat4::PerspectiveFov(lorPI / 2, 1280.f, 720.f, 1.f, 2.f);
  lorMat4 view = lorMat4::LookAt(cameraPosition, lookPosition, lorVec3::WorldUp());

  lorVec4 pos0 = viewport * perspective * view * lorVec4::Create(i0, 0.f, 1.f);
  lorVec4 pos1 = viewport * perspective * view * lorVec4::Create(i1, 0.f, 1.f);

  EXPECT_FLOAT_EQ(0, pos0.x);
}
