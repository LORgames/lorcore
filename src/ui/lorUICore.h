#ifndef LOR_UICORE
#define LOR_UICORE

#include "lorMath.h"
#include "gl/lorGraphicsCore.h"

struct lorUICore;

//Control
void lorUICore_Init(lorUICore **ppUI, uint32_t screenWidth, uint32_t screenHeight);
void lorUICore_Deinit(lorUICore **ppUI);

void lorUICore_ScreenResized(lorUICore *pUI, uint32_t width, uint32_t height);
void lorUICore_Update(lorUICore *pUI, float dt);  //UI actually has dt
void lorUICore_Render(lorUICore *pUI, lorGraphicsCore *pGraphics);

// Functions filtered to lorUICore
void lorUICore_PointDown(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY);
void lorUICore_PointUp(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY);
void lorUICore_PointMoved(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY);

void lorUICore_PointTap(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY);
void lorUICore_PointDoubleTap(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY);

#endif //LOR_UICORE
