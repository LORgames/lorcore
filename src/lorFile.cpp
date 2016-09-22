#include "lorFile.h"
#include "lorCore.h"

#include "SDL2/SDL_filesystem.h"

FILE* lorFile_OpenUserFile(const char *pFilename, const char *pMode)
{
  char *pPath = SDL_GetPrefPath("LORgames", "CastleRushGame");
  char buffer[1024];

  FILE *pFile;

#if defined(_MSC_VER)
  sprintf_s(buffer, 1024, "%s%s", pPath, pFilename);
  fopen_s(&pFile, buffer, pMode);
#else
  snprintf(buffer, 1024, "%s%s", pPath, pFilename);
  pFile = fopen(buffer, pMode);
#endif

  return pFile;
}
