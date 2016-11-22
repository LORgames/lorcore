flags { "ExtraWarnings" }

local proj = project()
targetname(proj.name)
flags { "C++11", "NoPCH", "NoIncrementalLink", "FatalWarnings" }
exceptionhandling "Off"

-- Directories
objdir "intermediate/obj"
targetdir "builds/bin"
debugdir "builds/bin"

filter { "system:ios" }
  defines { "IOS" }
  targetdir "builds/bin_ios"
  debugdir "builds/bin_ios"
  links { "CoreAudio.framework", "AudioToolbox.framework", "CoreFoundation.framework", "CoreGraphics.framework", "Foundation.framework", "UIKit.framework", "OpenGLES.framework", "QuartzCore.framework" }

filter { "architecture:x86", "system:android" }
  defines { "ANDROID" }
  targetdir "builds/bin_androidx86"
  debugdir "builds/bin_androidx86"
  links { "m" }

filter { "architecture:ARM", "system:android" }
  defines { "ANDROID" }
  targetdir "builds/bin_androidARM"
  debugdir "builds/bin_androidARM"
  links { "m" }

filter { "system:linux" }
  defines { "LINUX" }
  targetdir "builds/bin_linux"
  debugdir "builds/bin_linux"

filter { "system:macosx" }
  defines { "MACOSX" }
  targetdir "builds/bin_macosx"
  debugdir "builds/bin_macosx"
  removelinks { "SDL2" }
  links { "SDL2.framework" }

filter { "architecture:x64", "system:windows" }
  defines { "WIN32", "_WINDOWS" }
  targetdir "builds/bin_win64"
  debugdir "builds/bin_win64"

filter { "architecture:x64", "system:windowsstore8.1" }
  defines { "WIN32", "_WINDOWS" }
  targetdir "builds/bin_winRT81win64"
  debugdir "builds/bin_winRT81win64"

filter { "configurations:debug", "system:windows*", "system:not windows" }
  ignoredefaultlibraries { "msvcrtd", "vccorlibd" }
  if isWP81 then
    ignoredefaultlibraries { "ole32", "kernel32" }
  end
  links { "vccorlibd", "msvcrtd" }

filter { "configurations:release", "system:windows*", "system:not windows" }
  ignoredefaultlibraries { "msvcrt", "vccorlib" }
  if isWP81 then
    ignoredefaultlibraries { "ole32", "kernel32" }
  end
  links { "vccorlib", "msvcrt" }

filter { "architecture:x86", "system:windowsphone8.0" }
  defines { "WIN32", "_WINDOWS" }
  targetdir "builds/bin_wp80win32"
  debugdir "builds/bin_wp80win32"

filter { "architecture:x86", "system:windowsphone8.1" }
  defines { "WIN32", "_WINDOWS" }
  targetdir "builds/bin_wp81win32"
  debugdir "builds/bin_wp81win32"

filter { "architecture:x86", "system:windowsstore8.1" }
  defines { "WIN32", "_WINDOWS" }
  targetdir "builds/bin_winRT81win32"
  debugdir "builds/bin_winRT81win32"

filter { "architecture:arm", "system:windowsphone8.0" }
  defines { "WINRT", "_WINDOWS" }
  targetdir "builds/bin_wp80arm"
  debugdir "builds/bin_wp80arm"
  removelinks {
    "DelayImp",
    "gdi32",
    "psapi"
  }
  links {
    "d3d11",
    "dxgi",
  }

filter { "architecture:arm", "system:windowsphone8.1" }
  defines { "WINRT", "_WINDOWS" }
  targetdir "builds/bin_wp81arm"
  debugdir "builds/bin_wp81arm"
  removelinks {
    "DelayImp",
    "gdi32",
    "psapi"
  }
  links {
    "d3d11",
    "dxgi",
  }

filter { "architecture:arm", "system:windowsstore8.1" }
  defines { "WINRT", "_WINDOWS" }
  targetdir "builds/bin_winRT81arm"
  debugdir "builds/bin_winRT81arm"
  removelinks {
    "DelayImp",
    "gdi32",
    "psapi"
  }
  links {
    "d3d11",
    "dxgi",
  }

filter {} -- Clear the filters
