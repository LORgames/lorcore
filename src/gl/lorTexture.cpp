#include "lorTexture.h"
#include "lorGL_Internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "lorArray.h"
#include <stdio.h>

typedef lorArray<lorTexture*, 64> lorTextureCache;
static lorTextureCache textureCache;

void lorTexture_InitTextureCache()
{
  textureCache.Init();
}

void lorTexture_DestroyTextureCache()
{
  while (textureCache.length > 0)
  {
    lorTexture_Free(textureCache[0]);
  }

  textureCache.Deinit();
}

bool _TryLoadTexture(lorTexture *pTexture, const char *filename, lorGraphicsCore *pGL)
{
  lorLog("Loading texture \"%s\"", filename);

  bool retVal = false;
  int comp;

  SDL_Surface *pSDLSurface;

#if ANDROID
  SDL_RWops *file = SDL_RWFromFile(filename, "r");
  if (!file)
  {
    lorLog("Failed to load: %s", filename);
    return false;
  }

  int64_t filesize = SDL_RWsize(file);
  if (filesize < 0)
    filesize = 1024 * 1024 * 4; // 4MB

  unsigned char *dataBuffer = lorAllocType(unsigned char, filesize);
  int n_blocks = SDL_RWread(file, dataBuffer, 1, filesize);
  SDL_RWclose(file);
  if (n_blocks < 0)
    lorLog("Failed to read: %s", filename);
  uint8_t *data = stbi_load_from_memory(dataBuffer, n_blocks * 4, &pTexture->w, &pTexture->h, &comp, 0);
  lorFree(dataBuffer);
#else
  uint8_t *data = stbi_load(filename, &pTexture->w, &pTexture->h, &comp, 0);
#endif

  uint32_t rmask, gmask, bmask, amask;

  if (data == nullptr)
    goto epilogue;

  lorAssert(comp == 3 || comp == 4, "Channels is not a supported value!");

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  if (comp == 4)
    pSDLSurface = SDL_CreateRGBSurface(0, pTexture->w, pTexture->h, 32, rmask, gmask, bmask, amask);
  else if (comp == 3)
    pSDLSurface = SDL_CreateRGBSurface(0, pTexture->w, pTexture->h, 24, rmask, gmask, bmask, 0);
  else
    goto epilogue;

  memcpy(pSDLSurface->pixels, data, comp * pTexture->w * pTexture->h);

  //Create texture from surface pixels
  pTexture->pSDLTexture = SDL_CreateTextureFromSurface(pGL->pRenderer, pSDLSurface);
  lorAssert(pTexture->pSDLTexture != nullptr, "Unable to create texture from %s! SDL Error: %s\n", filename, SDL_GetError());

  //Get rid of old loaded surface
  SDL_FreeSurface(pSDLSurface);

  retVal = true;

epilogue:
  if(data != nullptr)
    stbi_image_free(data);

  return retVal;
}

lorTexture* lorTexture_Load(const char *filename, lorTextureType type, lorGraphicsCore *pGL)
{
  //Try find the texture in cache
  for (uint32_t i = 0; i < textureCache.length; i++)
  {
    if (lorStrcmp(filename, textureCache[i]->pFilename) == 0)
    {
      ++textureCache[i]->totalInstances;
      return textureCache[i];
    }
  }

  //Its not in cache, lets load one
  lorTexture *pRetTex = lorAllocType(lorTexture, 1);
  pRetTex->totalInstances = 1;
#ifdef _MSC_VER
  pRetTex->pFilename = _strdup(filename);
#else
  pRetTex->pFilename = strdup(filename);
#endif
  pRetTex->pSDLTexture = nullptr;

  uint8_t startingScale = 1;
  bool loaded = false;

  size_t filenameLen = lorStrlen(filename);
  char *pFilenameBuffer = lorAllocType(char, filenameLen + 3);
  char itoaBuf[2];

  switch (type)
  {
    case lorTextureType_UI:
      startingScale = pGL->pSettings->textureQualityUI;
      break;
    case lorTextureType_Unit:
      startingScale = pGL->pSettings->textureQualityUnits;
      break;
    case lorTextureType_Background:
      startingScale = pGL->pSettings->textureQualityBackground;
      break;
    case lorTextureType_Font:
      startingScale = pGL->pSettings->textureQualityFont;
      break;
  }

  while (startingScale > 1)
  {
    //This changes the filename to the correct @x style
    lorStrcpy(pFilenameBuffer, filenameLen + 3, filename);

    for (int i = 0; i < 5; ++i)
    {
      pFilenameBuffer[filenameLen - 2 + i] = filename[filenameLen - 4 + i];
    }

#ifdef _MSC_VER
    _itoa_s(startingScale, itoaBuf, 2, 10);
#else
    snprintf(itoaBuf, 2, "%d", startingScale);
#endif

    pFilenameBuffer[filenameLen - 4] = '@';
    pFilenameBuffer[filenameLen - 3] = itoaBuf[0];

    loaded = _TryLoadTexture(pRetTex, pFilenameBuffer, pGL);

    if (loaded)
      break;
    else
      startingScale = (uint8_t)(startingScale >> 1);
  }

  lorFree(pFilenameBuffer);

  if (!loaded)
    loaded = _TryLoadTexture(pRetTex, filename, pGL);

  if (loaded)
    *textureCache.PushBack() = pRetTex;

  return pRetTex;
}

void lorTexture_Free(lorTexture *pTexture)
{
  --pTexture->totalInstances;

  if (pTexture->totalInstances == 0)
  {
    SDL_DestroyTexture(pTexture->pSDLTexture);
    lorFree(pTexture->pFilename);
    lorFree(pTexture);

    for (uint32_t i = 0; i < textureCache.length; i++)
    {
      if (textureCache[i] == pTexture)
      {
        textureCache.RemoveSwapLast(i);
        break;
      }
    }
  }
}

void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, lorRect *pDestRect, lorRect *pSrcRect /*= nullptr*/, lorTextureFlipMode flipMode /*= lorFlipMode_None*/)
{
  lorTexure_BlitToScreen(pTexture, pGL, pDestRect->x, pDestRect->y, pDestRect->w, pDestRect->h, pSrcRect, flipMode);
}

void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, float x, float y, lorRect *pSrcRect /*= nullptr*/, lorTextureFlipMode flipMode /*= lorFlipMode_None*/)
{
  SDL_Rect _dst;
  _dst.x = (int)x;
  _dst.y = (int)y;

  if (pSrcRect != nullptr)
  {
    SDL_Rect _src;
    _src.x = int(pSrcRect->x * (float)pTexture->w);
    _src.y = int(pSrcRect->y * (float)pTexture->h);
    _src.w = int(pSrcRect->w * (float)pTexture->w);
    _src.h = int(pSrcRect->h * (float)pTexture->h);

    _dst.w = _src.w;
    _dst.h = _src.h;

    SDL_RenderCopyEx(pGL->pRenderer, pTexture->pSDLTexture, &_src, &_dst, 0, nullptr, (SDL_RendererFlip)flipMode);
  }
  else
  {
    _dst.w = pTexture->w;
    _dst.h = pTexture->h;

    SDL_RenderCopyEx(pGL->pRenderer, pTexture->pSDLTexture, nullptr, &_dst, 0, nullptr, (SDL_RendererFlip)flipMode);
  }
}

void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, float x, float y, float w, float h, lorRect *pSrcRect /*= nullptr*/, lorTextureFlipMode flipMode /*= lorFlipMode_None*/)
{
  lorAssert(pTexture != nullptr, "pTexture is a nullptr!");

  SDL_Rect _dst;
  _dst.x = (int)x;
  _dst.y = (int)y;
  _dst.w = (int)w;
  _dst.h = (int)h;

  if (pSrcRect != nullptr)
  {
    SDL_Rect _src;
    _src.x = int(pSrcRect->x * (float)pTexture->w);
    _src.y = int(pSrcRect->y * (float)pTexture->h);
    _src.w = int(pSrcRect->w * (float)pTexture->w);
    _src.h = int(pSrcRect->h * (float)pTexture->h);

    SDL_RenderCopyEx(pGL->pRenderer, pTexture->pSDLTexture, &_src, &_dst, 0, nullptr, (SDL_RendererFlip)flipMode);
  }
  else
  {
    SDL_RenderCopyEx(pGL->pRenderer, pTexture->pSDLTexture, nullptr, &_dst, 0, nullptr, (SDL_RendererFlip)flipMode);
  }
}
