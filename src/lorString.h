#ifndef LOR_STRING
#define LOR_STRING

#include "lorMemory.h"
#include "lorPlatform.h"
#include <stdarg.h>

const char radixAlphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline size_t lorStrlen(const char *pStr)
{
  if (pStr == nullptr)
    return 0;

  size_t len = 0;
  while (pStr[len] != '\0')
    ++len;
  return len;
}

inline const char* lorStrchr(const char *pStr, int c) //TODO: Fix to work with UTF8
{
  if (pStr == nullptr || c == 0)
    return nullptr;

  while (*pStr != '\0')
  {
    if (*pStr == c)
      return pStr;

    ++pStr;
  }

  return nullptr;
}

inline char* lorStrdup(const char *pStr)
{
  if (pStr == nullptr)
    return nullptr;

  size_t strLen = lorStrlen(pStr) + 1; //+1 for \0
  char *pNewStr = (char*)lorAlloc(strLen);
  memcpy(pNewStr, pStr, strLen);

  return pNewStr;
}

inline void lorStrcpy(char *strDestination, size_t destinationSize, const char *strSource)
{
  size_t i = 0;

  if (destinationSize == 0)
    return; // Maybe we should lorStrlen here?

  while (i < destinationSize && strSource[i] != '\0')
  {
    strDestination[i] = strSource[i];
    ++i;
  }

  strDestination[i] = '\0';
}

inline void lorStrcat(char *strDestination, size_t destinationSize, const char *strSource)
{
  size_t writePos = strlen(strDestination);
  size_t readPos = 0;

  if (writePos + 1 >= destinationSize)
    return;

  while (writePos != (destinationSize - 1) && strSource[readPos] != '\0')
  {
    strDestination[writePos] = strSource[readPos];
    ++writePos;
    ++readPos;
  }

  strDestination[writePos] = '\0';
}

inline int lorStrcmp(const char *pStrA, const char *pStrB)
{
  const char *pA = (pStrA == nullptr) ? "" : pStrA;
  const char *pB = (pStrB == nullptr) ? "" : pStrB;

  int result = 0;

  while (!result && *pA++ && *pB++)
    result = *pA - *pB;

  return result;
}

inline bool lorStrEquals(const char *pStrA, const char *pStrB)
{
  const char *pA = (pStrA == nullptr) ? "" : pStrA;
  const char *pB = (pStrB == nullptr) ? "" : pStrB;

  bool result = true;

  while (result && *pA++ && *pB++)
    result = (*pA == *pB);

  return result;
}

inline int lorStrAtoI(const char *pStr, int radix = 10)
{
  if (pStr == nullptr || radix < 2 || radix > sizeof(radixAlphabet))
    return 0;

  while (*pStr == ' ' || *pStr == '\t' || *pStr == '\n' || *pStr == '\r') //Ignore whitespace at the start
    ++pStr;

  int retVal = 0;
  int multiply = 1;
  if (*pStr == '-')
  {
    ++pStr;
    multiply = -1;
  }

  while (*pStr != '\0')
  {
    const char *pChr = lorStrchr(radixAlphabet, *pStr);
    int distance = (int)(pChr - radixAlphabet);

    if (pChr == nullptr || distance >= radix)
      break;

    retVal = retVal * radix + distance;
    ++pStr;
  }

  return retVal * multiply;
}

inline int lorSprintf(char *buffer, size_t bufferLen, const char *format, ...)
{
  va_list args;
  va_start(args, format);
#if defined(_MSC_VER)
  int retVal = vsnprintf_s(buffer, bufferLen, _TRUNCATE, format, args);
#else
  int retVal = vsnprintf(buffer, bufferLen, format, args);
#endif
  va_end(args);

  return retVal;
}

//UTF8 decoding
const int lorStringUTF8_End = -1;
const int lorStringUTF8_Error = -2;

struct lorStringUTF8Context
{
  const char *pStr;
  size_t strLength;
  int currentIndex;
};

// Initialize the UTF-8 decoder. The decoder is not reentrant
void lorStringUTF_StartDecode(lorStringUTF8Context *pContext, const char *pStr);

// Extract the next character. Can return lorStringUTF8_End or lorStringUTF8_Error
int lorStringUTF_DecodeNextCharacter(lorStringUTF8Context *pContext);

#endif //LOR_STRING
