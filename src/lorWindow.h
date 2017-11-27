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
struct lorAudioEngine;
struct lorWindow;

enum
{
  lorMouseCursor = 255
};

struct lorWindowEventData_Cursor
{
  int cursorID;
  lorVec2i screenPosition;
};

typedef void (lorAppResized)(void *pAppData, lorVec2i newSize);
typedef void (lorCursorEvent)(void *pAppdata, const lorWindowEventData_Cursor &cursorData);

struct lorWindowSettings
{
  int Width;
  int Height;
  float FrameMilliseconds;

  lorAppResized *pResizedFunc;

  lorCursorEvent *pCursorMoved;
  lorCursorEvent *pCursorDown;
  lorCursorEvent *pCursorUp;

  const char *pName;
  void *pAppData;

  //Out Values
  lorGraphicsCore *pGL;
  lorAudioEngine *pAudio;
};

bool lorWindow_Init(lorWindow **ppCore, lorWindowSettings *pAppSettings, uint32_t flags); //Startup the engine
bool lorWindow_Update(lorWindow *pCore); // Update the engine, returns true if should keep running
bool lorWindow_Exit(lorWindow **ppCore); // Cleanup the engine

#endif //lorWindow_h__
