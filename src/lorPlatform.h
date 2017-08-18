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

#endif // lorPlatform__h_
