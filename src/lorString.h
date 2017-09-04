#ifndef LOR_STRING
#define LOR_STRING

#include "lorPlatform.h"
#include <stdarg.h>

inline size_t lorStrlen(const char *string)
{
  size_t len = 0;
  while (string[len] != '\0')
    ++len;
  return len;
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

inline int lorStrcmp(const char *strA, const char *strB)
{
  const char *A, *B;

  if (strB == nullptr)
    B = "";
  else
    B = strB;

  if (strA == nullptr)
    A = "";
  else
    A = strA;

  int result = 0;

  while (!result && *A++ && *B++)
  {
    result = *A - *B;
  }

  return result;
}

inline void lorSprintf(char *buffer, size_t bufferLen, const char *format, ...)
{
  va_list args;
  va_start(args, format);
#if defined(_MSC_VER)
  vsnprintf_s(buffer, bufferLen, _TRUNCATE, format, args);
#else
  vsnprintf(buffer, bufferLen, format, args);
#endif
  va_end(args);
}

#endif //LOR_STRING
