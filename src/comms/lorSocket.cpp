#include "lorSocket.h"
#include "lorPlatform.h"

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/x509.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"

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

static struct
{
  mbedtls_entropy_context entropy;
  mbedtls_x509_crt certificateChain;
} g_sharedSocketData;

struct lorSocket
{
  SOCKET basicSocket;
  bool isServer;
  bool isSecure;

  struct
  {
    mbedtls_net_context socketContext; //The actual socket
    mbedtls_ctr_drbg_context ctr_drbg; //The encryption context
    mbedtls_ssl_context ssl; //The socket to encryption context
    mbedtls_ssl_config conf;

    //Additional server things
    mbedtls_x509_crt certificateServer;
    mbedtls_pk_context publicKey;
  } tlsClient;
};

bool lorSocket_InitSystem(const char *pCertificateChain /*= nullptr*/)
{
  mbedtls_entropy_init(&g_sharedSocketData.entropy);
  mbedtls_x509_crt_init(&g_sharedSocketData.certificateChain);

  //LOAD CA CERTIFICATES
  if (pCertificateChain != nullptr)
  {
    int retVal = mbedtls_x509_crt_parse(&g_sharedSocketData.certificateChain, (const unsigned char *)pCertificateChain, lorStrlen(pCertificateChain)+1);
    if (retVal != 0)
      lorLog("Could not parse certificate chain Error: -0x%x", -retVal);
  }

#ifdef _MSC_VER
  WSADATA wsa_data;
  return (WSAStartup(MAKEWORD(1, 1), &wsa_data) == 0);
#else
  return true;
#endif
}

bool lorSocket_DeinitSystem()
{
  mbedtls_entropy_free(&g_sharedSocketData.entropy);
  mbedtls_x509_crt_free(&g_sharedSocketData.certificateChain);

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
  if (pSocket->isSecure)
    return (pSocket->tlsClient.socketContext.fd != INVALID_SOCKET && pSocket->tlsClient.socketContext.fd != 0);
  else
    return (pSocket->basicSocket != INVALID_SOCKET);
}


static void lorSocketMBEDDebug(void * /*pUserData*/, int /*level*/, const char *file, int line, const char *str)
{
  lorLog("%s:%04d: %s", file, line, str);
}

bool lorSocket_Init(lorSocket **ppSocket, const char *pAddress, uint32_t port, lorSocketConnectionFlags flags, const char *pPrivateKey /*= nullptr*/, const char *pPublicCertificate /*= nullptr*/)
{
  if (ppSocket == nullptr)
    return false;

  lorLog("Socket init (%s:%d) flags=%d", pAddress, port, flags);
  bool openSuccess = false;
  int retVal;

  lorSocket *pSocket = lorAllocType(lorSocket, 1);
  addrinfo *pOutAddr = nullptr;

  bool isServer = (flags & lSCFIsServer) > 0;
  bool isSecure = (flags & lSCFUseTLS) > 0;

  pSocket->isServer = isServer;
  pSocket->isSecure = isSecure;

  if (isSecure)
  {
    //Init everything
    mbedtls_net_init(&pSocket->tlsClient.socketContext);
    mbedtls_ssl_init(&pSocket->tlsClient.ssl);
    mbedtls_ssl_config_init(&pSocket->tlsClient.conf);
    mbedtls_ctr_drbg_init(&pSocket->tlsClient.ctr_drbg);
    mbedtls_x509_crt_init(&pSocket->tlsClient.certificateServer);

    //Set up encryption things
    retVal = mbedtls_ctr_drbg_seed(&pSocket->tlsClient.ctr_drbg, mbedtls_entropy_func, &g_sharedSocketData.entropy, nullptr, 0);
    if (retVal != 0)
    {
      lorLog(" failed! mbedtls_ctr_drbg_seed returned %d", retVal);
      goto epilogue;
    }

    //Setup the port string
    char portStr[6];
    lorSprintf(portStr, 6, "%d", port);

    //Branch for server/client differences
    if (isServer)
    {
      if (pPrivateKey == nullptr || pPublicCertificate == nullptr)
      {
        lorLog("Certificate and private key cannot be null if running a secure server!");
        goto epilogue;
      }

      //Set up server certificate
      retVal = mbedtls_x509_crt_parse(&pSocket->tlsClient.certificateServer, (const unsigned char *)pPublicCertificate, lorStrlen(pPublicCertificate)+1);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_x509_crt_parse returned %d", retVal);
        goto epilogue;
      }

      //Set up public key
      mbedtls_pk_init(&pSocket->tlsClient.publicKey);
      retVal = mbedtls_pk_parse_key(&pSocket->tlsClient.publicKey, (const unsigned char *)pPrivateKey, lorStrlen(pPrivateKey)+1, NULL, 0);
      if (retVal != 0)
      {
        lorLog(" failed!  mbedtls_pk_parse_key returned %d", retVal);
        goto epilogue;
      }

      //Bind
      lorLog(" TLS Socket Bind on %s:%s", pAddress, portStr);
      retVal = mbedtls_net_bind(&pSocket->tlsClient.socketContext, pAddress, portStr, MBEDTLS_NET_PROTO_TCP);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_net_bind returned %d", retVal);
        goto epilogue;
      }

      //Set up config stuff for server
      retVal = mbedtls_ssl_config_defaults(&pSocket->tlsClient.conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_ssl_config_defaults returned %d", retVal);
        goto epilogue;
      }

      //Link certificate chains
      mbedtls_ssl_conf_ca_chain(&pSocket->tlsClient.conf, &g_sharedSocketData.certificateChain, NULL);
      retVal = mbedtls_ssl_conf_own_cert(&pSocket->tlsClient.conf, &pSocket->tlsClient.certificateServer, &pSocket->tlsClient.publicKey);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_ssl_conf_own_cert returned %d", retVal);
        goto epilogue;
      }
    }
    else //Is Client
    {
      //Connect
      retVal = mbedtls_net_connect(&pSocket->tlsClient.socketContext, pAddress, portStr, MBEDTLS_NET_PROTO_TCP);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_net_connect returned %d", retVal);
        goto epilogue;
      }

      //Config stuff
      retVal = mbedtls_ssl_config_defaults(&pSocket->tlsClient.conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_ssl_config_defaults returned %d", retVal);
        goto epilogue;
      }

      //TODO: Has to be removed before we ship...
      mbedtls_ssl_conf_authmode(&pSocket->tlsClient.conf, MBEDTLS_SSL_VERIFY_NONE);
    }

    //Required
    mbedtls_ssl_conf_rng(&pSocket->tlsClient.conf, mbedtls_ctr_drbg_random, &pSocket->tlsClient.ctr_drbg);
    mbedtls_ssl_conf_dbg(&pSocket->tlsClient.conf, lorSocketMBEDDebug, stdout);

    retVal = mbedtls_ssl_setup(&pSocket->tlsClient.ssl, &pSocket->tlsClient.conf);
    if (retVal != 0)
    {
      lorLog(" failed! mbedtls_ssl_setup returned %d", retVal);
      goto epilogue;
    }

    if (!isServer)
    {
      retVal = mbedtls_ssl_set_hostname(&pSocket->tlsClient.ssl, pAddress);
      if (retVal != 0)
      {
        lorLog(" failed! mbedtls_ssl_set_hostname returned %d", retVal);
        goto epilogue;
      }

      mbedtls_ssl_set_bio(&pSocket->tlsClient.ssl, &pSocket->tlsClient.socketContext, mbedtls_net_send, mbedtls_net_recv, NULL);

      do
      {
        retVal = mbedtls_ssl_handshake(&pSocket->tlsClient.ssl);
        if (retVal == 0)
          break;

        if (retVal != MBEDTLS_ERR_SSL_WANT_READ && retVal != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
          lorLog(" failed! mbedtls_ssl_handshake returned -0x%x", -retVal);
          goto epilogue;
        }
      } while (retVal != 0);
    }
  }
  else
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

    pSocket->basicSocket = socket(pOutAddr->ai_family, pOutAddr->ai_socktype, pOutAddr->ai_protocol);

    if (!lorSocket_IsValidSocket(pSocket))
      goto epilogue;

    if (isServer)
    {
      pSocket->isServer = true;

      retVal = bind(pSocket->basicSocket, pOutAddr->ai_addr, (int)pOutAddr->ai_addrlen);
      if (retVal == SOCKET_ERROR)
        goto epilogue;

      retVal = listen(pSocket->basicSocket, SOMAXCONN);
      if (retVal == SOCKET_ERROR)
        goto epilogue;
    }
    else
    {
      retVal = connect(pSocket->basicSocket, pOutAddr->ai_addr, (int)pOutAddr->ai_addrlen);
      if (retVal == SOCKET_ERROR)
        goto epilogue;
    }
  }

  openSuccess = true;

epilogue:
  if (pOutAddr != nullptr)
    freeaddrinfo(pOutAddr);

  if (!openSuccess)
    lorSocket_Deinit(&pSocket);

  *ppSocket = pSocket;

  lorLog("\t...Connection %s!", openSuccess ? "Success" : "Failed");
  return openSuccess;
}

bool lorSocket_Deinit(lorSocket **ppSocket)
{
  if (*ppSocket == nullptr)
    return true;

  if ((*ppSocket)->isSecure)
  {
    lorSocket *pSocket = *ppSocket;
    mbedtls_net_free(&pSocket->tlsClient.socketContext);
    mbedtls_ssl_free(&pSocket->tlsClient.ssl);
    mbedtls_ssl_config_free(&pSocket->tlsClient.conf);
    mbedtls_ctr_drbg_free(&pSocket->tlsClient.ctr_drbg);

    mbedtls_x509_crt_free(&pSocket->tlsClient.certificateServer);
    mbedtls_pk_free(&pSocket->tlsClient.publicKey);
  }
  else
  {
#ifdef _MSC_VER
    closesocket((*ppSocket)->basicSocket);
#else
    close((*ppSocket)->basicSocket);
#endif
  }

  lorFree(*ppSocket);
  *ppSocket = nullptr;

  return true;
}

bool lorSocket_SendData(lorSocket *pSocket, const uint8_t *pBytes, uint16_t totalBytes)
{
  if (pSocket == nullptr || pBytes == nullptr || totalBytes == 0)
    return false;

  if (pSocket->isSecure)
  {
    int status = mbedtls_ssl_write(&pSocket->tlsClient.ssl, pBytes, totalBytes);

    if (status < 0)
      return false; //TODO: this is really important to close socket somehow

    return (totalBytes == status);
  }
  else
  {
    return (totalBytes == (int)send(pSocket->basicSocket, (const char*)pBytes, totalBytes, 0));
  }
}

int lorSocket_ReceiveData(lorSocket *pSocket, uint8_t *pBytes, uint16_t bufferSize, bool blockForever /*= false*/)
{
  if (pSocket == nullptr || pBytes == nullptr || bufferSize == 0 || pSocket->isServer)
    return 0;

  if (blockForever)
  {
    if (pSocket->isSecure)
      return mbedtls_ssl_read(&pSocket->tlsClient.ssl, pBytes, bufferSize);
    else
      return (int)recv(pSocket->basicSocket, (char*)pBytes, bufferSize, 0);
  }
  else
  {
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 0;
    tv.tv_usec = 1; //0 was wait forever?

    FD_ZERO(&readfds);

    if (pSocket->isSecure)
      FD_SET((SOCKET)pSocket->tlsClient.socketContext.fd, &readfds);
    else
      FD_SET(pSocket->basicSocket, &readfds);

    select(0, &readfds, nullptr, nullptr, &tv);

    if (pSocket->isSecure && FD_ISSET(pSocket->tlsClient.socketContext.fd, &readfds))
      return mbedtls_ssl_read(&pSocket->tlsClient.ssl, pBytes, bufferSize);
    else if(!pSocket->isSecure && FD_ISSET(pSocket->basicSocket, &readfds))
      return (int)recv(pSocket->basicSocket, (char*)pBytes, bufferSize, 0);
  }

  return 0;
}

bool lorSocket_ServerAcceptClient(lorSocket *pServerSocket, lorSocket **ppClientSocket, uint32_t *pIPv4Address /*= nullptr*/)
{
  if (ppClientSocket == nullptr || pServerSocket == nullptr || !pServerSocket->isServer)
    return false;

  *ppClientSocket = nullptr;

  struct timeval tv;
  fd_set readfds;

  tv.tv_sec = 0;
  tv.tv_usec = 1; //0 is wait forever?

  FD_ZERO(&readfds);

  if(pServerSocket->isSecure)
    FD_SET((SOCKET)pServerSocket->tlsClient.socketContext.fd, &readfds);
  else
    FD_SET(pServerSocket->basicSocket, &readfds);

  select(0, &readfds, nullptr, nullptr, &tv);

  uint8_t clientIP[64];
  size_t clientBytesReturned;
  if (pIPv4Address != nullptr)
    *pIPv4Address = 0;

  if (pServerSocket->isSecure && FD_ISSET(pServerSocket->tlsClient.socketContext.fd, &readfds))
  {
    mbedtls_net_context clientContext;

    //Accept the client
    int retVal = mbedtls_net_accept(&pServerSocket->tlsClient.socketContext, &clientContext, clientIP, sizeof(clientIP), &clientBytesReturned);
    if (retVal != 0)
    {
      lorLog("Failed somehow to accept tls client?");
      return false;
    }

    if(pIPv4Address != nullptr && clientBytesReturned == 4)
      *pIPv4Address = (clientIP[0] << 24) | (clientIP[1] << 16) | (clientIP[2] << 8) | (clientIP[3]);

    (*ppClientSocket) = lorAllocType(lorSocket, 1);
    lorSocket *pClientSocket = *ppClientSocket;

    pClientSocket->isSecure = true;
    pClientSocket->tlsClient.socketContext = clientContext;

    //Handle SSL
    /* Make sure memory references are valid */
    mbedtls_ssl_init(&pClientSocket->tlsClient.ssl);

    lorLog("  Setting up SSL/TLS data");

    //Copy the config?
    retVal = mbedtls_ssl_setup(&pClientSocket->tlsClient.ssl, &pServerSocket->tlsClient.conf);
    if (retVal != 0)
    {
      lorLog("  failed! mbedtls_ssl_setup returned -0x%04x", -retVal);
      goto sslfail;
    }

    mbedtls_ssl_set_bio(&pClientSocket->tlsClient.ssl, &pClientSocket->tlsClient.socketContext, mbedtls_net_send, mbedtls_net_recv, NULL);

    //Handshake

    do
    {
      retVal = mbedtls_ssl_handshake(&pClientSocket->tlsClient.ssl);
      if (retVal == 0)
        break;

      if (retVal != MBEDTLS_ERR_SSL_WANT_READ && retVal != MBEDTLS_ERR_SSL_WANT_WRITE)
      {
        lorLog(" failed! mbedtls_ssl_handshake returned -0x%x", -retVal);
        goto sslfail;
      }
    } while (retVal != 0);

    lorLog("  Client accepted and secured");
    return true;

  sslfail:
    //Cleanup

    return false;
  }
  else if (!pServerSocket->isSecure && FD_ISSET(pServerSocket->basicSocket, &readfds))
  {
    sockaddr_storage clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(pServerSocket->basicSocket, (sockaddr*)&clientAddr, &clientAddrSize);

    if (clientSocket != INVALID_SOCKET)
    {
      (*ppClientSocket) = lorAllocType(lorSocket, 1);
      (*ppClientSocket)->basicSocket = clientSocket;

      if (pIPv4Address != nullptr)
      {
        sockaddr_in *pAddrV4 = (sockaddr_in*)&clientAddr;
        memcpy(clientIP, &pAddrV4->sin_addr.s_addr, sizeof(pAddrV4->sin_addr.s_addr));
        *pIPv4Address = (clientIP[0] << 24) | (clientIP[1] << 16) | (clientIP[2] << 8) | (clientIP[3]);
      }

      return true;
    }
  }

  return false;
}
