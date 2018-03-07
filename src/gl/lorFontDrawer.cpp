#include "lorFontDrawer.h"
#include "lorWindow.h"
#include "lorGL_Internal.h"
#include "lorFile.h"
#include "lorTexture.h"

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wall"
# pragma GCC diagnostic ignored "-Wextra"
# pragma GCC diagnostic ignored "-Wtype-limits"
# if !defined(__clang__)
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#  pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
# endif
#endif

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic pop
#endif

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

  float fontSize;
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

  (*ppFont)->fontSize = 30;
  (*ppFont)->pChars = lorAllocType(stbtt_packedchar, 223);

  stbtt_PackBegin(&context, pPixels, ImageSize, ImageSize, 0, 1, nullptr);
  stbtt_PackFontRange(&context, pTTFdata, 0, (*ppFont)->fontSize, 32, 223, (*ppFont)->pChars);
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

lorVec2 lorFontDrawer_MeasureString(lorFontDrawer *pFont, const char *pStr)
{
  lorVec2 size = lorVec2::Zero();

  if (pFont == nullptr || pStr == nullptr || pFont->maxCharacters == pFont->usedCharacters)
    return size;

  stbtt_aligned_quad tempQuad;

  while (*pStr != '\0')
  {
    stbtt_GetPackedQuad(pFont->pChars, ImageSize, ImageSize, (*pStr) - 32, &size.x, &size.y, &tempQuad, false);
    ++pStr;
  }

  size.y += pFont->fontSize * 2 / 3;

  return size;
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
