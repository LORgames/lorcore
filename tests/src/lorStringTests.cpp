#include "lorTests.h"
#include "lorString.h"

TEST(lorString, BasicTests)
{
  const char helloStr[] = "Hello~!";
  const char extraStr[] = "Here for your coconuts?";
  const char fullStr[] = "Hello~! Here for your coconuts?";
  char buffer[64];

  EXPECT_EQ(7, lorStrlen(helloStr));
  EXPECT_EQ(helloStr + 4, lorStrchr(helloStr, 'o'));

  lorStrcpy(buffer, sizeof(buffer), helloStr);
  EXPECT_TRUE(lorStrEquals(helloStr, buffer));
  EXPECT_EQ(0, lorStrcmp(helloStr, buffer));

  lorStrcat(buffer, sizeof(buffer), " ");
  lorStrcat(buffer, sizeof(buffer), extraStr);
  EXPECT_TRUE(lorStrEquals(buffer, fullStr));
  EXPECT_STRCASEEQ(fullStr, buffer);

  memset(buffer, 0, sizeof(buffer));
  EXPECT_EQ(lorStrlen(fullStr), lorSprintf(buffer, sizeof(buffer), "%s %s", helloStr, extraStr));
  EXPECT_TRUE(lorStrEquals(buffer, fullStr));
  EXPECT_STRCASEEQ(fullStr, buffer);

  const char *pCopy = lorStrdup(fullStr);
  EXPECT_NE(pCopy, fullStr); //Should be a different address
  EXPECT_STRCASEEQ(fullStr, pCopy);
  lorFree(pCopy);
}

TEST(lorString, AtoITests)
{
  //Base10
  EXPECT_EQ(0, lorStrAtoI(nullptr));
  EXPECT_EQ(0, lorStrAtoI("Apples!"));
  EXPECT_EQ(-1, lorStrAtoI("-1"));
  EXPECT_EQ(0, lorStrAtoI("-A"));
  EXPECT_EQ(7, lorStrAtoI("7"));
  EXPECT_EQ(0, lorStrAtoI("A"));
  EXPECT_EQ(0, lorStrAtoI("F"));
  EXPECT_EQ(0, lorStrAtoI("G"));
  EXPECT_EQ(10111011, lorStrAtoI("10111011"));

  //Hexadecimal
  EXPECT_EQ(0, lorStrAtoI(nullptr, 16));
  EXPECT_EQ(10, lorStrAtoI("Apples!", 16));
  EXPECT_EQ(-1, lorStrAtoI("-1", 16));
  EXPECT_EQ(-10, lorStrAtoI("-A", 16));
  EXPECT_EQ(7, lorStrAtoI("7", 16));
  EXPECT_EQ(10, lorStrAtoI("A", 16));
  EXPECT_EQ(15, lorStrAtoI("F", 16));
  EXPECT_EQ(0, lorStrAtoI("G", 16));
  EXPECT_EQ(269553681, lorStrAtoI("10111011", 16));

  //Binary
  EXPECT_EQ(0, lorStrAtoI(nullptr, 2));
  EXPECT_EQ(0, lorStrAtoI("Apples!", 2));
  EXPECT_EQ(0, lorStrAtoI("7", 2));
  EXPECT_EQ(0, lorStrAtoI("A", 2));
  EXPECT_EQ(0, lorStrAtoI("A", 2));
  EXPECT_EQ(187, lorStrAtoI("10111011", 2));
  EXPECT_EQ(-7, lorStrAtoI("-111", 2));
}

//inline int lorStrAtoI(const char *pStr, int radix = 10, bool *pSuccess = nullptr)
