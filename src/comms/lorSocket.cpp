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

  union
  {
    struct
    {
      mbedtls_net_context server_fd;
      mbedtls_entropy_context entropy;
      mbedtls_ctr_drbg_context ctr_drbg;
      mbedtls_ssl_context ssl;
      mbedtls_ssl_config conf;
    } tlsClient;
  };
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


static void lorSocketMBEDDebug(void * /*pUserData*/, int /*level*/, const char *file, int line, const char *str)
{
  lorLog("%s:%04d: %s", file, line, str);
}

bool lorSocket_OpenSocket(lorSocket **ppSocket, char *pAddress, uint32_t port, lorSocketConnectionFlags flags)
{
  lorLog("Socket connection to %s:%d", pAddress, port);
  bool openSuccess = false;
  int retVal;

  *ppSocket = lorAllocType(lorSocket, 1);
  lorSocket *pSocket = *ppSocket;

  addrinfo *pOutAddr = nullptr;

  bool isServer = (flags & lSCFIsServer) > 0;
  bool isSecure = (flags & lSCFUseTLS) > 0;

  pSocket->isServer = isServer;
  pSocket->isSecure = isSecure;

  if (!isSecure)
  {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char buffer[6];
    lorSprintf(buffer, sizeof(buffer), "%d", port);
    retVal = getaddrinfo(pAddress, buffer, &hints, &pOutAddr);
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
  }
  else //isSecure
  {
    mbedtls_net_init(&pSocket->tlsClient.server_fd);
    mbedtls_ssl_init(&pSocket->tlsClient.ssl);
    mbedtls_ssl_config_init(&pSocket->tlsClient.conf);
    //mbedtls_x509_crt_init(&pSocket->tlsClient.cacert);
    mbedtls_ctr_drbg_init(&pSocket->tlsClient.ctr_drbg);

    mbedtls_entropy_init(&pSocket->tlsClient.entropy);
    retVal = mbedtls_ctr_drbg_seed(&pSocket->tlsClient.ctr_drbg, mbedtls_entropy_func, &pSocket->tlsClient.entropy, nullptr, 0);

    if(retVal != 0)
    {
      lorLog(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", retVal);
      goto epilogue;
    }

    char portStr[6];
    lorSprintf(portStr, 6, "%d", port);
    retVal = mbedtls_net_connect(&pSocket->tlsClient.server_fd, pAddress, portStr, MBEDTLS_NET_PROTO_TCP);
    if (retVal != 0)
    {
      lorLog(" failed\n  ! mbedtls_net_connect returned %d\n\n", retVal);
      goto epilogue;
    }

    retVal = mbedtls_ssl_config_defaults(&pSocket->tlsClient.conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (retVal != 0)
    {
      lorLog(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", retVal);
      goto epilogue;
    }

    //TODO: Has to be removed before we ship...
    mbedtls_ssl_conf_authmode(&pSocket->tlsClient.conf, MBEDTLS_SSL_VERIFY_NONE);

    //Required
    mbedtls_ssl_conf_rng(&pSocket->tlsClient.conf, mbedtls_ctr_drbg_random, &pSocket->tlsClient.ctr_drbg);
    mbedtls_ssl_conf_dbg(&pSocket->tlsClient.conf, lorSocketMBEDDebug, stdout);

    retVal = mbedtls_ssl_set_hostname(&pSocket->tlsClient.ssl, "mbed TLS Server 1");
    if (retVal != 0)
    {
      lorLog(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", retVal);
      goto epilogue;
    }

    mbedtls_ssl_set_bio(&pSocket->tlsClient.ssl, &pSocket->tlsClient.server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);
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

  if (!(*ppSocket)->isSecure)
  {
#ifdef _MSC_VER
    closesocket((*ppSocket)->sockID);
#else
    close((*ppSocket)->sockID);
#endif
  }
  else
  {
    lorSocket *pSocket = *ppSocket;
    mbedtls_net_free(&pSocket->tlsClient.server_fd);
    mbedtls_ssl_free(&pSocket->tlsClient.ssl);
    mbedtls_ssl_config_free(&pSocket->tlsClient.conf);
    mbedtls_ctr_drbg_free(&pSocket->tlsClient.ctr_drbg);
    mbedtls_entropy_free(&pSocket->tlsClient.entropy);
  }

  lorFree(*ppSocket);
  *ppSocket = nullptr;

  return true;
}

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes)
{
  if (pSocket == nullptr || pBytes == nullptr || totalBytes == 0)
    return false;

  if (!pSocket->isSecure)
    return (totalBytes != (int)send(pSocket->sockID, (const char*)pBytes, totalBytes, 0));
  else
    return (totalBytes != mbedtls_ssl_write(&pSocket->tlsClient.ssl, pBytes, totalBytes));
}

int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize, bool blockForever /*= false*/)
{
  if (pSocket == nullptr || pBytes == nullptr || bufferSize == 0 || pSocket->isServer)
    return 0;

  if (blockForever || pSocket->isSecure)
  {
    if (!pSocket->isSecure)
      return (int)recv(pSocket->sockID, (char*)pBytes, bufferSize, 0);
    else
      return mbedtls_ssl_read(&pSocket->tlsClient.ssl, pBytes, bufferSize);
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
