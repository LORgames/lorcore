#ifndef lorWindow_h__
#define lorWindow_h__

#include "lorPlatform.h"

enum LORWindowFlags
{
  LOR_WF_NONE = 0,
  LOR_WF_FULLSCREEN = 1 << 0,
  LOR_WF_USE_DEVICE_RES = 1 << 1,
  LOR_WF_NO_RESIZE = 1 << 2,
};

//These structs will be passed around a lot
struct lorGraphicsCore;
struct lorCore;

enum lorCursorState
{
  lorCursorState_Moved,
  lorCursorState_Down,
  lorCursorState_Up,
  lorCursorState_Pressed,
};

enum
{
  lorMouseCursor = 255
};

typedef void (lorAppResized)(void *pAppData, int newWidth, int newHeight);
typedef void (lorCursorEvent)(void *pAppdata, int cursorID, int x, int y, lorCursorState cursorState);

struct lorAppSettings
{
  int Width;
  int Height;
  float FrameMilliseconds;

  lorAppResized *pResizedFunc;
  lorCursorEvent *pCursorEvent;

  const char *pName;
  void *pAppData;
};

bool lorInit(lorCore **ppCore, lorAppSettings *pAppSettings, uint32_t flags); //Startup the engine
bool lorUpdate(lorCore *pCore, lorGraphicsCore **ppGL); // Update the engine, returns true if should keep running
bool lorExit(lorCore **ppCore); // Cleanup the engine
bool lorSuspend(lorCore *pCore); //App is trying to minimize or go into low power mode

#endif //lorApp_h__
