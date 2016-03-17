#ifndef LOR_SETTINGS
#define LOR_SETTINGS

#include "lorMath.h"

struct lorSettings
{
  //Texture quality
  uint8_t textureQualityUnits;
  uint8_t textureQualityUI;
  uint8_t textureQualityBackground;
  uint8_t textureQualityFont;
};

void lorSettings_SetDefaults(lorSettings *pSettings);

#endif //LOR_SETTINGS
