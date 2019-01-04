#include "lorHTTP.h"
#include "lorPlatform.h"

#include <stdio.h>
#include "lorSocket.h"

bool lorHTTP_Request(const char **ppResponse, const char *pAddress, const char *pPayload /*= nullptr*/)
{
  if (ppResponse == nullptr || pAddress == nullptr)
    return false;

  lorSocket *pSocket = nullptr;
  lorSocketConnectionFlags connFlags = lSCFNone;

  char addressBuffer[128];
  int portNumber = 0;
  int skipChars = 0;

  *ppResponse = nullptr;

  if (lorStrBeginsWith(pAddress, "http://"))
  {
    skipChars = 7;
    portNumber = 80;
  }
  else if (lorStrBeginsWith(pAddress, "https://"))
  {
    skipChars = 8;
    connFlags = (connFlags | lSCFUseTLS);
    portNumber = 443;
  }

  const char *pRelPath = lorStrchr(&pAddress[skipChars], '/');
  const char *pPortStr = lorStrchr(&pAddress[skipChars], ':');

  // Get port number
  if (pPortStr != nullptr && ((pPortStr < pRelPath) || pRelPath == nullptr))
    portNumber = lorStrAtoI(&pPortStr[1]);

  // Get address
  if (pPortStr != nullptr && ((pPortStr < pRelPath) || pRelPath == nullptr))
    lorStrncpy(addressBuffer, sizeof(addressBuffer), &pAddress[skipChars], pPortStr - pAddress - skipChars);
  else if (pRelPath != nullptr)
    lorStrncpy(addressBuffer, sizeof(addressBuffer), &pAddress[skipChars], pRelPath - pAddress - skipChars);
  else
    lorStrcpy(addressBuffer, sizeof(addressBuffer), &pAddress[skipChars]);

  if (pRelPath == nullptr)
    pRelPath = "/";

  if (lorSocket_Init(&pSocket, addressBuffer, portNumber, connFlags))
  {
    bool sent = true;

    const char *msgs[] = { "GET ", "POST ", " HTTP/1.0\r\nHost: ", "\r\nContent-Length: ", "\r\nConnection: close\r\n\r\n" };

    // Request:
    if (pPayload == nullptr)
      sent &= lorSocket_SendData(pSocket, (uint8_t*)msgs[0], (uint16_t)lorStrlen(msgs[0]));
    else
      sent &= lorSocket_SendData(pSocket, (uint8_t*)msgs[1], (uint16_t)lorStrlen(msgs[1]));
    sent &= lorSocket_SendData(pSocket, (uint8_t*)pRelPath, (uint16_t)lorStrlen(pRelPath));

    // Host
    sent &= lorSocket_SendData(pSocket, (uint8_t*)msgs[2], (uint16_t)lorStrlen(msgs[2]));
    sent &= lorSocket_SendData(pSocket, (uint8_t*)addressBuffer, (uint16_t)lorStrlen(addressBuffer));

    if (pPayload != nullptr)
    {
      char contentSize[8];
      lorSprintf(contentSize, sizeof(contentSize), "%zu", lorStrlen(pPayload));
      sent &= lorSocket_SendData(pSocket, (uint8_t*)msgs[3], (uint16_t)lorStrlen(msgs[3]));
      sent &= lorSocket_SendData(pSocket, (uint8_t*)contentSize, (uint16_t)lorStrlen(contentSize));
    }

    sent &= lorSocket_SendData(pSocket, (uint8_t*)msgs[4], (uint16_t)lorStrlen(msgs[4]));

    if (pPayload != nullptr)
      sent &= lorSocket_SendData(pSocket, (uint8_t*)pPayload, (uint16_t)lorStrlen(pPayload));

    // Read reply
    const uint16_t perReadAmt = 1024;
    size_t currentSize = perReadAmt * 4; // *4 to give a bit of a buffer before resizing
    char *pReply = lorAllocType(char, currentSize);
    size_t totalRead = 0;

    if (sent)
    {
      int currentRead = 0;

      int failsInARow = 0;

      do
      {
        currentRead = lorSocket_ReceiveData(pSocket, (uint8_t*)&pReply[totalRead], lorMin(perReadAmt, (uint16_t)(currentSize-totalRead)), true);
        if (currentRead > 0)
        {
          failsInARow = 0;
          totalRead += currentRead;

          const char *pData = strstr(pReply, "\r\n\r\n");

          if (pData != nullptr) // We have a payload
          {
            const char *pLengthStr = strstr(pReply, "Content-Length: ");

            if (pLengthStr)
            {
              int expectedLength = lorStrAtoI(&pLengthStr[16]);
              if (expectedLength > 0 && totalRead == (uint32_t)(expectedLength + (pData - pReply) + 4)) // +4 = "\r\n\r\n"
                break; // We've read everything
            }
          }

          // Resize the internal buffer
          if (totalRead + perReadAmt > currentSize)
          {
            char *pTempReadBuffer = lorAllocType(char, currentSize+(perReadAmt*2));
            if (pTempReadBuffer)
            {
              memcpy(pTempReadBuffer, pReply, currentSize);
              lorFree(pReply);
              pReply = pTempReadBuffer;
              currentSize = currentSize + (perReadAmt * 2);
            }
          }
        }
        else
        {
          ++failsInARow;
        }
      } while (currentRead >= 0 && currentSize-totalRead != 0 && failsInARow < 10);

      pReply[totalRead] = '\0';
      const char *pCpyPoint = strstr(pReply, "\r\n\r\n");
      if (pCpyPoint != nullptr)
        *ppResponse = lorStrdup(&pCpyPoint[4]);
    }

    lorFree(pReply);
  }

  lorSocket_Deinit(&pSocket);

  return (*ppResponse != nullptr);
}
