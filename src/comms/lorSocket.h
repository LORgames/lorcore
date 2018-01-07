#ifndef LOR_SOCKET
#define LOR_SOCKET

#include "lorMath.h"

struct lorSocket;

bool lorSocket_InitSystem(const char *pCertificateChain);
bool lorSocket_DeinitSystem();

enum lorSocketConnectionFlags
{
  lSCFNone = 0,
  lSCFIsServer = 1 << 0,
  lSCFUseTLS = 1 << 1
};
inline lorSocketConnectionFlags operator |(lorSocketConnectionFlags a, lorSocketConnectionFlags b) { return (lorSocketConnectionFlags)(((int)a) | ((int)b)); }

bool lorSocket_Init(lorSocket **ppSocket, const char *pAddress, uint32_t port, lorSocketConnectionFlags flags = lSCFNone, const char *pPrivateKey = nullptr, const char *pPublicCertificate = nullptr);
bool lorSocket_Deinit(lorSocket **ppSocket);
bool lorSocket_IsValidSocket(lorSocket *pSocket);

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes);
int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize, bool blockForever = false);

bool lorSocket_ServerAcceptClient(lorSocket *pServerSocket, lorSocket **ppClientSocket);

#endif // LOR_SOCKET
