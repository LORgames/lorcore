#ifndef LOR_MEMORY
#define LOR_MEMORY

#include <stdlib.h>
#include <cstring>

#define TRACK_MEMORY 0

#if TRACK_MEMORY
#define MEMORY_LINEFILE , char *pFilename = "", uint32_t lineNumber = -1
#define lorAllocType(type, count) (type*)lorAlloc(sizeof(type)*count, true, __FILE__, __LINE__)
enum { MemoryPadding = 64 };
#else
#define MEMORY_LINEFILE
#define lorAllocType(type, count) (type*)lorAlloc(sizeof(type)*(count))
#endif

#define lorAlloca alloca

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

#define lorFree(pPtr) _lorFree((void*&)pPtr);

inline void _lorFree(void *&pPtr)
{
#if TRACK_MEMORY
  free((char*)pPtr - MemoryPadding);
#else
  free(pPtr);
#endif

  // This is not thread safe and can lead to double frees.
  // Ideally, this would be done first with an interlocked
  // compare exchange.
  pPtr = nullptr;
}

#endif //LOR_CORE
