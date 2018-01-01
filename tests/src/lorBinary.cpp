#include "lorTests.h"
#include "lorBinary.h"

TEST(lorBinary, BasicTest)
{
  lorBinary *pBinary;
  lorBinary_Init(&pBinary);

  const char *pFixedLenString = "This is my fixed length, constant, string.";
  const char *pPascalString = "Solid pascal char array!";
  const char *pNullTerminatedString = "你好。。。明天见";

  uint32_t fixedLenStrLen = (uint32_t)(lorStrlen(pFixedLenString) + 1); //+1 for null terminator

  const int8_t expectedInt8 = -7;
  const int16_t expectedInt16 = -25934;
  const int32_t expectedInt32 = 0xDEADBEEF;
  const int64_t expectedInt64 = 0xCEDEDC0DEDEC0DED;
  const uint8_t expectedUint8 = 0x43;
  const uint16_t expectedUint16 = 0xCAFE;
  const uint32_t expectedUint32 = 0xBADC0C0A;
  const uint64_t expectedUint64 = 0xFEEDF00D0B0EBABE;

  //Write to the buffer
  lorBinary_WriteInt8(pBinary, expectedInt8);
  lorBinary_WriteInt16(pBinary, expectedInt16);
  lorBinary_WriteInt32(pBinary, expectedInt32);
  lorBinary_WriteInt64(pBinary, expectedInt64);
  lorBinary_WriteUint8(pBinary, expectedUint8);
  lorBinary_WriteUint16(pBinary, expectedUint16);
  lorBinary_WriteUint32(pBinary, expectedUint32);
  lorBinary_WriteUint64(pBinary, expectedUint64);

  lorBinary_WriteStringFixedLength(pBinary, pFixedLenString, fixedLenStrLen);
  lorBinary_WriteStringPascal16(pBinary, pPascalString);
  lorBinary_WriteStringNullTerminated(pBinary, pNullTerminatedString);

  //Read the buffer back
  lorBinary_SetPosition(pBinary, 0);

  EXPECT_EQ(expectedInt8, lorBinary_ReadInt8(pBinary));
  EXPECT_EQ(expectedInt16, lorBinary_ReadInt16(pBinary));
  EXPECT_EQ(expectedInt32, lorBinary_ReadInt32(pBinary));
  EXPECT_EQ(expectedInt64, lorBinary_ReadInt64(pBinary));
  EXPECT_EQ(expectedUint8, lorBinary_ReadUint8(pBinary));
  EXPECT_EQ(expectedUint16, lorBinary_ReadUint16(pBinary));
  EXPECT_EQ(expectedUint32, lorBinary_ReadUint32(pBinary));
  EXPECT_EQ(expectedUint64, lorBinary_ReadUint64(pBinary));

  uint32_t caratPosition = lorBinary_GetPosition(pBinary);

  //Try pointer versions
  const char *pFixedLenStringOut = lorBinary_ReadStringFixedLength(pBinary, fixedLenStrLen);
  const char *pPascalStringOut = lorBinary_ReadStringPascal16(pBinary);
  const char *pNullTerminatedStringOut = lorBinary_ReadStringNullTerminated(pBinary);

  EXPECT_STREQ(pFixedLenString, pFixedLenStringOut);
  EXPECT_STREQ(pPascalString, pPascalStringOut);
  EXPECT_STREQ(pNullTerminatedString, pNullTerminatedStringOut);

  lorFree(pFixedLenStringOut);
  lorFree(pPascalStringOut);
  lorFree(pNullTerminatedStringOut);

  //Try buffer versions
  lorBinary_SetPosition(pBinary, caratPosition);
  char buffer[128];

  EXPECT_TRUE(lorBinary_ReadStringFixedLength(pBinary, buffer, fixedLenStrLen));
  EXPECT_STREQ(pFixedLenString, buffer);

  EXPECT_TRUE(lorBinary_ReadStringPascal16(pBinary, buffer, 128));
  EXPECT_STREQ(pPascalString, buffer);

  EXPECT_TRUE(lorBinary_ReadStringNullTerminated(pBinary, buffer, 128));
  EXPECT_STREQ(pNullTerminatedString, buffer);

  EXPECT_TRUE(lorBinary_IsPositionAtEnd(pBinary));

  lorBinary_Deinit(&pBinary);
}
