#ifndef lorWindow_h__
#define lorWindow_h__

#include "lorPlatform.h"

enum lorWindowFlags
{
  lorWF_NONE = 0,
  lorWF_FULLSCREEN = 1 << 0,
  lorWF_USE_DEVICE_RES = 1 << 1,
  lorWF_NO_RESIZE = 1 << 2,
};

//These structs will be passed around a lot
struct lorGraphicsCore;
struct lorWindow;

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

bool lorWindow_Init(lorWindow **ppCore, lorAppSettings *pAppSettings, uint32_t flags); //Startup the engine
bool lorWindow_Update(lorWindow *pCore, lorGraphicsCore **ppGL); // Update the engine, returns true if should keep running
bool lorWindow_Exit(lorWindow **ppCore); // Cleanup the engine

#endif //lorApp_h__
