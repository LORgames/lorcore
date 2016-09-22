#ifndef LOR_GRAPHICSCORE
#define LOR_GRAPHICSCORE

#include "lorCore.h"
#include "SDL2\SDL.h"

struct lorGraphicsCore;
struct lorLineDrawer;
struct lorSettings;

bool lorGraphicsCore_Init(lorGraphicsCore **ppGraphicsCore, SDL_Window *pWindow, lorSettings *pSettings);
bool lorGraphicsCore_Destroy(lorGraphicsCore **ppGraphicsCore);

void lorGraphicsCore_Resize(lorGraphicsCore *pGL, int width, int height);

void lorGraphicsCore_StartFrame(lorGraphicsCore *pGraphicsCore);
void lorGraphicsCore_EndFrame(lorGraphicsCore *pGraphicsCore);

lorLineDrawer* lorGraphicsCore_GetLineRenderer(lorGraphicsCore *pGraphicsCore);

void lorGraphicsCore_TakeScreenshot(lorGraphicsCore *pGraphicsCore);

#endif // LOR_GRAPHICSCORE
