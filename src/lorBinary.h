#ifndef lorBinary__h_
#define lorBinary__h_

#include "lorMath.h"

struct lorBinary;
struct lorSocket; //From lorSocket.h
struct lorFile; //From lorFile.h

void lorBinary_Init(lorBinary **ppBinary);
void lorBinary_Deinit(lorBinary **ppBinary);

uint32_t lorBinary_GetLength(lorBinary *pBinary);

bool lorBinary_IsPositionAtEnd(lorBinary *pBinary);
uint32_t lorBinary_GetPosition(lorBinary *pBinary);
void lorBinary_SetPosition(lorBinary *pBinary, uint32_t position);

void lorBinary_Clear(lorBinary *pBinary);

bool lorBinary_ReadSocket(lorBinary *pBinary, lorSocket *pSocket, int *pBytesRead = nullptr);
bool lorBinary_FlushToSocket(lorBinary *pBinary, lorSocket *pSocket, bool clearBuffer = true); //Writes entire binary chunk from position 0

bool lorBinary_ReadFile(lorBinary *pBinary, lorFile *pFile, size_t *pBytesRead = nullptr);
bool lorBinary_FlushToFile(lorBinary *pBinary, lorFile *pFile, bool clearBuffer = true); //Writes entire binary chunk from position 0

//Write Functions
void lorBinary_WriteInt8(lorBinary *pBinary, const int8_t val);
void lorBinary_WriteInt16(lorBinary *pBinary, const int16_t val);
void lorBinary_WriteInt32(lorBinary *pBinary, const int32_t val);
void lorBinary_WriteInt64(lorBinary *pBinary, const int64_t val);
void lorBinary_WriteUint8(lorBinary *pBinary, const uint8_t val);
void lorBinary_WriteUint16(lorBinary *pBinary, const uint16_t val);
void lorBinary_WriteUint32(lorBinary *pBinary, const uint32_t val);
void lorBinary_WriteUint64(lorBinary *pBinary, const uint64_t val);

void lorBinary_WriteStringFixedLength(lorBinary *pBinary, const char *pStr, uint32_t numBytes); //Will overrun memory if pStr is shorter than numBytes
void lorBinary_WriteStringPascal16(lorBinary *pBinary, const char *pStr); //Note this is limited to 64k characters
void lorBinary_WriteStringNullTerminated(lorBinary *pBinary, const char *pStr);

//Read Functions
int8_t lorBinary_ReadInt8(lorBinary *pBinary);
int16_t lorBinary_ReadInt16(lorBinary *pBinary);
int32_t lorBinary_ReadInt32(lorBinary *pBinary);
int64_t lorBinary_ReadInt64(lorBinary *pBinary);
uint8_t lorBinary_ReadUint8(lorBinary *pBinary);
uint16_t lorBinary_ReadUint16(lorBinary *pBinary);
uint32_t lorBinary_ReadUint32(lorBinary *pBinary);
uint64_t lorBinary_ReadUint64(lorBinary *pBinary);

const char* lorBinary_ReadStringFixedLength(lorBinary *pBinary, uint32_t numBytes); //Needs to be lorFree'd
const char* lorBinary_ReadStringPascal16(lorBinary *pBinary); //Needs to be lorFree'd
const char* lorBinary_ReadStringNullTerminated(lorBinary *pBinary); //Needs to be lorFree'd

bool lorBinary_ReadStringFixedLength(lorBinary *pBinary, char *pBuffer, uint32_t numBytes);
bool lorBinary_ReadStringPascal16(lorBinary *pBinary, char *pBuffer, uint32_t bufferSize); //If false, the buffer wasn't big enough and the position wasn't changed
bool lorBinary_ReadStringNullTerminated(lorBinary *pBinary, char *pBuffer, uint32_t bufferSize); //If false, the buffer wasn't big enough and the position wasn't changed

#endif // !lorBinary__h_
