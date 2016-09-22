#include "lorUICore.h"

struct lorUIInputPoint
{
  //Current info
  uint8_t pointID;
  uint32_t x;
  uint32_t y;

  //Previous positions if this point is moving
  uint32_t previousX;
  uint32_t previousY;

  bool isDown;
};

struct lorUICore
{
  uint32_t windowWidth;
  uint32_t windowHeight;

  lorUIApp *pApp;
};

//Control
void lorUICore_Init(lorUICore **ppUI, uint32_t screenWidth, uint32_t screenHeight)
{
  lorUICore *pUI = lorAllocType(lorUICore, 1);

  pUI->windowWidth = screenWidth;
  pUI->windowHeight = screenHeight;

  *ppUI = pUI;
}

void lorUICore_Deinit(lorUICore **ppUI)
{
  lorFree(*ppUI);
  *ppUI = nullptr;
}

void lorUICore_ScreenResized(lorUICore *pUI, uint32_t width, uint32_t height)
{
  pUI->windowWidth = width;
  pUI->windowHeight = height;
}

void lorUICore_Update(lorUICore *pUI, float dt)
{
  //Doesn't do anything yet
}

void lorUICore_Render(lorUICore *pUI, lorGraphicsCore *pGraphics)
{

}

void lorUICore_RegisterGame(lorUICore *pUI, lorUIApp *pApp)
{
  pUI->pApp = pApp;
}

// Functions filtered to lorUICore

void lorUICore_PointDown(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY)
{
  //TODO: Process UI here


  //UI didn't use the tap so pass to the game
  if (pUI->pApp != nullptr)
  {

  }
}

void lorUICore_PointUp(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY)
{
  //TODO: Process UI here


  //UI didn't use the tap so pass to the game
  if (pUI->pApp != nullptr)
  {
    int worldX, worldY;
    lorPoint screenPos;
    screenPos.x = (float)pointX;
    screenPos.y = (float)pointY;
  }
}

void lorUICore_PointMoved(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY)
{
  //TODO: Process UI here


  //UI didn't use the motion so pass to the game
  if (pUI->pApp != nullptr)
  {

  }
}

void lorUICore_PointTap(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY)
{
  //TODO: Process UI here


  //UI didn't use the tap so pass to the game
  if (pUI->pApp != nullptr)
  {

  }
}

void lorUICore_PointDoubleTap(lorUICore *pUI, uint8_t pointID, uint32_t pointX, uint32_t pointY)
{
  //TODO: Process UI here


  //UI didn't use the tap so pass to the game
  if (pUI->pApp != nullptr)
  {

  }
}
