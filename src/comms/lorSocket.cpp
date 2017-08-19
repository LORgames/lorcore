#include "lorSocket.h"
#include "lorCore.h"

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"

#ifdef _MSC_VER
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif

#ifndef INVALID_SOCKET //Some platforms don't have these defined
typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif //INVALID_SOCKET

struct lorSocket
{
  SOCKET sockID;
  bool isServer;
  bool isSecure;
};

bool lorSocket_InitSystem()
{
#ifdef _MSC_VER
  WSADATA wsa_data;
  return (WSAStartup(MAKEWORD(1, 1), &wsa_data) == 0);
#else
  return true;
#endif
}

bool lorSocket_DeinitSystem()
{
#ifdef _MSC_VER
  return (WSACleanup() == 0);
#else
  return true;
#endif
}

int lorSocket_GetErrorCode()
{
#ifdef _MSC_VER
  return WSAGetLastError();
#else
  return errno;
#endif
}

bool lorSocket_IsValidSocket(lorSocket *pSocket)
{
  return (pSocket->sockID != INVALID_SOCKET);
}

bool lorSocket_OpenSocket(lorSocket **ppSocket, char *pAddress, uint32_t port, lorSocketConnectionFlags flags)
{
  lorLog("Socket connection to %s:%d", pAddress, port);
  bool openSuccess = false;

  *ppSocket = lorAllocType(lorSocket, 1);
  lorSocket *pSocket = *ppSocket;

  bool isServer = (flags & lSCFIsServer) > 0;
  //bool isSecure = (flags & lSCFUseTLS) > 0;

  addrinfo *pOutAddr;

  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  char buffer[6];
  lorSprintf(buffer, sizeof(buffer), "%d", port);
  int retVal = getaddrinfo(pAddress, buffer, &hints, &pOutAddr);
  if (retVal != 0)
    goto epilogue;

  pSocket->sockID = socket(pOutAddr->ai_family, pOutAddr->ai_socktype, (int)pOutAddr->ai_addrlen);

  if (!lorSocket_IsValidSocket(pSocket))
    goto epilogue;

  if (isServer)
  {
    pSocket->isServer = true;

    retVal = bind(pSocket->sockID, pOutAddr->ai_addr, (int)pOutAddr->ai_addrlen);
    if (retVal == SOCKET_ERROR)
      goto epilogue;

    retVal = listen(pSocket->sockID, SOMAXCONN);
    if (retVal == SOCKET_ERROR)
      goto epilogue;
  }
  else
  {
    retVal = connect(pSocket->sockID, pOutAddr->ai_addr, (int)pOutAddr->ai_addrlen);
    if (retVal == SOCKET_ERROR)
      goto epilogue;
  }

  openSuccess = true;

epilogue:
  if (pOutAddr != nullptr)
    freeaddrinfo(pOutAddr);

  if (!openSuccess)
    lorSocket_CloseSocket(&pSocket);

  lorLog("\t...Connection %s!", openSuccess ? "Success" : "Failed");
  return openSuccess;
}

bool lorSocket_CloseSocket(lorSocket **ppSocket)
{
  if (*ppSocket == nullptr)
    return true;

#ifdef _MSC_VER
  closesocket((*ppSocket)->sockID);
#else
  close((*ppSocket)->sockID);
#endif

  lorFree(*ppSocket);
  *ppSocket = nullptr;

  return true;
}

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes)
{
  if (pSocket == nullptr || pBytes == nullptr || totalBytes == 0)
    return false;

  return (totalBytes != (int)send(pSocket->sockID, (const char*)pBytes, totalBytes, 0));
}

int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize, bool blockForever /*= false*/)
{
  if (pSocket == nullptr || pBytes == nullptr || bufferSize == 0 || pSocket->isServer)
    return 0;

  if (blockForever)
  {
    return (int)recv(pSocket->sockID, (char*)pBytes, bufferSize, 0);
  }
  else
  {
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(pSocket->sockID, &readfds);

    select(0, &readfds, nullptr, nullptr, &tv);

    if (FD_ISSET(pSocket->sockID, &readfds))
      return (int)recv(pSocket->sockID, (char*)pBytes, bufferSize, 0);
  }

  return 0;
}

bool lorSocket_ServerAcceptClient(lorSocket *pServerSocket, lorSocket **ppClientSocket)
{
  if (ppClientSocket == nullptr || pServerSocket == nullptr || !pServerSocket->isServer)
    return false;

  *ppClientSocket = nullptr;

  SOCKET clientSocket = accept(pServerSocket->sockID, nullptr, nullptr);

  if (clientSocket != INVALID_SOCKET)
  {
    (*ppClientSocket) = lorAllocType(lorSocket, 1);
    (*ppClientSocket)->sockID = clientSocket;
    return true;
  }

  return false;
}
