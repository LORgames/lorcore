#ifndef LOR_SHAPES_H__
#define LOR_SHAPES_H__

#include "lorMath.h"

//Shapes
template <typename T> struct lorCircle { T x; T y; T r; };
template <typename T> struct lorRectangle { T x; T y; T w; T h; };

typedef lorRectangle<float> lorRect;
typedef lorRectangle<int32_t> lorRectanglei;
typedef lorCircle<int32_t> lorCirclei;

template <typename T>
bool lorShapes_CircleVsCircle(const lorCircle<T> &pt0, const lorCircle<T> &pt1)
{
  T x = pt0.x - pt1.x;
  T y = pt0.y - pt1.y;

  return (x*x + y*y <= (pt0.r + pt1.r)*(pt0.r + pt1.r));
}

template <typename T>
bool lorShapes_CircleVsRectangle(const lorCircle<T> &circle, const lorRectangle<T> &rectangle, T *pOutDist = nullptr, lorVector2<T> *pOutPoint = nullptr)
{
  //copy the circle center to a new point (this changes so there needs to be a copy)
  lorVector2<T> pt = { circle.x, circle.y };

  //point on rectangle closest to circle
  if (pt.x > rectangle.x + rectangle.w)
    pt.x = rectangle.x + rectangle.w;
  if (pt.x < rectangle.x)
    pt.x = rectangle.x;
  if (pt.y > rectangle.y + rectangle.h)
    pt.y = rectangle.y + rectangle.h;
  if (pt.y < rectangle.y)
    pt.y = rectangle.y;

  //distance check, just use distance^2 for actual implementation
  T dx = pt.x - circle.x;
  T dy = pt.y - circle.y;

  T distSq = dx * dx + dy * dy;

  if (pOutDist != nullptr)
    *pOutDist = lorSqrt(distSq);

  if (pOutPoint != nullptr)
    *pOutPoint = { pt.x, pt.y };

  if (distSq < circle.r*circle.r)
    return true;

  return false;
}

#endif
