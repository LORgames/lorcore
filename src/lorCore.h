#ifndef LOR_CORE
#define LOR_CORE

#include "lorPlatform.h"

#if LORPLATFORM_IOS
# define nullptr NULL
#endif

#include <stdio.h>
#include <cstring>
#include "lorMath.h"

#include "lorMemory.h"
#include "lorString.h"

#ifndef ASSETDIR
# define ASSETDIR "../../Assets/"
#endif

#if LORPLATFORM_ANDROID
# include <android\log.h>
# ifndef PRId64 // This can be defined on 64bit Android
#   define PRId64 "lld"
# endif
#endif

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


#if defined(_MSC_VER) && defined(lorLogToFile)
#include "lorFile.h"
#define lorBreak() __debugbreak()
#define lorLog(fmt, ...) do { FILE *pFile = lorFile_OpenUserFile("game.log", "a"); fprintf_s(pFile, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); fclose(pFile); } while(0)
#elif defined(lorLogToFile)
#error Unsupported Platform for lorLogToFile!
#elif defined(_MSC_VER)
#define lorBreak() __debugbreak()
#define lorLog(fmt, ...) do { printf_s("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#else
#include <signal.h>
#define lorBreak() raise(SIGTRAP);
#if LORPLATFORM_ANDROID
#define lorLog(fmt, ...) do { __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#else //!LORPLATFORM_ANDROID
#define lorLog(fmt, ...) do { printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#endif //LORPLATFORM_ANDROID
#endif

#define lorAssert(check, fmt, ...) do { if(!(check)) { lorLog(fmt, ##__VA_ARGS__); lorBreak(); }} while(0)

bool lorInit(lorCore **ppCore, lorAppSettings *pAppSettings, uint32_t flags); //Startup the engine
bool lorUpdate(lorCore *pCore, lorGraphicsCore **ppGL); // Update the engine, returns true if should keep running
bool lorExit(lorCore **ppCore); // Cleanup the engine
bool lorSuspend(lorCore *pCore); //App is trying to minimize or go into low power mode

#endif //LOR_CORE
