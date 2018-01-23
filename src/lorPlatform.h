#ifndef lorPlatform__h_
#define lorPlatform__h_

// See https://sourceforge.net/p/predef/wiki/Home/ for small cheat sheet
// slightly outdated though.

#if defined(_MSC_VER )
# define WIN32_LEAN_AND_MEAN
# include "Windows.h"
# if WINAPI_PARTITION_DESKTOP
#  define LORPLATFORM_WINDOWS 1
# elif WINAPI_PARTITION_PC_APP
#  define LORPLATFORM_WINRT 1
# elif WINAPI_PARTITION_PHONE_APP
#  define LORPLATFORM_WINPHONE 1
# else
#  error "Unknown Windows platform"
# endif
#elif __APPLE__
# include "TargetConditionals.h"
# if TARGET_OS_IPHONE
#   define LORPLATFORM_IOS 1
#   if TARGET_IPHONE_SIMULATOR
#     define LORPLATFORM_IOS_SIMULATOR 1
#   endif
# elif TARGET_OS_MAC
#   define LORPLATFORM_OSX 1
# else
#   error "Unknown Apple platform"
# endif
#elif __ANDROID__
# define LORPLATFORM_ANDROID 1
#elif __linux__
# define LORPLATFORM_LINUX 1
#else
#   error "Unknown platform"
#endif

// Platform types
#define LORPLATFORM_MOBILE (LORPLATFORM_ANDROID || LORPLATFORM_IOS || LORPLATFORM_WINPHONE)
#define LORPLATFORM_DESKTOP (!LORPLATFORM_MOBILE)
#define LORPLATFORM_CONSOLE (0)

// Platform vendors
#define LORPLATFORM_MICROSOFT (LORPLATFORM_WINDOWS || LORPLATFORM_WINRT || LORPLATFORM_WINPHONE)
#define LORPLATFORM_APPLE (LORPLATFORM_OSX || LORPLATFORM_IOS)

// Determine if these are required
//#ifndef LORPLATFORM_WINDOWS
//# define LORPLATFORM_WINDOWS 0
//#endif
//#ifndef LORPLATFORM_LINUX
//# define LORPLATFORM_LINUX 0
//#endif
//#ifndef LORPLATFORM_OSX
//# define LORPLATFORM_OSX 0
//#endif
//#ifndef LORPLATFORM_ANDROID
//# define LORPLATFORM_ANDROID 0
//#endif
//#ifndef LORPLATFORM_IOS
//# define LORPLATFORM_IOS 0
//#endif
//#ifndef LORPLATFORM_IOS_SIMULATOR
//# define LORPLATFORM_IOS_SIMULATOR 0
//#endif
//#ifndef LORPLATFORM_WINRT
//# define LORPLATFORM_WINRT 0
//#endif
//#ifndef LORPLATFORM_WINPHONE
//# define LORPLATFORM_WINPHONE 0
//#endif

#if LORPLATFORM_IOS
# define nullptr NULL
#endif

#if LORPLATFORM_ANDROID
# include <android\log.h>
# ifndef PRId64 // This can be defined on 64bit Android
#   define PRId64 "lld"
# endif
#endif

#include <stdio.h>
#include <cstring>
#include "lorMath.h"

#include "lorMemory.h"
#include "lorString.h"

#if defined(_MSC_VER) && defined(lorLogToFile)
#include "lorFile.h"
#define lorBreak() __debugbreak()
#define lorLog(fmt, ...) do { lorFile *pFile; if(lorFile_OpenUserFile(&pFile, "lorLog.txt", lorFM_Read)){ char buffer[1024]; size_t len = lorSprintf(buffer, sizeof(buffer), "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); lorFile_Write(pFile, buffer, len); lorFile_CloseFile(&pFile); }} while(0)
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

#ifndef ASSETDIR
# define ASSETDIR "../../Assets/"
#endif

#endif // lorPlatform__h_
