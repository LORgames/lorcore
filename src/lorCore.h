#ifndef LOR_CORE
#define LOR_CORE

#ifdef IOS
#define nullptr NULL
#endif

#include <stdio.h>
#include <cstring>
#include "lorMath.h"

#include "lorMemory.h"
#include "lorString.h"

#ifndef ASSETDIR
#define ASSETDIR "../../Assets/"
#endif

#if ANDROID
#include <android\log.h>
#define PRId64 "lld"
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

typedef void (lorAppResized)(void *pAppData, int newWidth, int newHeight);

struct lorAppSettings
{
  int Width;
  int Height;
  float FrameMilliseconds;

  lorAppResized *pResizedFunc;

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
#define lorLog(fmt, ...) do { printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#endif

#define lorAssert(check, fmt, ...) do { if(!(check)) { lorLog(fmt, ##__VA_ARGS__); lorBreak(); }} while(0)

bool lorInit(lorCore **ppCore, lorAppSettings *pAppSettings, uint32_t flags); //Startup the engine
bool lorUpdate(lorCore *pCore, lorGraphicsCore **ppGL); // Update the engine, returns true if should keep running
bool lorExit(lorCore **ppCore); // Cleanup the engine
bool lorSuspend(lorCore *pCore); //App is trying to minimize or go into low power mode

#endif //LOR_CORE
