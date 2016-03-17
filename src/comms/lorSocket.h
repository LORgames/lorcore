#ifndef LOR_SOCKET
#define LOR_SOCKET

#include "lorMath.h"

struct lorSocket;

bool lorSocket_InitSystem();
bool lorSocket_DeinitSystem();

bool lorSocket_Connect(lorSocket **ppSocket, char *serverName, uint32_t port);
bool lorSocket_Close(lorSocket **ppSocket);
bool lorSocket_IsValidSocket(lorSocket *pSocket);

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes);
int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize);

#endif // LOR_SOCKET
