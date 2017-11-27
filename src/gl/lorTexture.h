#ifndef LOR_TEXTURE
#define LOR_TEXTURE

#include "lorMath.h"
#include "lorShapes.h"
#include "gl/lorGraphicsCore.h"

enum lorTextureType
{
  lorTextureType_UI,
  lorTextureType_Unit,
  lorTextureType_Background,
  lorTextureType_Font,
};

enum lorTextureFlipMode
{
  lorFlipMode_None = 0,
  lorFlipMode_Horizontal = 1,
  lorFlipMode_Vertical = 2
};

struct lorTexture;

void lorTexture_InitTextureCache();
void lorTexture_DestroyTextureCache();

lorTexture* lorTexture_Load(const char *filename, lorTextureType type, lorGraphicsCore *pGL);
void lorTexture_Free(lorTexture *pTexture);

void lorTexture_IncrCacheInstance(lorTexture *pTexture);
void lorTexture_DecrCacheInstance(lorTexture *pTexture);

void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, lorRect *pDestRect, lorRect *pSrcRect = nullptr, lorTextureFlipMode flipMode = lorFlipMode_None);
void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, float x, float y, lorRect *pSrcRect = nullptr, lorTextureFlipMode flipMode = lorFlipMode_None);
void lorTexure_BlitToScreen(lorTexture *pTexture, lorGraphicsCore *pGL, float x, float y, float w, float h, lorRect *pSrcRect = nullptr, lorTextureFlipMode flipMode = lorFlipMode_None);

#endif // LOR_TEXTURE
