#ifndef LOR_SOCKET
#define LOR_SOCKET

#include "lorMath.h"

struct lorSocket;

bool lorSocket_InitSystem();
bool lorSocket_DeinitSystem();

enum lorSocketConnectionFlags
{
  lSCFNone = 0,
  lSCFIsServer = 1,
  lSCFUseTLS = 2
};
inline lorSocketConnectionFlags operator |(lorSocketConnectionFlags a, lorSocketConnectionFlags b) { return (lorSocketConnectionFlags)(((int)a) | ((int)b)); }

bool lorSocket_OpenSocket(lorSocket **ppSocket, char *pAddress, uint32_t port, lorSocketConnectionFlags flags = lSCFNone);
bool lorSocket_CloseSocket(lorSocket **ppSocket);
bool lorSocket_IsValidSocket(lorSocket *pSocket);

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes);
int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize, bool blockForever = false);

bool lorSocket_ServerAcceptClient(lorSocket *pServerSocket, lorSocket **ppClientSocket);

#endif // LOR_SOCKET
