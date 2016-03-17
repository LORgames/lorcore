#ifndef LOR_AUDIO
#define LOR_AUDIO

struct lorAudioEngine;
struct lorAudioObject;

void lorAudio_Init(lorAudioEngine **ppAudioEngine);
void lorAudio_Deinit(lorAudioEngine **ppAudioEngine);

bool lorAudio_PlaySound(lorAudioEngine *pAudioEngine, lorAudioObject **ppAudioObject, const char *pFilename);
bool lorAudio_PlaySoundOnce(lorAudioEngine *pAudioEngine, const char *pFilename);

#endif //LOR_AUDIO
