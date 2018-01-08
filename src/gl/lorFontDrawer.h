#ifndef lorFontDrawer_h__
#define lorFontDrawer_h__

#include "lorMath.h"
#include "lorGraphicsCore.h"

struct lorFontDrawer;
struct lorFile;

void lorFontDrawer_CreateFromTTF(lorFontDrawer **ppFont, uint8_t *pTTFdata, lorGraphicsCore *pGL, uint32_t maxCharacters = 2048);

void lorFontDrawer_Destroy(lorFontDrawer **ppFont);

void lorFontDrawer_AddString(lorFontDrawer *pFont, int x, int y, const char *pStr);
lorVec2 lorFontDrawer_MeasureString(lorFontDrawer *pFont, const char *pStr);

void lorFontDrawer_Render(lorFontDrawer *pFont, lorGraphicsCore *pGL, uint32_t colour = 0xFFFFFFFF);

#endif //lorFontDrawer_h__
