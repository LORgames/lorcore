#ifndef LOR_GLINTERNAL
#define LOR_GLINTERNAL

//THIS HEADER TO ONLY BE INCLUDED FROM WITHIN THE /gl/ DIRECTORY!

#include "lorMath.h"
#include "lorWindow.h"
#include "lorSettings.h"
#include "lorLineDrawer.h"
#include "SDL2/SDL.h"

struct lorGraphicsCore
{
  int w, h; // Screen width and height

  lorSettings *pSettings;
  SDL_Renderer *pRenderer;

  lorLineDrawer *pLines;
};

struct lorTexture
{
  uint64_t totalInstances;
  char *pFilename;

  int w, h; //Width and height

#ifdef RENDERER_SDL
  SDL_Texture *pSDLTexture;
#endif
};

#endif // LOR_GLINTERNAL
