#include "lorAudio.h"
#include "lorCore.h"

struct lorAudioEngine
{

};

struct lorAudioObject
{

};

void lorAudio_Init(lorAudioEngine **ppAudioEngine)
{
  *ppAudioEngine = nullptr;

  *ppAudioEngine = lorAllocType(lorAudioEngine, 1);
}

void lorAudio_Deinit(lorAudioEngine **ppAudioEngine)
{
  lorFree(*ppAudioEngine);
  *ppAudioEngine = nullptr;
}

bool lorAudio_PlaySound(lorAudioEngine * /*pAudioEngine*/, lorAudioObject ** /*ppAudioObject*/, const char * /*pFilename*/)
{
  return false;
}

bool lorAudio_PlaySoundOnce(lorAudioEngine * /*pAudioEngine*/, const char * /*pFilename*/)
{
  return false;
}

void lorAudio_CleanupSound(lorAudioObject **ppAudioObject)
{
  if(ppAudioObject != nullptr && *ppAudioObject != nullptr)
    lorFree(*ppAudioObject);
}
