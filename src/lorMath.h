#ifndef LOR_MATH
#define LOR_MATH

#include <stdint.h>
#include <inttypes.h>
#include <math.h> // This provides sinf and is required for Linux (how does everything else get this?)

#include "lorCore.h"

#define lorPI 3.14159f

///////////////////////////////////////////////////////////////////////////////
// Vector2
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector2
{
  T x;
  T y;

  static lorVector2<T> Create(T x, T y) { return{ x, y }; };
  static lorVector2<T> Zero() { return { T(0), T(0) }; };
  static lorVector2<T> One() { return { T(1), T(1) }; };
};

// Comparisons
template <typename T> bool operator ==(const lorVector2<T> &a, const lorVector2<T> &b) { return ((a.x == b.x) && (a.y == b.y)); };
template <typename T> bool operator !=(const lorVector2<T> &a, const lorVector2<T> &b) { return !(a == b); };

// Arithmetic
template <typename T> lorVector2<T> operator +(const lorVector2<T> &a) { return { a.x, a.y }; };
template <typename T> lorVector2<T> operator -(const lorVector2<T> &a) { return { -a.x, -a.y }; };
template <typename T> lorVector2<T> operator +(const lorVector2<T> &a, const lorVector2<T> &b) { return{ a.x + b.x, a.y + b.y }; };
template <typename T> lorVector2<T> operator -(const lorVector2<T> &a, const lorVector2<T> &b) { return{ a.x - b.x, a.y - b.y }; };
template <typename T> lorVector2<T> operator *(const lorVector2<T> &a, const lorVector2<T> &b) { return { a.x * b.x, a.y * b.y }; };
template <typename T> lorVector2<T> operator /(const lorVector2<T> &a, const lorVector2<T> &b) { return { a.x / b.x, a.y / b.y }; };
template <typename T, typename U> lorVector2<T> operator *(const lorVector2<T> &a, const U &b) { return{ T(a.x * b), T(a.y * b) }; };
template <typename T, typename U> lorVector2<T> operator /(const lorVector2<T> &a, const U &b) { return{ T(a.x / b), T(a.y / b) }; };

//Functions
template <typename T> T lorMag(const lorVector2<T> &a) { return lorSqrt(a.x*a.x + a.y*a.y); };
template <typename T> T lorDot(const lorVector2<T> &a, const lorVector2<T> &b) { return (a.x * b.x + a.y * b.y); };
template <typename T> lorVector2<T> lorNormalize(const lorVector2<T> &a) { T mag = lorMag(a); return{ a.x / mag, a.y / mag }; };

///////////////////////////////////////////////////////////////////////////////
// Vector3
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector3
{
  T x;
  T y;
  T z;

  static lorVector3<T> Create(T x, T y, T z) { return { x, y, z }; };
  static lorVector3<T> Create(lorVector2<T> o, T z) { return{ o.x, o.y, z }; };
  static lorVector3<T> Zero() { return { T(0), T(0), T(0) }; };
  static lorVector3<T> One() { return { T(1), T(1), T(1) }; };

  static lorVector3<T> WorldForward() { return{ T(0), T(1), T(0) }; };
  static lorVector3<T> WorldBackward() { return{ T(0), -T(1), T(0) }; };
  static lorVector3<T> WorldUp() { return{ T(0), T(0), T(1) }; };
  static lorVector3<T> WorldDown() { return{ T(0), T(0), -T(1) }; };
  static lorVector3<T> WorldLeft() { return{ -T(1), T(0), T(0) }; };
  static lorVector3<T> WorldRight() { return{ T(1), T(0), T(0) }; };
};

// Comparisons
template <typename T> bool operator ==(const lorVector3<T> &a, const lorVector3<T> &b) { return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z)); };
template <typename T> bool operator !=(const lorVector3<T> &a, const lorVector3<T> &b) { return !(a == b); };

// Arithmetic
template <typename T> lorVector3<T> operator +(const lorVector3<T> &a) { return{ a.x, a.y, a.z }; };
template <typename T> lorVector3<T> operator -(const lorVector3<T> &a) { return{ -a.x, -a.y, -a.z }; };
template <typename T> lorVector3<T> operator +(const lorVector3<T> &a, const lorVector3<T> &b) { return{ a.x + b.x, a.y + b.y, a.z + b.z }; };
template <typename T> lorVector3<T> operator -(const lorVector3<T> &a, const lorVector3<T> &b) { return{ a.x - b.x, a.y - b.y, a.z - b.z }; };
template <typename T> lorVector3<T> operator *(const lorVector3<T> &a, const lorVector3<T> &b) { return{ a.x * b.x, a.y * b.y, a.z * b.z }; };
template <typename T> lorVector3<T> operator /(const lorVector3<T> &a, const lorVector3<T> &b) { return{ a.x / b.x, a.y / b.y, a.z / b.z }; };
template <typename T, typename U> lorVector3<T> operator *(const lorVector3<T> &a, const U &b) { return{ T(a.x * b), T(a.y * b), T(a.z * b) }; };
template <typename T, typename U> lorVector3<T> operator /(const lorVector3<T> &a, const U &b) { return{ T(a.x / b), T(a.y / b), T(a.z / b) }; };

//Functions
template <typename T> T lorMag(const lorVector3<T> &a) { return lorSqrt(a.x*a.x + a.y*a.y + a.z*a.z); };
template <typename T> T lorDot(const lorVector3<T> &a, const lorVector3<T> &b) { return (a.x * b.x + a.y * b.y + a.z * b.z); };
template <typename T> lorVector3<T> lorCross(const lorVector3<T> &a, const lorVector3<T> &b) { return{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; };
template <typename T> lorVector3<T> lorNormalize(const lorVector3<T> &a) { T mag = lorMag(a); return{ a.x / mag, a.y / mag, a.z / mag }; };

///////////////////////////////////////////////////////////////////////////////
// Vector4
///////////////////////////////////////////////////////////////////////////////
template <typename T> struct lorVector4
{
  T x;
  T y;
  T z;
  T w;

  static lorVector4<T> Create(T x, T y, T z, T w) { return{ x, y, z, w }; };
  static lorVector4<T> Create(lorVector2<T> o, T z, T w) { return{ o.x, o.y, z, w }; };
  static lorVector4<T> Create(lorVector3<T> o, T w) { return{ o.x, o.y, o.z, w }; };
  static lorVector4<T> Zero() { return{ T(0), T(0), T(0), T(0) }; };
  static lorVector4<T> One() { return{ T(1), T(1), T(1), T(1) }; };
};

// Comparisons
template <typename T> bool operator ==(const lorVector4<T> &a, const lorVector4<T> &b) { return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)); };
template <typename T> bool operator !=(const lorVector4<T> &a, const lorVector4<T> &b) { return !(a == b); };

// Arithmetic
template <typename T> lorVector4<T> operator +(const lorVector4<T> &a) { return{ a.x, a.y, a.z, a.w }; };
template <typename T> lorVector4<T> operator -(const lorVector4<T> &a) { return{ -a.x, -a.y, -a.z, -a.w }; };
template <typename T> lorVector4<T> operator +(const lorVector4<T> &a, const lorVector4<T> &b) { return{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; };
template <typename T> lorVector4<T> operator -(const lorVector4<T> &a, const lorVector4<T> &b) { return{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; };
template <typename T> lorVector4<T> operator *(const lorVector4<T> &a, const lorVector4<T> &b) { return{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; };
template <typename T> lorVector4<T> operator /(const lorVector4<T> &a, const lorVector4<T> &b) { return{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; };
template <typename T, typename U> lorVector4<T> operator *(const lorVector4<T> &a, const U &b) { return{ T(a.x * b), T(a.y * b), T(a.z * b), T(a.z * b) }; };
template <typename T, typename U> lorVector4<T> operator /(const lorVector4<T> &a, const U &b) { return{ T(a.x / b), T(a.y / b), T(a.z / b), T(a.z / b) }; };

//Functions
template <typename T> T lorMag(const lorVector4<T> &a) { return lorSqrt(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w); };
template <typename T> T lorDot(const lorVector4<T> &a, const lorVector4<T> &b) { return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w); };
template <typename T> lorVector4<T> lorNormalize(const lorVector4<T> &a) { T mag = lorMag(a); return{ a.x / mag, a.y / mag, a.z / mag, a.w / mag }; };

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

  static lorMatrix4<T> Identity() { return{ T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1) }; };
  static lorMatrix4<T> Zero() { return{ T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }; };

  static lorMatrix4<T> PerspectiveFov(T fov, T width, T height, T zNear, T zFar)
  {
    if (width <= T(0) || height <= T(0) || fov <= T(0))
      return Identity();

    T const h = lorCos(T(0.5) * fov) / lorSin(T(0.5) * fov);
    T const w = h * height / width; //Aspect ratio

    return { w, T(0), T(0), T(0),
             T(0), T(0), (zFar + zNear) / (zFar - zNear), T(1),
             T(0), h, T(0), T(0),
             T(0), T(0), -(T(2) * zFar * zNear) / (zFar - zNear), T(0) };
  };

  static lorMatrix4<T> LookAt(lorVector3<T> eye, lorVector3<T> center, lorVector3<T> up)
  {
    lorVector3<T> f = lorNormalize(center - eye);
    lorVector3<T> s = lorNormalize(lorCross(up, f));
    lorVector3<T> u = lorCross(f, s);

    lorMatrix4<T> retVal = Identity();
    retVal.a[0].x = s.x;
    retVal.a[1].x = s.y;
    retVal.a[2].x = s.z;
    retVal.a[0].y = u.x;
    retVal.a[1].y = u.y;
    retVal.a[2].y = u.z;
    retVal.a[0].z = f.x;
    retVal.a[1].z = f.y;
    retVal.a[2].z = f.z;
    retVal.a[3].x = -lorDot(s, eye);
    retVal.a[3].y = -lorDot(u, eye);
    retVal.a[3].z = -lorDot(f, eye);
    return retVal;
  };
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
template <typename T> bool operator !=(const lorMatrix4<T> &a, const lorMatrix4<T> &b) { return !(a == b); };

// Arithmetic
template <typename T> lorMatrix4<T> operator +(const lorMatrix4<T> &a) { return { a.m[0], a.m[1], a.m[2], a.m[3], a.m[4], a.m[5], a.m[6], a.m[7],a.m[8], a.m[9], a.m[10], a.m[11], a.m[12], a.m[13], a.m[14], a.m[15] }; };
template <typename T> lorMatrix4<T> operator -(const lorMatrix4<T> &a) { return (a * -1); };
template <typename T> lorMatrix4<T> operator +(const lorMatrix4<T> &a, const lorMatrix4<T> &b) { return{ a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2], a.m[3] + b.m[3],
                                                                                                         a.m[4] + b.m[4], a.m[5] + b.m[5], a.m[6] + b.m[6], a.m[7] + b.m[7],
                                                                                                         a.m[8] + b.m[8], a.m[9] + b.m[9], a.m[10] + b.m[10], a.m[11] + b.m[11],
                                                                                                         a.m[12] + b.m[12], a.m[13] + b.m[13], a.m[14] + b.m[14], a.m[15] + b.m[15] }; };
template <typename T> lorMatrix4<T> operator -(const lorMatrix4<T> &a, const lorMatrix4<T> &b) { return{ a.m[0] - b.m[0], a.m[1] - b.m[1], a.m[2] - b.m[2], a.m[3] - b.m[3],
                                                                                                         a.m[4] - b.m[4], a.m[5] - b.m[5], a.m[6] - b.m[6], a.m[7] - b.m[7],
                                                                                                         a.m[8] - b.m[8], a.m[9] - b.m[9], a.m[10] - b.m[10], a.m[11] - b.m[11],
                                                                                                         a.m[12] - b.m[12], a.m[13] - b.m[13], a.m[14] - b.m[14], a.m[15] - b.m[15] }; };
template <typename T, typename U> lorMatrix4<T> operator *(const lorMatrix4<T> &a, const U &b) { return{ T(a.m[0] * b), T(a.m[1] * b), T(a.m[2] * b), T(a.m[3] * b),
                                                                                                         T(a.m[4] * b), T(a.m[5] * b), T(a.m[6] * b), T(a.m[7] * b),
                                                                                                         T(a.m[8] * b), T(a.m[9] * b), T(a.m[10] * b), T(a.m[11] * b),
                                                                                                         T(a.m[12] * b), T(a.m[13] * b), T(a.m[14] * b), T(a.m[15] * b) }; };
template <typename T, typename U> lorMatrix4<T> operator /(const lorMatrix4<T> &a, const U &b) { return{ T(a.m[0] / b), T(a.m[1] / b), T(a.m[2] / b), T(a.m[3] / b),
                                                                                                         T(a.m[4] / b), T(a.m[5] / b), T(a.m[6] / b), T(a.m[7] / b),
                                                                                                         T(a.m[8] / b), T(a.m[9] / b), T(a.m[10] / b), T(a.m[11] / b),
                                                                                                         T(a.m[12] / b), T(a.m[13] / b), T(a.m[14] / b), T(a.m[15] / b) }; };
template <typename T> lorMatrix4<T> operator *(const lorMatrix4<T> &a, const lorMatrix4<T> &b) { return {
  a.m[0] * b.m[0] + a.m[1] * b.m[4] + a.m[2] * b.m[8] + a.m[3] * b.m[12], a.m[0] * b.m[1] + a.m[1] * b.m[5] + a.m[2] * b.m[9] + a.m[3] * b.m[13], a.m[0] * b.m[2] + a.m[1] * b.m[6] + a.m[2] * b.m[10] + a.m[3] * b.m[14], a.m[0] * b.m[3] + a.m[1] * b.m[7] + a.m[2] * b.m[11] + a.m[3] * b.m[15],
  a.m[4] * b.m[0] + a.m[5] * b.m[4] + a.m[6] * b.m[8] + a.m[7] * b.m[12], a.m[4] * b.m[1] + a.m[5] * b.m[5] + a.m[6] * b.m[9] + a.m[7] * b.m[13], a.m[4] * b.m[2] + a.m[5] * b.m[6] + a.m[6] * b.m[10] + a.m[7] * b.m[14], a.m[4] * b.m[3] + a.m[5] * b.m[7] + a.m[6] * b.m[11] + a.m[7] * b.m[15],
  a.m[8] * b.m[0] + a.m[9] * b.m[4] + a.m[10] * b.m[8] + a.m[11] * b.m[12], a.m[8] * b.m[1] + a.m[9] * b.m[5] + a.m[10] * b.m[9] + a.m[11] * b.m[13], a.m[8] * b.m[2] + a.m[9] * b.m[6] + a.m[10] * b.m[10] + a.m[11] * b.m[14], a.m[8] * b.m[3] + a.m[9] * b.m[7] + a.m[10] * b.m[11] + a.m[11] * b.m[15],
  a.m[12] * b.m[0] + a.m[13] * b.m[4] + a.m[14] * b.m[8] + a.m[15] * b.m[12], a.m[12] * b.m[1] + a.m[13] * b.m[5] + a.m[14] * b.m[9] + a.m[15] * b.m[13], a.m[12] * b.m[2] + a.m[13] * b.m[6] + a.m[14] * b.m[10] + a.m[15] * b.m[14], a.m[12] * b.m[3] + a.m[13] * b.m[7] + a.m[14] * b.m[11] + a.m[15] * b.m[15] };
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
template<typename T> inline T lorMin(T val0, T val1) { return (val0 < val1 ? val0 : val1); }
template<typename T> inline T lorMax(T val0, T val1) { return (val0 > val1 ? val0 : val1); }
template<typename T> inline T lorClamp(T val, T min, T max) { return (val < min ? min : (val > max ? max : val)); }

inline double lorSqrt(double val) { return sqrt(val); }
inline float lorSqrt(float val) { return sqrtf(val); }
inline int lorSqrt(int val) { return (int)sqrt((double)val); }

inline float lorSin(float val) { return sinf(val); }
inline double lorSin(double val) { return sin(val); }

inline float lorCos(float val) { return cosf(val); }
inline double lorCos(double val) { return cos(val); }

inline int lorPow(int val, int exp) { return (int)powf((float)val, (float)exp); }
inline float lorPow(float val, float exp) { return powf(val, exp); }
inline double lorPow(double val, double exp) { return pow(val, exp); }

inline float lorAbs(float val) { return fabsf(val); }
inline double lorAbs(double val) { return fabs(val); }
template<typename T> inline T lorAbs(T val) { T mask = (val >> (sizeof(T) * 8 - 1)); return (val + mask) ^ mask; }

#endif //LOR_MATH
