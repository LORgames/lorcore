#include "lorWindow.h"

#include "gl/lorGraphicsCore.h"
#include "comms/lorSocket.h"
#include "lorSettings.h"
#include "lorAudio.h"
#include "lorHTTP.h"

#include "SDL2/SDL.h"

struct lorCore
{
  SDL_Window* gWindow = NULL;
  SDL_Event event;  // Our event handle

  uint32_t now;
  float nextTime;

  bool isRunning;

  lorGraphicsCore *pGLCore;
  lorAudioEngine *pAudioCore;
  lorSettings Settings;
  lorAppSettings *pAppSettings;
};

void lorProcessMessageQueue(lorCore *pCore)
{
  SDL_Event *pEvent = &pCore->event;

  while (SDL_PollEvent(pEvent))
  {
    if (pEvent->type == SDL_WINDOWEVENT)
    {
      switch (pEvent->window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        pCore->pAppSettings->Width = pEvent->window.data1;
        pCore->pAppSettings->Height = pEvent->window.data2;

        lorGraphicsCore_Resize(pCore->pGLCore, pCore->pAppSettings->Width, pCore->pAppSettings->Height);

        if(pCore->pAppSettings->pResizedFunc)
          pCore->pAppSettings->pResizedFunc(pCore->pAppSettings->pAppData, pCore->pAppSettings->Width, pCore->pAppSettings->Height);

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
          pCore->isRunning = false;
        else if (pEvent->key.keysym.sym == SDLK_PRINTSCREEN)
          lorGraphicsCore_TakeScreenshot(pCore->pGLCore);

        break;
      case SDL_MOUSEMOTION:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, lorMouseCursor, pEvent->motion.x, pEvent->motion.y, lorCursorState_Moved);
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, lorMouseCursor, pEvent->button.x, pEvent->button.y, lorCursorState_Down);
        break;
      case SDL_MOUSEBUTTONUP:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, lorMouseCursor, pEvent->button.x, pEvent->button.y, lorCursorState_Up);
        break;
      case SDL_MOUSEWHEEL:
        //lorLog("Mouse scrolled (x=%d, y=%d, button=%d)", pEvent->wheel.x, pEvent->wheel.y, pEvent->wheel.direction);
        break;
      case SDL_FINGERDOWN:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, int(pEvent->tfinger.fingerId), int(pEvent->tfinger.x * pCore->pAppSettings->Width), int(pEvent->tfinger.y * pCore->pAppSettings->Height), lorCursorState_Down);
        break;
      case SDL_FINGERUP:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, int(pEvent->tfinger.fingerId), int(pEvent->tfinger.x * pCore->pAppSettings->Width), int(pEvent->tfinger.y * pCore->pAppSettings->Height), lorCursorState_Up);
        break;
      case SDL_FINGERMOTION:
        if (pCore->pAppSettings->pCursorEvent)
          pCore->pAppSettings->pCursorEvent(pCore->pAppSettings->pAppData, int(pEvent->tfinger.fingerId), int(pEvent->tfinger.x * pCore->pAppSettings->Width), int(pEvent->tfinger.y * pCore->pAppSettings->Height), lorCursorState_Moved);
        break;
      case SDL_QUIT:
        lorLog("Quit requested, quitting.\n");
        pCore->isRunning = false;
        break;
      }
    }
  }
}

//App has just been launched
bool lorInit(lorCore **ppCore, lorAppSettings *pAppSettings, uint32_t flags)
{
  lorLog("Starting client...");

  lorAssert(pAppSettings != nullptr, "App cannot be nullptr!");

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    lorLog("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    return false;
  }

  uint32_t windowFlags = SDL_WINDOW_SHOWN;
  SDL_DisplayMode mode;

  if (flags & LOR_WF_FULLSCREEN)
    windowFlags |= SDL_WINDOW_FULLSCREEN;

  if ((flags & LOR_WF_NO_RESIZE) == 0)  //SDL has resizable but not, 'not resizable'
    windowFlags |= SDL_WINDOW_RESIZABLE;

  if ((flags & LOR_WF_USE_DEVICE_RES) == LOR_WF_USE_DEVICE_RES)
  {
    if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
      return false;

    pAppSettings->Width = mode.w;
    pAppSettings->Height = mode.h;
  }

  //Create window
  SDL_Window *pWindow = SDL_CreateWindow(pAppSettings->pName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pAppSettings->Width, pAppSettings->Height, windowFlags);

  if (pWindow == NULL)
  {
    lorLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  if (!lorSocket_InitSystem())
  {
    lorLog("Could not start socket system!");
    return false;
  }

  //Create the core
  lorCore *pCore = lorAllocType(lorCore, 1);
  *ppCore = pCore;
  pCore->gWindow = pWindow;

  pCore->now = SDL_GetTicks();
  pCore->nextTime = (float)pCore->now + pAppSettings->FrameMilliseconds;

  lorAuth_Init();

  pCore->pAppSettings = pAppSettings;
  SDL_GetWindowSize(pCore->gWindow, &pAppSettings->Width, &pAppSettings->Height);

  lorSettings_SetDefaults(&pCore->Settings);
  lorGraphicsCore_Init(&pCore->pGLCore, pCore->gWindow, &pCore->Settings);
  lorAudio_Init(&pCore->pAudioCore);

  pCore->isRunning = true;

  //UPDATE
  return true;
}

// Update the engine, returns true if should keep running
bool lorUpdate(lorCore *pCore, lorGraphicsCore **ppGL)
{
  lorProcessMessageQueue(pCore);

  //Sort out sleep time
  pCore->now = SDL_GetTicks();
  if (pCore->now < pCore->nextTime)
    SDL_Delay((uint32_t)(pCore->nextTime - pCore->now));
  pCore->nextTime += pCore->pAppSettings->FrameMilliseconds;

  if (ppGL)
    *ppGL = pCore->pGLCore;

  return pCore->isRunning;
}

//App is trying to exit
bool lorExit(lorCore **ppCore)
{
  if (ppCore == nullptr || *ppCore == nullptr)
    return false;

  lorCore *pCore = *ppCore;
  *ppCore = nullptr;

  lorAudio_Deinit(&pCore->pAudioCore);
  lorGraphicsCore_Destroy(&pCore->pGLCore);
  lorFree(pCore);

  lorAuth_Close();

  if (!lorSocket_DeinitSystem())
    return false;

  return true;
}

//App is trying to minimize or go into low power mode
bool lorSuspend(lorCore * /*pCore*/)
{
  return false; //Did not successfully suspend
}
