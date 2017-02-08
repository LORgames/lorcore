#ifndef LOR_MEMORY
#define LOR_MEMORY

#include <stdio.h>
#include "lorString.h"
#include <stdlib.h>

#define TRACK_MEMORY 0

#if TRACK_MEMORY
#define MEMORY_LINEFILE , char *pFilename = "", uint32_t lineNumber = -1
#define lorAllocType(type, count) (type*)lorAlloc(sizeof(type)*count, true, __FILE__, __LINE__)
enum { MemoryPadding = 64 };
#else
#define MEMORY_LINEFILE
#define lorAllocType(type, count) (type*)lorAlloc(sizeof(type)*count)
#endif

inline void* lorAlloc(size_t numBytes, bool zero = true MEMORY_LINEFILE)
{
#if TRACK_MEMORY
  void* pRealMemory = malloc(numBytes + MemoryPadding);
  void* pMemory = ((char*)pRealMemory + MemoryPadding);
  lorSprintf((char*)pRealMemory, MemoryPadding, "%s:%d", pFilename, lineNumber);
#else
  void* pMemory = malloc(numBytes);
#endif

  if (zero)
    memset(pMemory, 0, numBytes);

  return pMemory;
}

inline void lorFree(void *pPtr)
{
#if TRACK_MEMORY
  free((char*)pPtr - MemoryPadding);
#else
  free(pPtr);
#endif
}

#endif //LOR_CORE
