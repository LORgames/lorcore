#include "lorCore.h"

#include "gl/lorGraphicsCore.h"
#include "comms/lorSocket.h"
#include "ui/lorUICore.h"
#include "lorSettings.h"
#include "lorAudio.h"
#include "lorHTTP.h"

#include "SDL2\SDL.h"

struct lorCore
{
  int32_t windowWidth;
  int32_t windowHeight;

  lorGraphicsCore *pGLCore;
  lorAudioEngine *pAudioCore;
  lorUICore *pUI;

  lorApp *pApp;

  lorSettings Settings;
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
bool isRunning = false;
SDL_Event event;  // Our event handle

void lorProcessMessageQueue(lorCore *pCore)
{
  SDL_Event *pEvent = &event;

  while (SDL_PollEvent(pEvent))
  {
    if (pEvent->type == SDL_WINDOWEVENT)
    {
      switch (pEvent->window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        pCore->windowWidth = pEvent->window.data1;
        pCore->windowHeight = pEvent->window.data2;

        lorGraphicsCore_Resize(pCore->pGLCore, pCore->windowWidth, pCore->windowHeight);
        //lorGame_Resized(pCore->pGame, pCore->windowWidth, pCore->windowHeight);
        lorUICore_ScreenResized(pCore->pUI, pCore->windowWidth, pCore->windowHeight);

        lorLog("Window %d resized to %dx%d\n", pEvent->window.windowID, pEvent->window.data1, pEvent->window.data2);
        break;
      }
    }
    else
    {
      switch (pEvent->type)
      {
      case SDL_KEYDOWN:
        //lorLog("Key press detected (%i)", pEvent->key.keysym.scancode);
        break;
      case SDL_KEYUP:
        //lorLog("Key release detected (%i)", pEvent->key.keysym.scancode);
        if (pEvent->key.keysym.sym == SDLK_ESCAPE)
          isRunning = false;
        else if (pEvent->key.keysym.sym == SDLK_PRINTSCREEN)
          lorGraphicsCore_TakeScreenshot(pCore->pGLCore);

        break;
      case SDL_MOUSEMOTION:
        lorUICore_PointMoved(pCore->pUI, 255, pEvent->motion.x, pEvent->motion.y);
        //lorLog("Mouse moved (%d, %d)", pEvent->motion.x, pEvent->motion.y);
        break;
      case SDL_MOUSEBUTTONDOWN:
        lorUICore_PointDown(pCore->pUI, 255, pEvent->motion.x, pEvent->motion.y);
        //lorLog("Mouse button pressed (x=%d, y=%d, button=%d)", pEvent->motion.x, pEvent->motion.y, pEvent->button.button);
        break;
      case SDL_MOUSEBUTTONUP:
        lorUICore_PointUp(pCore->pUI, 255, pEvent->motion.x, pEvent->motion.y);
        //lorLog("Mouse button released (x=%d, y=%d, button=%d)", pEvent->motion.x, pEvent->motion.y, pEvent->button.button);
        break;
      case SDL_MOUSEWHEEL:
        //lorLog("Mouse scrolled (x=%d, y=%d, button=%d)", pEvent->wheel.x, pEvent->wheel.y, pEvent->wheel.direction);
        break;
      case SDL_FINGERDOWN:
        lorUICore_PointDown(pCore->pUI, (pEvent->tfinger.fingerId % 255), pEvent->motion.x, pEvent->motion.y);
        //lorLog("Finger (%" PRId64 ") touched: %5.5f, %5.5f, pressure: %5.5f", pEvent->tfinger.fingerId, pEvent->tfinger.dx, pEvent->tfinger.dy, pEvent->tfinger.pressure);
        break;
      case SDL_FINGERUP:
        lorUICore_PointUp(pCore->pUI, (pEvent->tfinger.fingerId % 255), pEvent->motion.x, pEvent->motion.y);
        //lorLog("Finger (%" PRId64 ") removed: %5.5f, %5.5f, pressure: %5.5f", pEvent->tfinger.fingerId, pEvent->tfinger.dx, pEvent->tfinger.dy, pEvent->tfinger.pressure);
        break;
      case SDL_FINGERMOTION:
        lorUICore_PointMoved(pCore->pUI, (pEvent->tfinger.fingerId % 255), pEvent->motion.x, pEvent->motion.y);
        //lorLog("Finger (%" PRId64 ") moved: %5.5f, %5.5f, pressure: %5.5f", pEvent->tfinger.fingerId, pEvent->tfinger.dx, pEvent->tfinger.dy, pEvent->tfinger.pressure);
        break;
      case SDL_QUIT:
        lorLog("Quit requested, quitting.\n");
        isRunning = false;
        break;
      }
    }
  }
}

//App has just been launched
bool lorInit(lorApp *pApp, uint32_t flags)
{
  lorLog("Starting client...");

  lorAssert(pApp != nullptr, "App cannot be nullptr!");
  lorAssert(pApp->Step != nullptr, "App Update cannot be nullptr!");
  lorAssert(pApp->Render != nullptr, "App Render cannot be nullptr!");

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    lorLog("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  uint32_t windowFlags = SDL_WINDOW_SHOWN;
  SDL_DisplayMode mode;
  int windowWidth = pApp->Width;
  int windowHeight = pApp->Height;

  uint32_t now = SDL_GetTicks();
  float nextTime = (float)now + pApp->FrameMilliseconds;

  if (flags & LOR_WF_FULLSCREEN)
    windowFlags |= SDL_WINDOW_FULLSCREEN;

  if ((flags & LOR_WF_NO_RESIZE) == 0)  //SDL has resizable but not, 'not resizable'
    windowFlags |= SDL_WINDOW_RESIZABLE;

  if ((flags & LOR_WF_USE_DEVICE_RES) == LOR_WF_USE_DEVICE_RES)
  {
    if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
      return false;

    windowWidth = mode.w;
    windowHeight = mode.h;
  }

  //Create window
  gWindow = SDL_CreateWindow(pApp->pName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, windowFlags);

  if (gWindow == NULL)
  {
    lorLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  if (!lorSocket_InitSystem())
  {
    lorLog("Could not start socket system!");
    return false;
  }

  lorAuth_Init();

  lorCore *pCore = lorAllocType(lorCore, 1);
  pCore->pApp = pApp;
  SDL_GetWindowSize(gWindow, &pCore->windowWidth, &pCore->windowHeight);

  lorSettings_SetDefaults(&pCore->Settings);
  lorGraphicsCore_Init(&pCore->pGLCore, gWindow, &pCore->Settings);
  lorAudio_Init(&pCore->pAudioCore);

  lorUICore_Init(&pCore->pUI, pCore->windowWidth, pCore->windowHeight);

  isRunning = true;

  while (isRunning)
  {
    lorProcessMessageQueue(pCore);

    //Sort out sleep time
    now = SDL_GetTicks();
    if (now < nextTime)
      SDL_Delay((uint32_t)(nextTime - now));
    nextTime += pApp->FrameMilliseconds;

    //UPDATE
    pCore->pApp->Step(pApp->FrameMilliseconds / 1000.f);

    //RENDER
    lorGraphicsCore_StartFrame(pCore->pGLCore);
    pCore->pApp->Render(pCore->pGLCore);
    lorGraphicsCore_EndFrame(pCore->pGLCore);
  }

  lorUICore_Deinit(&pCore->pUI);
  lorAudio_Deinit(&pCore->pAudioCore);
  lorGraphicsCore_Destroy(&pCore->pGLCore);
  lorFree(pCore);

  lorAuth_Close();

  lorExit();
  return true;
}

//App is trying to exit
bool lorExit()
{
  if (!lorSocket_DeinitSystem())
    return false;

  return true;
}

//App is trying to minimize or go into low power mode
bool lorSuspend()
{
  return false; //Did not successfully suspend
}
