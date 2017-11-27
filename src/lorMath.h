#ifndef LOR_MATH
#define LOR_MATH

#include <stdint.h>
#include <inttypes.h>
#include <math.h> // This provides sinf and is required for Linux (how does everything else get this?)

#include "lorPlatform.h"

#define lorPI 3.14159f

///////////////////////////////////////////////////////////////////////////////
// Vector2
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector2
{
  T x;
  T y;

  static lorVector2<T> Create(T x, T y)
  {
    return { x, y };
  };

  static lorVector2<T> Zero()
  {
    return { T(0), T(0) };
  };

  static lorVector2<T> One()
  {
    return { T(1), T(1) };
  };
};

// Comparisons
template <typename T> bool operator ==(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return ((a.x == b.x) && (a.y == b.y));
};

template <typename T> bool operator !=(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return !(a == b);
};

// Arithmetic
template <typename T> lorVector2<T> operator +(const lorVector2<T> &a)
{
  return { a.x, a.y };
};

template <typename T> lorVector2<T> operator -(const lorVector2<T> &a)
{
  return { -a.x, -a.y };
};

template <typename T> lorVector2<T> operator +(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return{ a.x + b.x, a.y + b.y };
};

template <typename T> lorVector2<T> operator -(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return{ a.x - b.x, a.y - b.y };
};

template <typename T> lorVector2<T> operator *(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return { a.x * b.x, a.y * b.y };
};

template <typename T> lorVector2<T> operator /(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return { a.x / b.x, a.y / b.y };
};

template <typename T, typename U> lorVector2<T> operator *(const lorVector2<T> &a, const U &b)
{
  return{ T(a.x * b), T(a.y * b) };
};

template <typename T, typename U> lorVector2<T> operator /(const lorVector2<T> &a, const U &b)
{
  return{ T(a.x / b), T(a.y / b) };
};

//Functions
template <typename T> T lorMag(const lorVector2<T> &a)
{
  return lorSqrt(a.x*a.x + a.y*a.y);
};

template <typename T> T lorDot(const lorVector2<T> &a, const lorVector2<T> &b)
{
  return (a.x * b.x + a.y * b.y);
};

template <typename T> lorVector2<T> lorNormalize(const lorVector2<T> &a)
{
  T mag = lorMag(a);
  return{ a.x / mag, a.y / mag };
};

///////////////////////////////////////////////////////////////////////////////
// Vector3
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector3
{
  T x;
  T y;
  T z;

  static lorVector3<T> Create(T x, T y, T z)
  {
    return { x, y, z };
  };

  static lorVector3<T> Create(lorVector2<T> o, T z)
  {
    return{ o.x, o.y, z };
  };

  static lorVector3<T> Zero()
  {
    return { T(0), T(0), T(0) };
  };

  static lorVector3<T> One()
  {
    return { T(1), T(1), T(1) };
  };

  static lorVector3<T> WorldForward()
  {
    return{ T(0), T(1), T(0) };
  };

  static lorVector3<T> WorldBackward()
  {
    return{ T(0), -T(1), T(0) };
  };

  static lorVector3<T> WorldUp()
  {
    return{ T(0), T(0), T(1) };
  };

  static lorVector3<T> WorldDown()
  {
    return{ T(0), T(0), -T(1) };
  };

  static lorVector3<T> WorldLeft()
  {
    return{ -T(1), T(0), T(0) };
  };

  static lorVector3<T> WorldRight()
  {
    return{ T(1), T(0), T(0) };
  };
};

// Comparisons
template <typename T> bool operator ==(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
};

template <typename T> bool operator !=(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return !(a == b);
};

// Arithmetic
template <typename T> lorVector3<T> operator +(const lorVector3<T> &a)
{
  return{ a.x, a.y, a.z };
};

template <typename T> lorVector3<T> operator -(const lorVector3<T> &a)
{
  return{ -a.x, -a.y, -a.z };
};

template <typename T> lorVector3<T> operator +(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return{ a.x + b.x, a.y + b.y, a.z + b.z };
};

template <typename T> lorVector3<T> operator -(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return{ a.x - b.x, a.y - b.y, a.z - b.z };
};

template <typename T> lorVector3<T> operator *(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return{ a.x * b.x, a.y * b.y, a.z * b.z };
};

template <typename T> lorVector3<T> operator /(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return{ a.x / b.x, a.y / b.y, a.z / b.z };
};

template <typename T, typename U> lorVector3<T> operator *(const lorVector3<T> &a, const U &b)
{
  return{ T(a.x * b), T(a.y * b), T(a.z * b) };
};

template <typename T, typename U> lorVector3<T> operator /(const lorVector3<T> &a, const U &b)
{
  return{ T(a.x / b), T(a.y / b), T(a.z / b) };
};

//Functions
template <typename T> T lorMag(const lorVector3<T> &a)
{
  return lorSqrt(a.x*a.x + a.y*a.y + a.z*a.z);
};

template <typename T> T lorDot(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z);
};

template <typename T> lorVector3<T> lorCross(const lorVector3<T> &a, const lorVector3<T> &b)
{
  return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
};

template <typename T> lorVector3<T> lorNormalize(const lorVector3<T> &a)
{
  T mag = lorMag(a);
  return{ a.x / mag, a.y / mag, a.z / mag };
};

///////////////////////////////////////////////////////////////////////////////
// Vector4
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector4
{
  T x;
  T y;
  T z;
  T w;

  static lorVector4<T> Create(T x, T y, T z, T w)
  {
    return{ x, y, z, w };
  };

  static lorVector4<T> Create(lorVector2<T> o, T z, T w)
  {
    return{ o.x, o.y, z, w };
  };

  static lorVector4<T> Create(lorVector3<T> o, T w)
  {
    return{ o.x, o.y, o.z, w };
  };

  static lorVector4<T> Zero()
  {
    return{ T(0), T(0), T(0), T(0) };
  };

  static lorVector4<T> One()
  {
    return{ T(1), T(1), T(1), T(1) };
  };
};

// Comparisons
template <typename T> bool operator ==(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
};

template <typename T> bool operator !=(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return !(a == b);
};

// Arithmetic
template <typename T> lorVector4<T> operator +(const lorVector4<T> &a)
{
  return{ a.x, a.y, a.z, a.w };
};

template <typename T> lorVector4<T> operator -(const lorVector4<T> &a)
{
  return{ -a.x, -a.y, -a.z, -a.w };
};

template <typename T> lorVector4<T> operator +(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
};

template <typename T> lorVector4<T> operator -(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
};

template <typename T> lorVector4<T> operator *(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
};

template <typename T> lorVector4<T> operator /(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
};

template <typename T, typename U> lorVector4<T> operator *(const lorVector4<T> &a, const U &b)
{
  return{ T(a.x * b), T(a.y * b), T(a.z * b), T(a.w * b) };
};

template <typename T, typename U> lorVector4<T> operator /(const lorVector4<T> &a, const U &b)
{
  return{ T(a.x / b), T(a.y / b), T(a.z / b), T(a.w / b) };
};

//Functions
template <typename T> T lorMag(const lorVector4<T> &a)
{
  return lorSqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
};

template <typename T> T lorDot(const lorVector4<T> &a, const lorVector4<T> &b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
};

template <typename T> lorVector4<T> lorNormalize(const lorVector4<T> &a)
{
  T mag = lorMag(a);
  return{ a.x / mag, a.y / mag, a.z / mag, a.w / mag };
};

///////////////////////////////////////////////////////////////////////////////
// Matrix4
///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct lorMatrix4
{
  union
  {
    T m[16];
    lorVector4<T> a[4];
  };

  static lorMatrix4<T> Identity()
  {
    return
    {
      T(1), T(0), T(0), T(0),
      T(0), T(1), T(0), T(0),
      T(0), T(0), T(1), T(0),
      T(0), T(0), T(0), T(1)
    };
  };

  static lorMatrix4<T> Zero()
  {
    return
    {
      T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0)
    };
  };

  static lorMatrix4<T> PerspectiveFov(T fov, T width, T height, T zNear, T zFar)
  {
    if (width <= T(0) || height <= T(0) || fov <= T(0))
      return Identity();

    T const h = lorCos(T(0.5) * fov) / lorSin(T(0.5) * fov);
    T const w = h * height / width; //Aspect ratio

    lorMatrix4<T> retVal = lorMatrix4<T>::Zero();

    retVal.a[0].x = w;
    retVal.a[2].y = h;
    retVal.a[1].w = T(-1);

    retVal.a[1].z = zFar / (zFar - zNear);
    retVal.a[3].z = -(zFar * zNear) / (zFar - zNear);

    return retVal;
  };

  static lorMatrix4<T> LookAt(lorVector3<T> eye, lorVector3<T> center, lorVector3<T> up)
  {
    lorVector3<T> f = lorNormalize(center - eye); //Y axis?
    lorVector3<T> s = lorNormalize(lorCross(f, up)); //X axis
    lorVector3<T> u = lorCross(s, f); //Z Axis

    lorMatrix4<T> retVal = Identity();
    retVal.a[0].x = s.x;
    retVal.a[1].x = s.y;
    retVal.a[2].x = s.z;
    retVal.a[3].x = lorDot(s, -eye);
    retVal.a[0].y = f.x;
    retVal.a[1].y = f.y;
    retVal.a[2].y = f.z;
    retVal.a[3].y = lorDot(f, -eye);
    retVal.a[0].z = u.x;
    retVal.a[1].z = u.y;
    retVal.a[2].z = u.z;
    retVal.a[3].z = lorDot(u, -eye);

    return retVal;
  };

  static lorMatrix4<T> Viewport(T x, T y, T w, T h, T nearPlane = 0.f, T farPlane = 1.f)
  {
    T l = x;
    T r = x + w;
    T t = y;
    T b = y + h;
    T n = nearPlane;
    T f = farPlane;

    lorMatrix4<T> retVal = Identity();
    retVal.a[0].x = (r - l) / T(2);
    retVal.a[1].y = (t - b) / T(2);
    retVal.a[2].z = (f - n) / T(2);

    retVal.a[3].x = (r + l) / T(2);
    retVal.a[3].y = (t + b) / T(2);
    retVal.a[3].z = (f + n) / T(2);

    return retVal;
  }

  static lorMatrix4<T> UniformScale(T s)
  {
    lorMatrix4<T> retVal = Identity();
    retVal.a[0].x = s;
    retVal.a[1].y = s;
    retVal.a[2].z = s;
    retVal.a[3].w = s;
    return retVal;
  }

  static lorMatrix4<T> NonUniformScale(T x, T y, T z, T w = 1.f)
  {
    lorMatrix4<T> retVal = Identity();
    retVal.a[0].x = x;
    retVal.a[1].y = y;
    retVal.a[2].z = z;
    retVal.a[3].w = w;
    return retVal;
  }

  static lorMatrix4<T> AffineInverse(const lorMatrix4<T> &m)
  {
    //Borrowed from GLM

    T oneOverDeterminant = T(1) / (
      + m.a[0].x * (m.a[1].y * m.a[2].z - m.a[2].y * m.a[1].z)
      - m.a[1].x * (m.a[0].y * m.a[2].z - m.a[2].y * m.a[0].z)
      + m.a[2].x * (m.a[0].y * m.a[1].z - m.a[1].y * m.a[0].z));

    lorMatrix4<T> inverse = lorMatrix4<T>::Identity();

    //3x3 Part
    inverse.a[0].x = +(m.a[1].y * m.a[2].z - m.a[2].y * m.a[1].z) * oneOverDeterminant;
    inverse.a[1].x = -(m.a[1].x * m.a[2].z - m.a[2].x * m.a[1].z) * oneOverDeterminant;
    inverse.a[2].x = +(m.a[1].x * m.a[2].y - m.a[2].x * m.a[1].y) * oneOverDeterminant;
    inverse.a[0].y = -(m.a[0].y * m.a[2].z - m.a[2].y * m.a[0].z) * oneOverDeterminant;
    inverse.a[1].y = +(m.a[0].x * m.a[2].z - m.a[2].x * m.a[0].z) * oneOverDeterminant;
    inverse.a[2].y = -(m.a[0].x * m.a[2].y - m.a[2].x * m.a[0].y) * oneOverDeterminant;
    inverse.a[0].z = +(m.a[0].y * m.a[1].z - m.a[1].y * m.a[0].z) * oneOverDeterminant;
    inverse.a[1].z = -(m.a[0].x * m.a[1].z - m.a[1].x * m.a[0].z) * oneOverDeterminant;
    inverse.a[2].z = +(m.a[0].x * m.a[1].y - m.a[1].x * m.a[0].y) * oneOverDeterminant;

    lorVector4<T> translation = -inverse * lorVector4<T>::Create(m.a[3].x, m.a[3].y, m.a[3].z, 0.f);

    inverse.a[3].x = translation.x;
    inverse.a[3].y = translation.y;
    inverse.a[3].z = translation.z;

    return inverse;
  }

  static lorMatrix4<T> Inverse(const lorMatrix4<T> &m)
  {
    //Borrowed from GLM

    T coef00 = m.a[2].z * m.a[3].w - m.a[3].z * m.a[2].w;
    T coef02 = m.a[1].z * m.a[3].w - m.a[3].z * m.a[1].w;
    T coef03 = m.a[1].z * m.a[2].w - m.a[2].z * m.a[1].w;
    T coef04 = m.a[2].y * m.a[3].w - m.a[3].y * m.a[2].w;
    T coef06 = m.a[1].y * m.a[3].w - m.a[3].y * m.a[1].w;
    T coef07 = m.a[1].y * m.a[2].w - m.a[2].y * m.a[1].w;
    T coef08 = m.a[2].y * m.a[3].z - m.a[3].y * m.a[2].z;
    T coef10 = m.a[1].y * m.a[3].z - m.a[3].y * m.a[1].z;
    T coef11 = m.a[1].y * m.a[2].z - m.a[2].y * m.a[1].z;
    T coef12 = m.a[2].x * m.a[3].w - m.a[3].x * m.a[2].w;
    T coef14 = m.a[1].x * m.a[3].w - m.a[3].x * m.a[1].w;
    T coef15 = m.a[1].x * m.a[2].w - m.a[2].x * m.a[1].w;
    T coef16 = m.a[2].x * m.a[3].z - m.a[3].x * m.a[2].z;
    T coef18 = m.a[1].x * m.a[3].z - m.a[3].x * m.a[1].z;
    T coef19 = m.a[1].x * m.a[2].z - m.a[2].x * m.a[1].z;
    T coef20 = m.a[2].x * m.a[3].y - m.a[3].x * m.a[2].y;
    T coef22 = m.a[1].x * m.a[3].y - m.a[3].x * m.a[1].y;
    T coef23 = m.a[1].x * m.a[2].y - m.a[2].x * m.a[1].y;

    lorVector4<T> fac0 = { coef00, coef00, coef02, coef03 };
    lorVector4<T> fac1 = { coef04, coef04, coef06, coef07 };
    lorVector4<T> fac2 = { coef08, coef08, coef10, coef11 };
    lorVector4<T> fac3 = { coef12, coef12, coef14, coef15 };
    lorVector4<T> fac4 = { coef16, coef16, coef18, coef19 };
    lorVector4<T> fac5 = { coef20, coef20, coef22, coef23 };

    lorVector4<T> vec0 = { m.a[1].x, m.a[0].x, m.a[0].x, m.a[0].x };
    lorVector4<T> vec1 = { m.a[1].y, m.a[0].y, m.a[0].y, m.a[0].y };
    lorVector4<T> vec2 = { m.a[1].z, m.a[0].z, m.a[0].z, m.a[0].z };
    lorVector4<T> vec3 = { m.a[1].w, m.a[0].w, m.a[0].w, m.a[0].w };

    lorVector4<T> inv0 = { vec1 * fac0 - vec2 * fac1 + vec3 * fac2 };
    lorVector4<T> inv1 = { vec0 * fac0 - vec2 * fac3 + vec3 * fac4 };
    lorVector4<T> inv2 = { vec0 * fac1 - vec1 * fac3 + vec3 * fac5 };
    lorVector4<T> inv3 = { vec0 * fac2 - vec1 * fac4 + vec2 * fac5 };

    lorVector4<T> signA = { +1, -1, +1, -1 };
    lorVector4<T> signB = { -1, +1, -1, +1 };

    lorMatrix4<T> inverse;
    inverse.a[0] = inv0 * signA;
    inverse.a[1] = inv1 * signB;
    inverse.a[2] = inv2 * signA;
    inverse.a[3] = inv3 * signB;

    lorVector4<T> row0 = { inverse.a[0].x, inverse.a[1].x, inverse.a[2].x, inverse.a[3].x };

    lorVector4<T> dot0 = (m.a[0] * row0);
    T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

    T oneOverDeterminant = T(1) / dot1;

    return inverse * oneOverDeterminant;
  }
};

// Comparisons
template <typename T> bool operator ==(const lorMatrix4<T> &a, const lorMatrix4<T> &b)
{
  bool equal = true;

  for (int i = 0; i < 16 && equal; ++i)
  {
    equal = (equal && (a.m[i] == b.m[i]));
  }

  return equal;
};

template <typename T> bool operator !=(const lorMatrix4<T> &a, const lorMatrix4<T> &b)
{
  return !(a == b);
};

// Arithmetic
template <typename T> lorMatrix4<T> operator +(const lorMatrix4<T> &a)
{
  return
  {
    a.m[0], a.m[1], a.m[2], a.m[3],
    a.m[4], a.m[5], a.m[6], a.m[7],
    a.m[8], a.m[9], a.m[10], a.m[11],
    a.m[12], a.m[13], a.m[14], a.m[15]
  };
};

template <typename T> lorMatrix4<T> operator -(const lorMatrix4<T> &a)
{
  return (a * -1);
};

template <typename T> lorMatrix4<T> operator +(const lorMatrix4<T> &a, const lorMatrix4<T> &b)
{
  return
  {
    a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2], a.m[3] + b.m[3],
    a.m[4] + b.m[4], a.m[5] + b.m[5], a.m[6] + b.m[6], a.m[7] + b.m[7],
    a.m[8] + b.m[8], a.m[9] + b.m[9], a.m[10] + b.m[10], a.m[11] + b.m[11],
    a.m[12] + b.m[12], a.m[13] + b.m[13], a.m[14] + b.m[14], a.m[15] + b.m[15]
  };
};

template <typename T> lorMatrix4<T> operator -(const lorMatrix4<T> &a, const lorMatrix4<T> &b)
{
  return
  {
    a.m[0] - b.m[0], a.m[1] - b.m[1], a.m[2] - b.m[2], a.m[3] - b.m[3],
    a.m[4] - b.m[4], a.m[5] - b.m[5], a.m[6] - b.m[6], a.m[7] - b.m[7],
    a.m[8] - b.m[8], a.m[9] - b.m[9], a.m[10] - b.m[10], a.m[11] - b.m[11],
    a.m[12] - b.m[12], a.m[13] - b.m[13], a.m[14] - b.m[14], a.m[15] - b.m[15]
  };
};

template <typename T, typename U> lorMatrix4<T> operator *(const lorMatrix4<T> &a, const U &b)
{
  return
  {
    T(a.m[0] * b), T(a.m[1] * b), T(a.m[2] * b), T(a.m[3] * b),
    T(a.m[4] * b), T(a.m[5] * b), T(a.m[6] * b), T(a.m[7] * b),
    T(a.m[8] * b), T(a.m[9] * b), T(a.m[10] * b), T(a.m[11] * b),
    T(a.m[12] * b), T(a.m[13] * b), T(a.m[14] * b), T(a.m[15] * b)
  };
};

template <typename T, typename U> lorMatrix4<T> operator /(const lorMatrix4<T> &a, const U &b)
{
  return
  {
    T(a.m[0] / b), T(a.m[1] / b), T(a.m[2] / b), T(a.m[3] / b),
    T(a.m[4] / b), T(a.m[5] / b), T(a.m[6] / b), T(a.m[7] / b),
    T(a.m[8] / b), T(a.m[9] / b), T(a.m[10] / b), T(a.m[11] / b),
    T(a.m[12] / b), T(a.m[13] / b), T(a.m[14] / b), T(a.m[15] / b)
  };
};

template <typename T> lorMatrix4<T> operator *(const lorMatrix4<T> &a, const lorMatrix4<T> &b)
{
  return
  {
    b.m[0] * a.m[0] + b.m[1] * a.m[4] + b.m[2] * a.m[8] + b.m[3] * a.m[12],
    b.m[0] * a.m[1] + b.m[1] * a.m[5] + b.m[2] * a.m[9] + b.m[3] * a.m[13],
    b.m[0] * a.m[2] + b.m[1] * a.m[6] + b.m[2] * a.m[10] +b.m[3] * a.m[14],
    b.m[0] * a.m[3] + b.m[1] * a.m[7] + b.m[2] * a.m[11] +b.m[3] * a.m[15],
    b.m[4] * a.m[0] + b.m[5] * a.m[4] + b.m[6] * a.m[8] + b.m[7] * a.m[12],
    b.m[4] * a.m[1] + b.m[5] * a.m[5] + b.m[6] * a.m[9] + b.m[7] * a.m[13],
    b.m[4] * a.m[2] + b.m[5] * a.m[6] + b.m[6] * a.m[10] +b.m[7] * a.m[14],
    b.m[4] * a.m[3] + b.m[5] * a.m[7] + b.m[6] * a.m[11] +b.m[7] * a.m[15],
    b.m[8] * a.m[0] + b.m[9] * a.m[4] + b.m[10] *a.m[8] + b.m[11] *a.m[12],
    b.m[8] * a.m[1] + b.m[9] * a.m[5] + b.m[10] *a.m[9] + b.m[11] *a.m[13],
    b.m[8] * a.m[2] + b.m[9] * a.m[6] + b.m[10] *a.m[10] +b.m[11] *a.m[14],
    b.m[8] * a.m[3] + b.m[9] * a.m[7] + b.m[10] *a.m[11] +b.m[11] *a.m[15],
    b.m[12] *a.m[0] + b.m[13] *a.m[4] + b.m[14] *a.m[8] + b.m[15] *a.m[12],
    b.m[12] *a.m[1] + b.m[13] *a.m[5] + b.m[14] *a.m[9] + b.m[15] *a.m[13],
    b.m[12] *a.m[2] + b.m[13] *a.m[6] + b.m[14] *a.m[10] +b.m[15] *a.m[14],
    b.m[12] *a.m[3] + b.m[13] *a.m[7] + b.m[14] *a.m[11] +b.m[15] *a.m[15]
  };
};

template <typename T> lorVector4<T> operator *(const lorMatrix4<T> &a, const lorVector4<T> &b)
{
  return
  {
    a.m[0] * b.x + a.m[4] * b.y + a.m[8] * b.z + a.m[12] * b.w,
    a.m[1] * b.x + a.m[5] * b.y + a.m[9] * b.z + a.m[13] * b.w,
    a.m[2] * b.x + a.m[6] * b.y + a.m[10] * b.z + a.m[14] * b.w,
    a.m[3] * b.x + a.m[7] * b.y + a.m[11] * b.z + a.m[15] * b.w,
  };
};

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////
typedef lorVector2<int32_t> lorVec2i;
typedef lorVector3<int32_t> lorVec3i;
typedef lorVector4<int32_t> lorVec4i;

typedef lorVector2<float> lorVec2;
typedef lorVector3<float> lorVec3;
typedef lorVector4<float> lorVec4;

typedef lorMatrix4<float> lorMat4;

///////////////////////////////////////////////////////////////////////////////
// Misc Math Functions
///////////////////////////////////////////////////////////////////////////////
template<typename T> inline T lorMin(T val0, T val1)
{
  return (val0 < val1 ? val0 : val1);
}

template<typename T> inline T lorMax(T val0, T val1)
{
  return (val0 > val1 ? val0 : val1);
}

template<typename T> inline T lorClamp(T val, T min, T max)
{
  return (val < min ? min : (val > max ? max : val));
}

inline double lorSqrt(double val)
{
  return sqrt(val);
}

inline float lorSqrt(float val)
{
  return sqrtf(val);
}

inline int lorSqrt(int val)
{
  return (int)sqrt((double)val);
}

inline float lorSin(float val)
{
  return sinf(val);
}

inline double lorSin(double val)
{
  return sin(val);
}

inline float lorCos(float val)
{
  return cosf(val);
}

inline double lorCos(double val)
{
  return cos(val);
}

inline int lorPow(int val, int exp)
{
  return (int)powf((float)val, (float)exp);
}

inline float lorPow(float val, float exp)
{
  return powf(val, exp);
}

inline double lorPow(double val, double exp)
{
  return pow(val, exp);
}

inline float lorAbs(float val)
{
  return fabsf(val);
}

inline double lorAbs(double val)
{
  return fabs(val);
}

template<typename T> inline T lorAbs(T val)
{
  T mask = (val >> (sizeof(T) * 8 - 1));
  return (val + mask) ^ mask;
}

#endif //LOR_MATH
