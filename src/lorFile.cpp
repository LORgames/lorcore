#include "lorFile.h"
#include "lorPlatform.h"

#include "SDL2/SDL.h"

uint8_t* lorFile_LoadAssetFile(const char *pFilename)
{
  SDL_RWops *file = SDL_RWFromFile(pFilename, "r");
  if (!file)
  {
    lorLog("Failed to load: %s", pFilename);
    return false;
  }

  int64_t filesize = SDL_RWsize(file);
  if (filesize < 0)
    filesize = 1024 * 1024 * 4; // 4MB

  uint8_t *pDataBuffer = lorAllocType(unsigned char, filesize);
  size_t n_blocks = SDL_RWread(file, pDataBuffer, 1, filesize);
  SDL_RWclose(file);

  if (n_blocks < 0)
  {
    lorLog("Failed to read: %s", pFilename);
    lorFree(pDataBuffer)
  }

  return pDataBuffer;
}
