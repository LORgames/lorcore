#include "lorGraphicsCore.h"
#include "lorGL_Internal.h"
#include "lorTexture.h"

bool lorGraphicsCore_Init(lorGraphicsCore **ppCore, SDL_Window *pWindow, lorSettings *pSettings)
{
  *ppCore = lorAllocType(lorGraphicsCore, 1);
  (*ppCore)->pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

  SDL_GetWindowSize(pWindow, &(*ppCore)->w, &(*ppCore)->h);

  lorLineDrawer_Create(&(*ppCore)->pLines);
  lorTexture_InitTextureCache();

  (*ppCore)->pSettings = pSettings;

  return true;
}

bool lorGraphicsCore_Destroy(lorGraphicsCore **ppCore)
{
  lorLineDrawer_Destroy(&(*ppCore)->pLines);

  lorTexture_DestroyTextureCache();

  lorFree(*ppCore);
  *ppCore = nullptr;

  return true;
}

void lorGraphicsCore_Resize(lorGraphicsCore *pGL, int width, int height)
{
  pGL->w = width;
  pGL->h = height;
}

void lorGraphicsCore_StartFrame(lorGraphicsCore *pGraphicsCore)
{
  SDL_SetRenderDrawColor(pGraphicsCore->pRenderer, 255, 255, 255, 0xFF);

  //Clear screen
  SDL_RenderClear(pGraphicsCore->pRenderer);
}

void lorGraphicsCore_EndFrame(lorGraphicsCore *pGraphicsCore)
{
  //Render any queued lines
  lorLineDrawer_Render(pGraphicsCore->pLines, pGraphicsCore);

  //Swap buffers
  SDL_RenderPresent(pGraphicsCore->pRenderer);
}

lorLineDrawer* lorGraphicsCore_GetLineRenderer(lorGraphicsCore *pGraphicsCore)
{
  return pGraphicsCore->pLines;
}

void lorGraphicsCore_TakeScreenshot(lorGraphicsCore *pGraphicsCore)
{
  SDL_Surface *sshot = SDL_CreateRGBSurface(0, pGraphicsCore->w, pGraphicsCore->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(pGraphicsCore->pRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
  SDL_SaveBMP(sshot, "screenshot.bmp");
  SDL_FreeSurface(sshot);
}
