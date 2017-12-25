#ifndef LOR_ARRAY
#define LOR_ARRAY

#include "lorPlatform.h"

template <typename T, uint32_t elementsPerBlock>
struct lorArray
{
  bool Init();
  void Deinit();

  void Clear();

  T &operator[](size_t index);

  T *PushBack();
  bool GrowBack(uint32_t numberOfNewElements);

  bool PopBack();
  bool PopFront();

  void RemoveSwapLast(size_t index);

  void Sort(int (*pComparisonFunc)(T *pElementA, T *pElementB), size_t start = 0, size_t end = -1);

  void CopyFrom(const T *pArray, size_t startIndex, size_t copySize);
  void CopyTo(size_t startIndex, size_t copySize, T *pArray);

  template<typename _T, uint32_t _elementsPerBlock>
  struct block
  {
    _T data[_elementsPerBlock];
  };

  typedef block<T, elementsPerBlock> block_t;
  enum { ptrArrayInc = 32 };

  block_t **ppBlocks;
  uint32_t ptrArraySize;
  uint32_t blockCount;

  uint32_t length;
  uint32_t offset;

  bool AddBlocks(uint32_t numberOfNewBlocks);
};

template <typename T, uint32_t elementsPerBlock>
inline bool lorArray<T, elementsPerBlock>::Init()
{
  length = 0;
  offset = 0;
  blockCount = 0;

  ptrArraySize = ptrArrayInc;

  ppBlocks = lorAllocType(block_t*, ptrArraySize);
  if (!ppBlocks)
    goto epilogue;

  blockCount = 1;

  ppBlocks[0] = lorAllocType(block_t, 1);
  if (!ppBlocks[0])
    goto epilogue;

  return true;
epilogue:
  for (uint32_t i = 0; i < blockCount; ++i)
    lorFree(ppBlocks[i]);

  lorFree(ppBlocks);

  return false;
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::Deinit()
{
  for (uint32_t i = 0; i < blockCount; ++i)
    lorFree(ppBlocks[i]);

  lorFree(ppBlocks);

  blockCount = 0;
  length = 0;
  offset = 0;
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::Clear()
{
  length = 0;
  offset = 0;
}

template <typename T, uint32_t elementsPerBlock>
inline bool lorArray<T, elementsPerBlock>::AddBlocks(uint32_t numberOfNewBlocks)
{
  uint32_t newBlockCount = blockCount + numberOfNewBlocks;

  if (newBlockCount > ptrArraySize)
  {
    uint32_t newPtrArraySize = ((newBlockCount + ptrArrayInc - 1) / ptrArrayInc) * ptrArrayInc;
    block_t **newppBlocks = lorAllocType(block_t*, newPtrArraySize);
    if (!newppBlocks)
      return false;

    memcpy(newppBlocks, ppBlocks, ptrArraySize * sizeof(block_t*));
    lorFree(ppBlocks);

    ppBlocks = newppBlocks;
    ptrArraySize = newPtrArraySize;
  }

  for (uint32_t c = blockCount; c < newBlockCount; ++c)
  {
    ppBlocks[c] = lorAllocType(block_t, 1);
    if (!ppBlocks[c])
    {
      blockCount = c;
      return false;
    }
  }

  blockCount = newBlockCount;
  return true;
}

template <typename T, uint32_t elementsPerBlock>
inline bool lorArray<T, elementsPerBlock>::GrowBack(uint32_t numberOfNewElements)
{
  if (numberOfNewElements == 0)
    return true;

  uint32_t oldLength = offset + length;
  uint32_t newLength = oldLength + numberOfNewElements;
  uint32_t prevUsedBlockCount = (oldLength + elementsPerBlock - 1) / elementsPerBlock;

  const uint32_t capacity = elementsPerBlock * blockCount;
  if (newLength > capacity)
  {
    uint32_t requiredEntries = newLength - capacity;
    uint32_t numberOfNewBlocksToAdd = (requiredEntries + elementsPerBlock - 1) / elementsPerBlock;

    if (!AddBlocks(numberOfNewBlocksToAdd))
      return false;
  }

  // Zero new elements
  uint32_t newUsedBlockCount = (newLength + elementsPerBlock - 1) / elementsPerBlock;
  uint32_t usedBlockDelta = newUsedBlockCount - prevUsedBlockCount;
  uint32_t head = oldLength % elementsPerBlock;

  if (usedBlockDelta)
  {
    if (head)
      memset(&ppBlocks[prevUsedBlockCount - 1]->data[head], 0, (elementsPerBlock - head) * sizeof(T));

    uint32_t tail = newLength % elementsPerBlock;

    for (uint32_t blockIndex = prevUsedBlockCount; blockIndex < (newUsedBlockCount - 1 + !tail); ++blockIndex)
      memset(ppBlocks[blockIndex]->data, 0, sizeof(block_t));

    if (tail)
      memset(&ppBlocks[newUsedBlockCount - 1]->data[0], 0, tail * sizeof(T));
  }
  else
  {
    memset(&ppBlocks[prevUsedBlockCount - 1]->data[head], 0, numberOfNewElements * sizeof(T));
  }

  length += numberOfNewElements;

  return true;
}

template <typename T, uint32_t elementsPerBlock>
inline T &lorArray<T, elementsPerBlock>::operator[](size_t index)
{
  lorAssert(index < length, "Index out of bounds");
  index += offset;
  size_t blockIndex = index / elementsPerBlock;
  return ppBlocks[blockIndex]->data[index % elementsPerBlock];
}

template <typename T, uint32_t elementsPerBlock>
inline T *lorArray<T, elementsPerBlock>::PushBack()
{
  uint32_t newIndex = offset + length;
  if (newIndex >= (elementsPerBlock * blockCount))
    AddBlocks(1);

  uint32_t blockIndex = newIndex / elementsPerBlock;

  T *pElement = &ppBlocks[blockIndex]->data[newIndex % elementsPerBlock];
  memset(pElement, 0, sizeof(T));

  ++length;
  return pElement;
}

template <typename T, uint32_t elementsPerBlock>
inline bool lorArray<T, elementsPerBlock>::PopBack()
{
  if (length)
  {
    --length;

    if (length == 0)
      offset = 0;

    return true;
  }
  return false;
}

template <typename T, uint32_t elementsPerBlock>
inline bool lorArray<T, elementsPerBlock>::PopFront()
{
  if (length)
  {
    ++offset;

    // Move all the blocks back if the first one is unused
    if (offset == elementsPerBlock)
    {
      offset = 0;
      if (blockCount > 1)
      {
        block_t *pHead = ppBlocks[0];
        memmove(ppBlocks, ppBlocks + 1, (blockCount - 1) * sizeof(block_t*));
        ppBlocks[blockCount - 1] = pHead;
      }
    }

    --length;

    if (length == 0)
      offset = 0;

    return true;
  }

  return false;
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::RemoveSwapLast(size_t index)
{
  lorAssert(index < length, "Index out of bounds");
  if (index != (length - 1))
    ppBlocks[index / elementsPerBlock]->data[index % elementsPerBlock] = operator[](length - 1);

  --length;
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::Sort(int(*pComparisonFunc)(T *pElementA, T *pElementB), size_t start /*= 0*/, size_t end /*= -1*/)
{
  if (end == -1 || end >= length)
    end = (length - 1);

  if (start < end)
  {
    //TODO: Such a bad way to pick a pivot; this is like programmer lvl 1 pivot picking
    //TBH this entire sort so bad that it's sad
    T* pPivot = &ppBlocks[(end + offset) / elementsPerBlock]->data[(end + offset) % elementsPerBlock];
    size_t i = start - 1;
    {
      for (size_t j = start; j <= (end - 1); ++j)
      {
        if (pComparisonFunc(&ppBlocks[(j + offset) / elementsPerBlock]->data[(j + offset) % elementsPerBlock], pPivot) < 0)
        {
          i++;
          T temp = ppBlocks[(i + offset) / elementsPerBlock]->data[(i + offset) % elementsPerBlock];
          ppBlocks[(i + offset) / elementsPerBlock]->data[(i + offset) % elementsPerBlock] = ppBlocks[(j + offset) / elementsPerBlock]->data[(j + offset) % elementsPerBlock];
          ppBlocks[(j + offset) / elementsPerBlock]->data[(j + offset) % elementsPerBlock] = temp;
        }
      }

      if (pComparisonFunc(pPivot, &ppBlocks[(i + 1 + offset) / elementsPerBlock]->data[(i + 1 + offset) % elementsPerBlock]) < 0)
      {
        T temp = ppBlocks[(i + 1 + offset) / elementsPerBlock]->data[(i + 1 + offset) % elementsPerBlock];
        ppBlocks[(i + 1 + offset) / elementsPerBlock]->data[(i + 1 + offset) % elementsPerBlock] = ppBlocks[(end + offset) / elementsPerBlock]->data[(end + offset) % elementsPerBlock];
        ppBlocks[(end + offset) / elementsPerBlock]->data[(end + offset) % elementsPerBlock] = temp;
      }
    }

    Sort(pComparisonFunc, start, i);
    return Sort(pComparisonFunc, i + 2, end); //Tail call
  }
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::CopyFrom(const T *pArray, size_t startIndex, size_t copySize)
{
  lorAssert(pArray != nullptr, "Array was nullptr");

  // Add new blocks
  size_t spaceAtEnd = (blockCount * elementsPerBlock) - length + offset;
  if (copySize > spaceAtEnd)
  {
    uint32_t numElementsRequired = (uint32_t)(copySize - spaceAtEnd);
    uint32_t numBlocksRequired = (numElementsRequired / elementsPerBlock) + ((numElementsRequired % elementsPerBlock) == 0 ? 0 : 1);
    AddBlocks(numBlocksRequired);
  }

  // Handle end of existing block
  size_t blockLengthUsed = (length + offset) % elementsPerBlock;
  size_t blockLengthFree = elementsPerBlock - blockLengthUsed;
  if (blockLengthUsed != 0)
  {
    size_t copyAmount = lorMin(blockLengthFree, copySize);
    block_t *pBlock = ppBlocks[(length + offset) / elementsPerBlock];
    block_t *pBlockStart = &(pBlock[blockLengthUsed]);
    (void)pBlockStart;
    memcpy(&(ppBlocks[(length + offset) / elementsPerBlock]->data[blockLengthUsed]), &(pArray[startIndex]), sizeof(T) * copyAmount);
    length += (uint32_t)copyAmount;
    startIndex += copyAmount;
    copySize -= copyAmount;
  }

  // Handle full block copies
  while (copySize >= elementsPerBlock)
  {
    memcpy((ppBlocks[(length + offset) / elementsPerBlock]->data), &(pArray[startIndex]), sizeof(T) * elementsPerBlock);
    length += elementsPerBlock;
    startIndex += elementsPerBlock;
    copySize -= elementsPerBlock;
  }

  // Handle last partial block copy
  if (copySize > 0)
  {
    memcpy((ppBlocks[(length + offset) / elementsPerBlock]->data), &(pArray[startIndex]), sizeof(T) * copySize);
    length += (uint32_t)copySize;
    startIndex += copySize;
    copySize -= copySize;
  }

  lorAssert(copySize == 0, "Not all elements copied");
}

template <typename T, uint32_t elementsPerBlock>
inline void lorArray<T, elementsPerBlock>::CopyTo(size_t startIndex, size_t copySize, T *pArray)
{
  lorAssert(pArray != nullptr, "Array was nullptr");
  lorAssert(startIndex < length, "Index was out of bounds");
  lorAssert((copySize + startIndex) <= length, "Copy size goes out of bounds");

  size_t arrayOffset = 0;
  startIndex += offset;

  size_t blockIndex = startIndex / elementsPerBlock;
  size_t blockOffset = startIndex % elementsPerBlock;

  // Handle end of first block
  if (blockOffset != 0)
  {
    size_t copyAmount = lorMin(copySize, elementsPerBlock - blockOffset);
    memcpy(&(pArray[arrayOffset]), &(ppBlocks[blockIndex]->data[blockOffset]), sizeof(T) * copyAmount);
    arrayOffset += copyAmount;
    startIndex += copyAmount;
    copySize -= copyAmount;
  }

  // Handle full block copies
  while (copySize >= elementsPerBlock)
  {
    blockIndex = startIndex / elementsPerBlock;
    memcpy(&(pArray[arrayOffset]), &(ppBlocks[blockIndex]->data[0]), sizeof(T) * elementsPerBlock);
    arrayOffset += elementsPerBlock;
    startIndex += elementsPerBlock;
    copySize -= elementsPerBlock;
  }

  // Handle last block
  if (copySize > 0)
  {
    blockIndex = startIndex / elementsPerBlock;
    memcpy(&(pArray[arrayOffset]), &(ppBlocks[blockIndex]->data[0]), sizeof(T) * copySize);
    arrayOffset += copySize;
    startIndex += copySize;
    copySize -= copySize;
  }

  lorAssert(copySize == 0, "Not all elements copied");
}

#endif //LOR_CHUNKED_ARRAY
