#include "lorSocket.h"
#include "lorCore.h"

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

#if WINAPI_PARTITION_PHONE_APP
int inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN + 1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy_s(src_copy, INET6_ADDRSTRLEN + 1, src, INET6_ADDRSTRLEN + 1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddressA(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch (af) {
    case AF_INET:
      *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
      return 1;
    case AF_INET6:
      *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
      return 1;
    }
  }
  return 0;
}
#endif

struct lorSocket
{
#ifdef _MSC_VER
  SOCKET sockID;
#else
  int32_t sockID;
#endif //_MSC_VER
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

bool lorSocket_IsValidSocket(lorSocket *pSocket)
{
#ifdef _MSC_VER
  return (pSocket->sockID != INVALID_SOCKET);
#else
  return (pSocket->sockID > 0);
#endif //_MSC_VER
}

bool lorSocket_Connect(lorSocket **ppSocket, char *serverName, uint32_t port)
{
  lorLog("Socket connection to %s:%d", serverName, port);
  bool retVal = false;

  *ppSocket = lorAllocType(lorSocket, 1);
  lorSocket *pSocket = *ppSocket;

  pSocket->sockID = socket(AF_INET, SOCK_STREAM, 0);

  if (!lorSocket_IsValidSocket(pSocket))
    goto epilogue;

  in_addr dest;

  if (inet_pton(AF_INET, serverName, &dest) == 1)
  {
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons((uint16_t)port);
    clientService.sin_addr = dest;

    int socketResult = connect(pSocket->sockID, (const sockaddr*)&clientService, sizeof(clientService));

    if (socketResult == 0)
    {
      retVal = true;
    }
    else
    {
#ifdef _MSC_VER
      lorLog("Socket Error: %d", WSAGetLastError());
#else
      lorLog("Socket Error: %d", errno);
#endif //_MSC_VER
    }
  }

epilogue:
  if (!retVal)
  {
    lorFree(*ppSocket);
    *ppSocket = nullptr;
  }

  lorLog("\t...Connection %s!", retVal?"Success":"Failed");
  return retVal;
}

bool lorSocket_Close(lorSocket **ppSocket)
{
  if (*ppSocket == nullptr)
    return true;

#ifdef _MSC_VER
  int status = shutdown((*ppSocket)->sockID, SD_BOTH);
  if (status == 0)
    status = closesocket((*ppSocket)->sockID);
#else
  int status = shutdown((*ppSocket)->sockID, SHUT_RDWR);
  if (status == 0)
    status = close((*ppSocket)->sockID);
#endif

  lorFree(*ppSocket);
  *ppSocket = nullptr;

  return (status == 0);
}

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes)
{
  return (totalBytes != send(pSocket->sockID, (const char*)pBytes, totalBytes, NULL));
}

int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize)
{
  struct timeval tv;
  fd_set readfds;

  tv.tv_sec = 0;
  tv.tv_usec = 500000;

  FD_ZERO(&readfds);
  FD_SET(pSocket->sockID, &readfds);

  select(NULL, &readfds, nullptr, nullptr, &tv);

  if (FD_ISSET(pSocket->sockID, &readfds))
    return recv(pSocket->sockID, (char*)pBytes, bufferSize, NULL);

  return 0;
}
