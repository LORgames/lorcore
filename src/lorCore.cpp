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

  lorUIApp *pApp;

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
bool lorInit(uint32_t flags)
{
  lorLog("Starting client...");

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    lorLog("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  uint32_t windowFlags = SDL_WINDOW_SHOWN;
  SDL_DisplayMode mode;
  int windowWidth = DefaultWindowWidth;
  int windowHeight = DefaultWindowHeight;

  Uint32 now = SDL_GetTicks();
  Uint32 nextTime = now + GlobalFrameMS;

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
  gWindow = SDL_CreateWindow("LORgames Castle Rush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, windowFlags);

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
  SDL_GetWindowSize(gWindow, &pCore->windowWidth, &pCore->windowHeight);

  lorSettings_SetDefaults(&pCore->Settings);
  lorGraphicsCore_Init(&pCore->pGLCore, gWindow, &pCore->Settings);
  lorAudio_Init(&pCore->pAudioCore);

  lorUICore_Init(&pCore->pUI, pCore->windowWidth, pCore->windowHeight);

  isRunning = true;

  lorUICore_RegisterGame(pCore->pUI, pCore->pApp);

  //Ping the server
  //lorSocket *pSocket;
  //if (!lorSocket_Connect(&pSocket, "220.237.74.59", 5659))
  //  lorSocket_Connect(&pSocket, "192.168.0.21", 5659);
  //lorSocket_Close(&pSocket);

  while (isRunning)
  {
    lorProcessMessageQueue(pCore);

    //Sort out sleep time
    now = SDL_GetTicks();
    if (now < nextTime)
      SDL_Delay(nextTime - now);
    nextTime += GlobalFrameMS;

    //UPDATE
    pCore->pApp->Step(GlobalFrameMS / 1000.f);

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
