#include "lorFile.h"
#include "lorPlatform.h"

#include "SDL2/SDL.h"

struct lorFile
{
  SDL_RWops *pSDLFile;
};

uint8_t* lorFile_LoadAssetFile(const char *pFilename, size_t *pNumBytes)
{
  SDL_RWops *file = SDL_RWFromFile(pFilename, "r");
  if (!file)
  {
    lorLog("Failed to load: %s", pFilename);
    return nullptr;
  }

  int64_t filesize = SDL_RWsize(file);
  if (filesize < 0)
    filesize = 1024 * 1024 * 4; // 4MB

  uint8_t *pDataBuffer = lorAllocType(uint8_t, (size_t)filesize);
  int64_t n_blocks = SDL_RWread(file, pDataBuffer, 1, (size_t)filesize);
  SDL_RWclose(file);

  if (pNumBytes != nullptr)
    *pNumBytes = (size_t)n_blocks;

  if (n_blocks < 0)
  {
    lorLog("Failed to read: %s", pFilename);
    lorFree(pDataBuffer)
  }

  return pDataBuffer;
}

bool lorFile_OpenAssetFile(lorFile **ppFile, const char *pFilename, lorFileMode fileMode, size_t *pFileSize /*= nullptr*/)
{
  SDL_RWops *pSDLFile;

  if (fileMode == lorFM_Read)
    pSDLFile = SDL_RWFromFile(pFilename, "rb");
  else if (fileMode == lorFM_Write)
    pSDLFile = SDL_RWFromFile(pFilename, "wb");
  else if (fileMode == lorFM_Append)
    pSDLFile = SDL_RWFromFile(pFilename, "ab");
  else
    return false;

  if (!pSDLFile)
    return false;

  if (pFileSize != nullptr)
    *pFileSize = (size_t)SDL_RWsize(pSDLFile);

  lorFile *pFile = lorAllocType(lorFile, 1);
  pFile->pSDLFile = pSDLFile;
  *ppFile = pFile;

  return true;
}

bool lorFile_OpenUserFile(lorFile **ppFile, const char *pFilename, lorFileMode fileMode, size_t *pFileSize /*= nullptr*/, const char *pAppName /*= "temp"*/)
{
  if (ppFile == nullptr || pFilename == nullptr)
    return false;

  *ppFile = nullptr;

  char fullPath[1024];
  lorSprintf(fullPath, sizeof(fullPath), "%s/%s", SDL_GetPrefPath("lorgames", pAppName), pFilename);

  return lorFile_OpenAssetFile(ppFile, fullPath, fileMode, pFileSize);
}

void lorFile_CloseFile(lorFile **ppFile)
{
  if (ppFile == nullptr || *ppFile == nullptr)
    return;

  SDL_RWclose((*ppFile)->pSDLFile);

  lorFree(*ppFile);
  *ppFile = nullptr;
}

size_t lorFile_Read(lorFile *pFile, void *pBuffer, size_t numBytes)
{
  if (pFile == nullptr || pBuffer == nullptr || numBytes == 0)
    return 0;

  return SDL_RWread(pFile->pSDLFile, pBuffer, 1, numBytes);
}

size_t lorFile_Write(lorFile *pFile, void *pBuffer, size_t numBytes)
{
  if (pFile == nullptr || pBuffer == nullptr || numBytes == 0)
    return 0;

  return SDL_RWwrite(pFile->pSDLFile, pBuffer, 1, numBytes);
}
