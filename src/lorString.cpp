#include "lorString.h"

//UTF8 decoder modified from http://www.json.org/JSON_checker/utf8_decode.c

// Get the 6-bit payload of the next continuation byte.
int lorStringUTF_GetContinuationByte(lorStringUTF8Context *pContext)
{
  int c = (pContext->pStr[pContext->currentIndex++] & 0xFF);
  return ((c & 0xC0) == 0x80) ? (c & 0x3F) : lorStringUTF8_Error;
}


// Initialize the UTF-8 decoder. The decoder is not reentrant
void lorStringUTF_StartDecode(lorStringUTF8Context *pContext, const char *pStr)
{
  pContext->pStr = pStr;
  pContext->strLength = lorStrlen(pStr);
  pContext->currentIndex = 0;
}

// Extract the next character. Can return lorStringUTF8_End or lorStringUTF8_Error
int lorStringUTF_DecodeNextCharacter(lorStringUTF8Context *pContext)
{
  int c[4]; // The return bytes
  int retVal;  /* the result */

  if (pContext->currentIndex >= pContext->strLength)
    return ((pContext->currentIndex == pContext->strLength) ? lorStringUTF8_End : lorStringUTF8_Error);

  c[0] = (pContext->pStr[pContext->currentIndex++] & 0xFF);

  if ((c[0] & 0x80) == 0)  // Zero continuation (0 to 127)
  {
    return c[0];
  }
  else if ((c[0] & 0xE0) == 0xC0) // One continuation (128 to 2047)
  {
    c[1] = lorStringUTF_GetContinuationByte(pContext);
    if (c[1] >= 0)
    {
      retVal = ((c[0] & 0x1F) << 6) | c[1];
      if (retVal >= 128)
        return retVal;
    }
  }
  else if ((c[0] & 0xF0) == 0xE0) // Two continuations (2048 to 55295 and 57344 to 65535)
  {
    c[1] = lorStringUTF_GetContinuationByte(pContext);
    c[2] = lorStringUTF_GetContinuationByte(pContext);
    if ((c[1] | c[2]) >= 0)
    {
      retVal = ((c[0] & 0x0F) << 12) | (c[1] << 6) | c[2];
      if (retVal >= 2048 && (retVal < 55296 || retVal > 57343))
        return retVal;
    }
  }
  else if ((c[0] & 0xF8) == 0xF0) // Three continuations (65536 to 1114111)
  {
    c[1] = lorStringUTF_GetContinuationByte(pContext);
    c[2] = lorStringUTF_GetContinuationByte(pContext);
    c[3] = lorStringUTF_GetContinuationByte(pContext);
    if ((c[1] | c[2] | c[3]) >= 0)
    {
      retVal = ((c[0] & 0x07) << 18) | (c[1] << 12) | (c[2] << 6) | c[3];
      if (retVal >= 65536 && retVal <= 1114111)
        return retVal;
    }
  }

  return lorStringUTF8_Error;
}
