#include "lorTests.h"
#include "lorShapes.h"

TEST(BasicShapeTests, CircleVsCircleInside)
{
  lorCircle<float> a = { 1.f, 3.f, 1.f };
  lorCircle<float> b = { 1.f, 3.f, 0.5f };

  EXPECT_TRUE(lorShapes_CircleVsCircle(a, b));
}
