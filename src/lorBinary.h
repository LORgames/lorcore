#ifndef lorBinary__h_
#define lorBinary__h_

#include "lorMath.h"

struct lorBinary;
struct lorSocket; //From lorSocket.h

void lorBinary_Init(lorBinary **ppBinary);
void lorBinary_Deinit(lorBinary **ppBinary);

uint32_t lorBinary_GetLength(lorBinary *pBinary);

uint32_t lorBinary_GetPosition(lorBinary *pBinary);
void lorBinary_SetPosition(lorBinary *pBinary, uint32_t position);

bool lorBinary_ReadSocket(lorBinary *pBinary, lorSocket *pSocket);
bool lorBinary_FlushToSocket(lorBinary *pBinary, lorSocket *pSocket); //Clears the binary chunk, writes from position 0

void lorBinary_WriteInt8(lorBinary *pBinary, int8_t val);
void lorBinary_WriteInt16(lorBinary *pBinary, int16_t val);
void lorBinary_WriteInt32(lorBinary *pBinary, int32_t val);
void lorBinary_WriteInt64(lorBinary *pBinary, int64_t val);
void lorBinary_WriteUint8(lorBinary *pBinary, uint8_t val);
void lorBinary_WriteUint16(lorBinary *pBinary, uint16_t val);
void lorBinary_WriteUint32(lorBinary *pBinary, uint32_t val);
void lorBinary_WriteUint64(lorBinary *pBinary, uint64_t val);

int8_t lorBinary_ReadInt8(lorBinary *pBinary);
int16_t lorBinary_ReadInt16(lorBinary *pBinary);
int32_t lorBinary_ReadInt32(lorBinary *pBinary);
int64_t lorBinary_ReadInt64(lorBinary *pBinary);
uint8_t lorBinary_ReadUint8(lorBinary *pBinary);
uint16_t lorBinary_ReadUint16(lorBinary *pBinary);
uint32_t lorBinary_ReadUint32(lorBinary *pBinary);
uint64_t lorBinary_ReadUint64(lorBinary *pBinary);

#endif // !lorBinary__h_
