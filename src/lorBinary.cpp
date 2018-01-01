#include "lorBinary.h"
#include "lorArray.h"
#include "lorSocket.h"

enum
{
  ChunkSize = 16384, //16KB
};

struct lorBinary
{
  lorArray<uint8_t, ChunkSize> bytes;
  uint32_t carat;
};

void lorBinary_Init(lorBinary **ppBinary)
{
  if (ppBinary == nullptr)
    return;

  *ppBinary = lorAllocType(lorBinary, 1);
  (*ppBinary)->bytes.Init();
}

void lorBinary_Deinit(lorBinary **ppBinary)
{
  if (ppBinary == nullptr || *ppBinary == nullptr)
    return;

  (*ppBinary)->bytes.Deinit();
  lorFree(*ppBinary);
}

uint32_t lorBinary_GetLength(lorBinary *pBinary)
{
  return pBinary->bytes.length;
}

bool lorBinary_IsPositionAtEnd(lorBinary *pBinary)
{
  return (pBinary->carat == pBinary->bytes.length);
}

uint32_t lorBinary_GetPosition(lorBinary *pBinary)
{
  return pBinary->carat;
}

void lorBinary_SetPosition(lorBinary *pBinary, uint32_t position)
{
  //Should this return if it fails?
  if(position < pBinary->bytes.length)
    pBinary->carat = position;
}

void lorBinary_Clear(lorBinary *pBinary)
{
  pBinary->bytes.Clear();
  pBinary->carat = 0;
}

bool lorBinary_ReadSocket(lorBinary *pBinary, lorSocket *pSocket, int *pBytesRead /*= nullptr*/)
{
  uint8_t bytes[ChunkSize];
  int bytesRead = lorSocket_ReceiveData(pSocket, bytes, sizeof(bytes));

  if (pBytesRead != nullptr)
    *pBytesRead = bytesRead;

  if (bytesRead > 0)
  {
    if (pBinary->bytes.length < pBinary->carat + bytesRead)
      pBinary->bytes.GrowBack(pBinary->carat + bytesRead - pBinary->bytes.length);

    //TODO: Read in chunks
    for (int i = 0; i < bytesRead; ++i)
    {
      pBinary->bytes[i + pBinary->carat] = bytes[i];
    }

    return true;
  }

  return false;
}

bool lorBinary_FlushToSocket(lorBinary *pBinary, lorSocket *pSocket, bool clearBuffer /*= true*/)
{
  uint32_t totalBytes = pBinary->bytes.length;

  bool success = true;

  int i;
  for (i = 0; totalBytes >= ChunkSize && success; ++i)
  {
    success = lorSocket_SendData(pSocket, pBinary->bytes.ppBlocks[i]->data, ChunkSize);
    totalBytes -= ChunkSize;
  }

  if(totalBytes > 0 && success)
    success = lorSocket_SendData(pSocket, pBinary->bytes.ppBlocks[i]->data, (uint16_t)totalBytes);

  if (success && clearBuffer)
    lorBinary_Clear(pBinary);

  return success;
}

template<typename T>
void lorBinary_WriteInt(lorBinary *pBinary, T val)
{
  if (pBinary->bytes.length < pBinary->carat + sizeof(T))
    pBinary->bytes.GrowBack(pBinary->carat + sizeof(T) - pBinary->bytes.length);

  for (int i = 0; i < sizeof(T); ++i)
  {
    pBinary->bytes[pBinary->carat + i] = (uint8_t)((val >> (i*8)) & 0xFF);
  }

  pBinary->carat += sizeof(T);
}

//Intentionally single line
void lorBinary_WriteInt8(lorBinary *pBinary, const int8_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteInt16(lorBinary *pBinary, const int16_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteInt32(lorBinary *pBinary, const int32_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteInt64(lorBinary *pBinary, const int64_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteUint8(lorBinary *pBinary, const uint8_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteUint16(lorBinary *pBinary, const uint16_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteUint32(lorBinary *pBinary, const uint32_t val) { lorBinary_WriteInt(pBinary, val); }
void lorBinary_WriteUint64(lorBinary *pBinary, const uint64_t val) { lorBinary_WriteInt(pBinary, val); }

void lorBinary_WriteStringFixedLength(lorBinary *pBinary, const char *pStr, uint32_t numBytes)
{
  for (uint32_t i = 0; i < numBytes; ++i)
    lorBinary_WriteInt(pBinary, ((uint8_t*)pStr)[i]);
}

void lorBinary_WriteStringPascal16(lorBinary *pBinary, const char *pStr)
{
  //Pascal strings don't include null terminator
  uint16_t length = (uint16_t)lorMin(lorStrlen(pStr), (size_t)UINT16_MAX);
  lorBinary_WriteInt(pBinary, length);

  for (uint32_t i = 0; i < length; ++i)
    lorBinary_WriteInt(pBinary, ((uint8_t*)pStr)[i]);
}

void lorBinary_WriteStringNullTerminated(lorBinary *pBinary, const char *pStr)
{
  for(size_t i = 0; ; ++i)
  {
    lorBinary_WriteInt(pBinary, ((uint8_t*)pStr)[i]);
    if (pStr[i] == '\0')
      break;
  }
}

template<typename T>
T lorBinary_ReadInt(lorBinary *pBinary)
{
  T temp = 0;

  if (pBinary->bytes.length < pBinary->carat + sizeof(T))
    return temp; //This is bad...

  for (int i = 0; i < sizeof(T); ++i)
  {
    temp |= (((T)pBinary->bytes[pBinary->carat]) << (i * 8));
    ++pBinary->carat;
  }

  return temp;
}

int8_t lorBinary_ReadInt8(lorBinary *pBinary) { return lorBinary_ReadInt<int8_t>(pBinary); }
int16_t lorBinary_ReadInt16(lorBinary *pBinary) { return lorBinary_ReadInt<int16_t>(pBinary); }
int32_t lorBinary_ReadInt32(lorBinary *pBinary) { return lorBinary_ReadInt<int32_t>(pBinary); }
int64_t lorBinary_ReadInt64(lorBinary *pBinary) { return lorBinary_ReadInt<int64_t>(pBinary); }
uint8_t lorBinary_ReadUint8(lorBinary *pBinary) { return lorBinary_ReadInt<uint8_t>(pBinary); }
uint16_t lorBinary_ReadUint16(lorBinary *pBinary) { return lorBinary_ReadInt<uint16_t>(pBinary); }
uint32_t lorBinary_ReadUint32(lorBinary *pBinary) { return lorBinary_ReadInt<uint32_t>(pBinary); }
uint64_t lorBinary_ReadUint64(lorBinary *pBinary) { return lorBinary_ReadInt<uint64_t>(pBinary); }

const char* lorBinary_ReadStringFixedLength(lorBinary *pBinary, uint32_t numBytes)
{
  char *pData = lorAllocType(char, numBytes);

  for (uint32_t i = 0; i < numBytes; ++i)
    pData[i] = lorBinary_ReadInt<int8_t>(pBinary);

  return pData;
}

const char* lorBinary_ReadStringPascal16(lorBinary *pBinary)
{
  uint16_t numBytes = lorBinary_ReadInt<uint16_t>(pBinary);
  char *pData = lorAllocType(char, numBytes+1); //+1 for null terminator

  for (uint32_t i = 0; i < numBytes; ++i)
    pData[i] = lorBinary_ReadInt<int8_t>(pBinary);

  return pData;
}

const char* lorBinary_ReadStringNullTerminated(lorBinary *pBinary)
{
  uint16_t numBytes = 0;

  for (uint32_t i = pBinary->carat; pBinary->bytes.length; ++i)
  {
    ++numBytes;
    if (pBinary->bytes[i] == '\0')
      break;
  }

  char *pData = lorAllocType(char, numBytes);

  for (uint32_t i = 0; i < numBytes; ++i)
    pData[i] = lorBinary_ReadInt<int8_t>(pBinary);

  return pData;
}

bool lorBinary_ReadStringFixedLength(lorBinary *pBinary, char *pBuffer, uint32_t numBytes)
{
  if (pBinary->carat + numBytes > pBinary->bytes.length)
    return false;

  for (uint32_t i = 0; i < numBytes; ++i)
    pBuffer[i] = lorBinary_ReadInt<int8_t>(pBinary);

  return true;
}

bool lorBinary_ReadStringPascal16(lorBinary *pBinary, char *pBuffer, uint32_t bufferSize)
{
  uint16_t numBytes = lorBinary_ReadInt<uint16_t>(pBinary);

  if (numBytes + 1U >= bufferSize) //+1 for the null terminator
    return false;

  pBuffer[numBytes] = '\0';

  return lorBinary_ReadStringFixedLength(pBinary, pBuffer, numBytes);
}

bool lorBinary_ReadStringNullTerminated(lorBinary *pBinary, char *pBuffer, uint32_t bufferSize)
{
  uint16_t numBytes = 0;

  for (uint32_t i = pBinary->carat; pBinary->bytes.length; ++i)
  {
    ++numBytes;
    if (pBinary->bytes[i] == '\0')
      break;
  }

  if (numBytes >= bufferSize)
    return false;

  return lorBinary_ReadStringFixedLength(pBinary, pBuffer, numBytes);
}
