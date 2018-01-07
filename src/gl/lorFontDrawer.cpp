#include "lorFontDrawer.h"
#include "lorWindow.h"
#include "lorGL_Internal.h"
#include "lorFile.h"
#include "lorTexture.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

enum
{
  ImageSize = 256
};

struct lorFontDrawer
{
  //Variables
  int32_t usedCharacters;
  int32_t maxCharacters;

  int32_t totalVertices;
  stbtt_aligned_quad *pVertices;

  bool hasChanged;

  stbtt_packedchar *pChars;
  lorTexture *pTexture;
};

void lorFontDrawer_CreateFromTTF(lorFontDrawer **ppFont, uint8_t *pTTFdata, lorGraphicsCore *pGL, uint32_t maxCharacters /*= 2048*/)
{
  *ppFont = lorAllocType(lorFontDrawer, 1);
  (*ppFont)->maxCharacters = maxCharacters;

  (*ppFont)->pVertices = lorAllocType(stbtt_aligned_quad, maxCharacters * 4);
  (*ppFont)->totalVertices = maxCharacters * 4;
  (*ppFont)->usedCharacters = 0;

  uint8_t *pPixels = lorAllocType(uint8_t, ImageSize * ImageSize);

  stbtt_pack_context context;

  (*ppFont)->pChars = lorAllocType(stbtt_packedchar, 223);

  stbtt_PackBegin(&context, pPixels, ImageSize, ImageSize, 0, 1, nullptr);
  stbtt_PackFontRange(&context, pTTFdata, 0, 30, 32, 223, (*ppFont)->pChars);
  stbtt_PackEnd(&context);

  uint32_t *p32Pixels = lorAllocType(uint32_t, ImageSize * ImageSize);
  for (int i = 0; i < ImageSize * ImageSize; ++i)
  {
    p32Pixels[i] = (pPixels[i] << 24) | 0xFFFFFF;
  }

  (*ppFont)->pTexture = lorTexture_CreateFromPtr(p32Pixels, ImageSize, ImageSize, 4, pGL);
}

void lorFontDrawer_Destroy(lorFontDrawer **ppFont)
{
  lorFree((*ppFont)->pVertices);
  lorFree(*ppFont);

  *ppFont = nullptr;
}

void lorFontDrawer_AddString(lorFontDrawer *pFont, int x, int y, const char *pStr)
{
  if (pFont == nullptr || pStr == nullptr || pFont->maxCharacters == pFont->usedCharacters)
    return;

  float xP = (float)x;
  float yP = (float)y;

  while (*pStr != '\0' && pFont->usedCharacters < pFont->maxCharacters)
  {
    stbtt_GetPackedQuad(pFont->pChars, ImageSize, ImageSize, (*pStr) - 32, &xP, &yP, &pFont->pVertices[pFont->usedCharacters], false);
    ++pFont->usedCharacters;
    ++pStr;
  }
}

void lorFontDrawer_Render(lorFontDrawer *pFont, lorGraphicsCore *pGL, uint32_t colour /*= 0xFFFFFFFF*/)
{
  lorRect src;
  lorRect dst;

  for (int i = 0; i < pFont->usedCharacters; ++i)
  {
    dst.x = pFont->pVertices[i].x0;
    dst.y = pFont->pVertices[i].y0;
    dst.w = pFont->pVertices[i].x1 - dst.x;
    dst.h = pFont->pVertices[i].y1 - dst.y;
    src.x = pFont->pVertices[i].s0;
    src.y = pFont->pVertices[i].t0;
    src.w = pFont->pVertices[i].s1 - src.x;
    src.h = pFont->pVertices[i].t1 - src.y;

    lorTexure_BlitToScreen(pFont->pTexture, pGL, &dst, &src, lorFlipMode_None, colour);
  }

  pFont->usedCharacters = 0;
}
