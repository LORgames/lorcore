#ifndef LOR_MATH
#define LOR_MATH

#include <stdint.h>
#include <inttypes.h>
#include <math.h> // This provides sinf and is required for Linux (how does everything else get this?)

#include "lorCore.h"

#define lorPI 3.14159f

template <typename T>
struct lorPointTemplate
{
  T x;
  T y;
};

template <typename T>
struct lorCircleTemplate
{
  T x;
  T y;
  T r;
};

template <typename T>
struct lorRectangleTemplate {
  T x;
  T y;
  T w;
  T h;
};

inline double lorSqrt(double val) { return sqrt(val); }
inline float lorSqrt(float val) { return sqrtf(val); }
inline int lorSqrt(int val) { return (int)sqrt((double)val); }

inline float lorSin(float val) { return sinf(val); }
inline double lorSin(double val) { return sin(val); }

inline float lorCos(float val) { return cosf(val); }
inline double lorCos(double val) { return cos(val); }

inline float lorAbs(float val) { return fabsf(val); }
inline double lorAbs(double val) { return fabs(val); }
inline int lorAbs(int val) { int mask = (val >> (sizeof(int) * 8 - 1)); return (val + mask) ^ mask; } //8bits per byte

template<typename T> inline T lorMin(T val0, T val1) { return (val0 < val1 ? val0 : val1); }
template<typename T> inline T lorMax(T val0, T val1) { return (val0 > val1 ? val0 : val1); }
template<typename T> inline T lorClamp(T val, T min, T max) { return (val < min ? min : (val > max ? max : val)); }

typedef lorRectangleTemplate<float> lorRect;
typedef lorPointTemplate<float> lorPoint;

typedef lorPointTemplate<int32_t> lorCollisionPoint;
typedef lorRectangleTemplate<int32_t> lorCollisionRect;
typedef lorCircleTemplate<int32_t> lorCollisionCircle;

#endif //LOR_MATH
