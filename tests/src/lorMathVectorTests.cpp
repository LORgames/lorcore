#include "lorTests.h"

template <typename T>
void lorMathVector2Tests_TestTypedVector()
{
  lorVector2<T> a = lorVector2<T>::Zero();
  lorVector2<T> b = lorVector2<T>::One();
  lorVector2<T> c = lorVector2<T>::One();

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a + b == b);
  EXPECT_TRUE(b - b == a);
  EXPECT_TRUE(b * a == a);
  EXPECT_TRUE(a * b == a);
  EXPECT_TRUE(b / b == b);
  EXPECT_TRUE(a / b == a);

  c.x = T(3.5);
  EXPECT_FALSE(c == b);
  EXPECT_TRUE(c == lorVector2<T>::Create(T(3.5), 1));
  EXPECT_TRUE(c * 1.5f == lorVector2<T>::Create((T)(T(3.5) * 1.5f), T(1.5)));
  EXPECT_TRUE(c * 3 == lorVector2<T>::Create(T(3.5) * 3, 3));
  EXPECT_TRUE(c / 3 == lorVector2<T>::Create(T(3.5) / 3, T(1) / 3));
  EXPECT_TRUE(c == lorVector2<T>::Create(T(3.5), 1));
}

TEST(MathVectors, Vector2Double)
{
  lorMathVector2Tests_TestTypedVector<double>();
}

TEST(MathVectors, Vector2Float)
{
  lorMathVector2Tests_TestTypedVector<float>();
}

TEST(MathVectors, Vector2Int)
{
  lorMathVector2Tests_TestTypedVector<int32_t>();
}
