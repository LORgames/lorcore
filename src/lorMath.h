#ifndef LOR_MATH
#define LOR_MATH

#include <stdint.h>
#include <inttypes.h>
#include <math.h> // This provides sinf and is required for Linux (how does everything else get this?)

#include "lorCore.h"

#define lorPI 3.14159f

// Vectors
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

//Arithmetic
template <typename T> lorVector2<T> operator +(const lorVector2<T> &a) { return { a.x, a.y }; };
template <typename T> lorVector2<T> operator -(const lorVector2<T> &a) { return { -a.x, -a.y }; };
template <typename T> lorVector2<T> operator +(const lorVector2<T> &a, const lorVector2<T> &b) { return{ a.x + b.x, a.y + b.y }; };
template <typename T> lorVector2<T> operator -(const lorVector2<T> &a, const lorVector2<T> &b) { return{ a.x - b.x, a.y - b.y }; };
template <typename T, typename U> lorVector2<T> operator *(const lorVector2<T> &a, const U &b) { return { T(a.x * b), T(a.y * b) }; };
template <typename T, typename U> lorVector2<T> operator /(const lorVector2<T> &a, const U &b) { return{ T(a.x / b), T(a.y / b) }; };
template <typename T> lorVector2<T> operator *(const lorVector2<T> &a, const lorVector2<T> &b) { return { a.x * b.x, a.y * b.y }; };
template <typename T> lorVector2<T> operator /(const lorVector2<T> &a, const lorVector2<T> &b) { return { a.x / b.x, a.y / b.y }; };

template <typename T> struct lorVector3 { T x; T y; T z;  };
template <typename T> struct lorVector4 { T x; T y; T z; T w; };

typedef lorVector2<int32_t> lorVec2i;
typedef lorVector3<int32_t> lorVec3i;
typedef lorVector4<int32_t> lorVec4i;

typedef lorVector2<float> lorVec2;
typedef lorVector3<float> lorVec3;
typedef lorVector4<float> lorVec4;

//Matrices
template <typename T>
struct lorMatrix4
{
  union
  {
    T m[16];
    lorVector4 a[4];
  };
};

//Functions
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

inline float lorAbs(float val) { return fabsf(val); }
inline double lorAbs(double val) { return fabs(val); }
template<typename T> inline T lorAbs(T val) { T mask = (val >> (sizeof(T) * 8 - 1)); return (val + mask) ^ mask; }

#endif //LOR_MATH
