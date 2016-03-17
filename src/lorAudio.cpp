#include "lorAudio.h"
#include "lorCore.h"

#include "SDL2_mixer/SDL_mixer.h"

struct lorAudioEngine
{

};

struct lorAudioObject
{
  Mix_Chunk *pChunk;
};

void lorAudio_Init(lorAudioEngine **ppAudioEngine)
{
  *ppAudioEngine = nullptr;

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    return;

  if (Mix_Init(MIX_INIT_OGG) == -1)
    return;

  *ppAudioEngine = lorAllocType(lorAudioEngine, 1);
}

void lorAudio_Deinit(lorAudioEngine **ppAudioEngine)
{
  Mix_Quit();

  lorFree(*ppAudioEngine);
  *ppAudioEngine = nullptr;
}

bool lorAudio_PlaySound(lorAudioEngine *pAudioEngine, lorAudioObject **ppAudioObject, const char *pFilename)
{
  *ppAudioObject = lorAllocType(lorAudioObject, 1);
  lorAudioObject *pAudioObject = *ppAudioObject;

  pAudioObject->pChunk = Mix_LoadWAV(pFilename);
  Mix_PlayChannel(-1, pAudioObject->pChunk, 0);

  return false;
}

bool lorAudio_PlaySoundOnce(lorAudioEngine *pAudioEngine, const char *pFilename)
{
  Mix_Chunk *pChunk = Mix_LoadWAV(pFilename);
  Mix_PlayChannel(-1, pChunk, 0);

  return false;
}

void lorAudio_CleanupSound(lorAudioObject **ppAudioObject)
{
  Mix_FreeChunk((*ppAudioObject)->pChunk);
  lorFree(*ppAudioObject);
}
