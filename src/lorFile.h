#ifndef LOR_FILE
#define LOR_FILE

#include "lorMath.h"

struct lorFile;

enum lorFileMode
{
  lorFM_Read,
  lorFM_Write
};

uint8_t* lorFile_LoadAssetFile(const char *pFilename, size_t *pNumBytes = nullptr); //Requires lorFree after; returns nullptr on failure

bool lorFile_OpenAssetFile(lorFile **ppFile, const char *pFilename, lorFileMode fileMode, size_t *pFileSize = nullptr);
bool lorFile_OpenUserFile(lorFile **ppFile, const char *pFilename, lorFileMode fileMode, size_t *pFileSize = nullptr, const char *pAppName = "temp");
void lorFile_CloseFile(lorFile **ppFile);

size_t lorFile_Read(lorFile *pFile, void *pBuffer, size_t numBytes); //pBuffer must be large enough, returns actual bytes read
size_t lorFile_Write(lorFile *pFile, void *pBuffer, size_t numBytes); //Returns actual bytes written

#endif //LOR_SETTINGS
