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

  EXPECT_EQ(T(1), lorDot(lorVector2<T>::Create(1, 0), lorVector2<T>::Create(1, 0))); //Parallel
  EXPECT_EQ(T(0), lorDot(lorVector2<T>::Create(1, 0), lorVector2<T>::Create(0, 1))); //Perpendicular

  //No point testing some functions on integer types
  constexpr bool isFloat = (T(0.5) != T(0));
  if (isFloat)
  {
    lorVector2<T> d = lorNormalize(lorVector2<T>::One());
    T expectedMag = lorSqrt(T(1) / 2);

    EXPECT_DOUBLE_EQ(expectedMag, d.x);
    EXPECT_DOUBLE_EQ(expectedMag, d.y);
  }
}

template <typename T>
void lorMathVector3Tests_TestTypedVector()
{
  lorVector3<T> a = lorVector3<T>::Zero();
  lorVector3<T> b = lorVector3<T>::One();
  lorVector3<T> c = lorVector3<T>::One();

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a + b == b);
  EXPECT_TRUE(b - b == a);
  EXPECT_TRUE(b * a == a);
  EXPECT_TRUE(a * b == a);
  EXPECT_TRUE(b / b == b);
  EXPECT_TRUE(a / b == a);

  c.x = T(3.5);
  EXPECT_FALSE(c == b);
  EXPECT_TRUE(c == lorVector3<T>::Create(T(3.5), T(1), T(1)));
  EXPECT_TRUE(c * 1.5f == lorVector3<T>::Create((T)(T(3.5) * 1.5f), T(1.5), T(1.5)));
  EXPECT_TRUE(c * 3 == lorVector3<T>::Create(T(3.5) * 3, T(3), T(3)));
  EXPECT_TRUE(c / 3 == lorVector3<T>::Create(T(3.5) / 3, T(1) / 3, T(1) / 3));
  EXPECT_TRUE(c == lorVector3<T>::Create(T(3.5), T(1), T(1)));

  EXPECT_EQ(T(1), lorDot(lorVector3<T>::Create(1, 0, 0), lorVector3<T>::Create(1, 0, 0))); //Parallel
  EXPECT_EQ(T(0), lorDot(lorVector3<T>::Create(1, 0, 0), lorVector3<T>::Create(0, 1, 0))); //Perpendicular

  EXPECT_EQ(lorVector3<T>::Create(0, 0, 1), lorCross(lorVector3<T>::Create(1, 0, 0), lorVector3<T>::Create(0, 1, 0)));

  //No point testing some functions on integer types
  constexpr bool isFloat = (T(0.5) != T(0));
  if (isFloat)
  {
    lorVector3<T> d = lorNormalize(lorVector3<T>::One());
    T expectedMag = lorSqrt(T(1) / 3);
    EXPECT_DOUBLE_EQ(expectedMag, d.x);
    EXPECT_DOUBLE_EQ(expectedMag, d.y);
    EXPECT_DOUBLE_EQ(expectedMag, d.z);
  }
}

template <typename T>
void lorMathVector4Tests_TestTypedVector()
{
  lorVector4<T> a = lorVector4<T>::Zero();
  lorVector4<T> b = lorVector4<T>::One();
  lorVector4<T> c = lorVector4<T>::One();

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a + b == b);
  EXPECT_TRUE(b - b == a);
  EXPECT_TRUE(b * a == a);
  EXPECT_TRUE(a * b == a);
  EXPECT_TRUE(b / b == b);
  EXPECT_TRUE(a / b == a);

  c.x = T(3.5);
  EXPECT_FALSE(c == b);
  EXPECT_TRUE(c == lorVector4<T>::Create(T(3.5), T(1), T(1), T(1)));
  EXPECT_TRUE(c * 1.5f == lorVector4<T>::Create((T)(T(3.5) * 1.5f), T(1.5), T(1.5), T(1.5)));
  EXPECT_TRUE(c * 3 == lorVector4<T>::Create(T(3.5) * 3, T(3), T(3), T(3)));
  EXPECT_TRUE(c / 3 == lorVector4<T>::Create(T(3.5) / 3, T(1) / 3, T(1) / 3, T(1) / 3));
  EXPECT_TRUE(c == lorVector4<T>::Create(T(3.5), T(1), T(1), T(1)));

  EXPECT_EQ(T(1), lorDot(lorVector4<T>::Create(1, 0, 0, 0), lorVector4<T>::Create(1, 0, 0, 0))); //Parallel
  EXPECT_EQ(T(0), lorDot(lorVector4<T>::Create(1, 0, 0, 0), lorVector4<T>::Create(0, 1, 0, 0))); //Perpendicular

  //No point testing some functions on integer types
  constexpr bool isFloat = (T(0.5) != T(0));
  if (isFloat)
  {
    lorVector4<T> d = lorNormalize(lorVector4<T>::One());
    T expectedMag = lorSqrt(T(1) / 4);
    EXPECT_DOUBLE_EQ(expectedMag, d.x);
    EXPECT_DOUBLE_EQ(expectedMag, d.y);
    EXPECT_DOUBLE_EQ(expectedMag, d.z);
    EXPECT_DOUBLE_EQ(expectedMag, d.w);
  }
}

TEST(MathVectors, VectorDouble)
{
  lorMathVector2Tests_TestTypedVector<double>();
  lorMathVector3Tests_TestTypedVector<double>();
  lorMathVector4Tests_TestTypedVector<double>();
}

TEST(MathVectors, VectorFloat)
{
  lorMathVector2Tests_TestTypedVector<float>();
  lorMathVector3Tests_TestTypedVector<float>();
  lorMathVector4Tests_TestTypedVector<float>();
}

TEST(MathVectors, VectorInt)
{
  lorMathVector2Tests_TestTypedVector<int32_t>();
  lorMathVector3Tests_TestTypedVector<int32_t>();
  lorMathVector4Tests_TestTypedVector<int32_t>();
}
