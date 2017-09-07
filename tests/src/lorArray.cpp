#include "lorTests.h"
#include "lorArray.h"

TEST(Array, BasicTest)
{
  lorArray<uint32_t, 64> testArray;
  EXPECT_TRUE(testArray.Init());
  EXPECT_EQ(0, testArray.length);

  *testArray.PushBack() = 82;
  EXPECT_EQ(1, testArray.length);
  EXPECT_EQ(82, testArray[0]);

  *testArray.PushBack() = 41;
  EXPECT_EQ(2, testArray.length);

  EXPECT_TRUE(testArray.PopFront());
  EXPECT_EQ(1, testArray.length);
  EXPECT_EQ(41, testArray[0]);

  testArray.Deinit();
}

TEST(Array, CopyFromTest)
{
  uint32_t values[150];
  for (uint32_t i = 0; i < 150; ++i)
    values[i] = i;

  // Copy into empty array
  lorArray<uint32_t, 64> testArray;
  EXPECT_TRUE(testArray.Init());
  EXPECT_EQ(0, testArray.length);

  testArray.CopyFrom(values, 0, 20);
  EXPECT_EQ(20, testArray.length);

  for (uint32_t i = 0; i < 20; ++i)
    EXPECT_EQ(i, testArray[i]);

  // Copy into non-empty array
  testArray.CopyFrom(values, 0, 20);
  EXPECT_EQ(40, testArray.length);

  for (uint32_t i = 0; i < 20; ++i)
    EXPECT_EQ(i, testArray[20 + i]);

  // Copy into non-empty array, over block boundary
  testArray.CopyFrom(values, 0, 80);
  EXPECT_EQ(120, testArray.length);

  for (uint32_t i = 0; i < 80; ++i)
    EXPECT_EQ(i, testArray[40 + i]);

  // Copy into non-empty array, over multiple block boundaries
  testArray.CopyFrom(values, 0, 150);
  EXPECT_EQ(270, testArray.length);

  for (uint32_t i = 0; i < 150; ++i)
    EXPECT_EQ(i, testArray[120 + i]);

  // Copy into non-empty array, over multiple block boundaries, with an offset
  EXPECT_TRUE(testArray.PopFront());
  EXPECT_EQ(269, testArray.length);
  testArray.CopyFrom(values, 0, 150);
  EXPECT_EQ(419, testArray.length);

  for (uint32_t i = 0; i < 150; ++i)
    EXPECT_EQ(i, testArray[269 + i]);

  testArray.Deinit();
}

TEST(Array, CopyToTest)
{
  uint32_t values[150];
  for (uint32_t i = 0; i < 150; ++i)
    values[i] = 0xFFFFFFFF;

  lorArray<uint32_t, 64> testArray;
  EXPECT_TRUE(testArray.Init());
  for (uint32_t i = 0; i < 150; ++i)
    *testArray.PushBack() = i;

  EXPECT_EQ(150, testArray.length);

  do
  {
    // Copy into array
    testArray.CopyTo(0, 20, values);
    for (uint32_t i = 0; i < 20; ++i)
      EXPECT_EQ(i, values[i]);

    // Copy into array, across block boundary
    testArray.CopyTo(60, 20, values);
    for (uint32_t i = 0; i < 20; ++i)
      EXPECT_EQ(60 + i, values[i]);

    // Copy into array, across multiple block boundaries
    testArray.CopyTo(0, 150, values);
    for (uint32_t i = 0; i < 150; ++i)
      EXPECT_EQ(i, values[i]);

    // Do it again, with an offset in lorArray
    testArray.PopFront();
    testArray.PushBack();
    for (uint32_t i = 0; i < 150; ++i)
      testArray[i] = i;
  } while (testArray.offset == 1);

  testArray.Deinit();
}
