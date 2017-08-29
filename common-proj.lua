filter { }

local proj = project()
targetname(string.gsub(proj.name, projectsuffix, ""))
flags { "C++11", "NoPCH", "NoIncrementalLink", "FatalWarnings" }
--exceptionhandling "On"
warnings "Extra"
editandcontinue "Off"

-- Directories
objdir "intermediate/obj"
targetdir "../builds/bin"
debugdir "../builds/bin"

filter { "system:ios" }
  defines { "IOS" }
  objdir "intermediate/obj_ios"
  targetdir "../builds/bin_ios"
  debugdir "../builds/bin_ios"
  links { "CoreAudio.framework", "AudioToolbox.framework", "CoreFoundation.framework", "CoreGraphics.framework", "Foundation.framework", "UIKit.framework", "OpenGLES.framework", "QuartzCore.framework" }

filter { "architecture:x86", "system:android" }
  defines { "ANDROID" }
  objdir "intermediate/obj_androidx86"
  targetdir "../builds/bin_androidx86"
  debugdir "../builds/bin_androidx86"
  links { "m" }

filter { "architecture:ARM", "system:android" }
  defines { "ANDROID" }
  objdir "intermediate/obj_androidARM"
  targetdir "../builds/bin_androidARM"
  debugdir "../builds/bin_androidARM"
  links { "m" }

filter { "system:linux" }
  defines { "LINUX" }
  objdir "intermediate/obj_linux"
  targetdir "../builds/bin_linux"
  debugdir "../builds/bin_linux"

filter { "system:macosx" }
  defines { "MACOSX" }
  objdir "intermediate/obj_macosx"
  targetdir "../builds/bin_macosx"
  debugdir "../builds/bin_macosx"
  removelinks { "SDL2" }
  links { "SDL2.framework" }

filter { "architecture:x64", "system:windows" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_win64"
  targetdir "../builds/bin_win64"
  debugdir "../builds/bin_win64"

filter { "system:windowsphone8.1" }
  ignoredefaultlibraries { "ole32", "kernel32" }

filter { "configurations:debug", "system:windows*", "system:not windows", "kind:not StaticLib" }
  ignoredefaultlibraries { "msvcrtd", "vccorlibd" }
  links { "vccorlibd", "msvcrtd" }

filter { "configurations:release", "system:windows*", "system:not windows", "kind:not StaticLib" }
  ignoredefaultlibraries { "msvcrt", "vccorlib" }
  links { "vccorlib", "msvcrt" }

filter { "architecture:x64", "system:windowsstore8.1" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_winRT81win64"
  targetdir "../builds/bin_winRT81win64"
  debugdir "../builds/bin_winRT81win64"

filter { "architecture:x86", "system:windowsphone8.0" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_wp80win32"
  targetdir "../builds/bin_wp80win32"
  debugdir "../builds/bin_wp80win32"

filter { "architecture:x86", "system:windowsphone8.1" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_wp81win32"
  targetdir "../builds/bin_wp81win32"
  debugdir "../builds/bin_wp81win32"

filter { "architecture:x86", "system:windowsstore8.1" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_winRT81win32"
  targetdir "../builds/bin_winRT81win32"
  debugdir "../builds/bin_winRT81win32"

filter { "architecture:arm", "system:windowsphone8.0" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_wp80arm"
  targetdir "../builds/bin_wp80arm"
  debugdir "../builds/bin_wp80arm"

filter { "architecture:arm", "system:windowsphone8.1" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_wp81arm"
  targetdir "../builds/bin_wp81arm"
  debugdir "../builds/bin_wp81arm"

filter { "architecture:arm", "system:windowsstore8.1" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_winRT81arm"
  targetdir "../builds/bin_winRT81arm"
  debugdir "../builds/bin_winRT81arm"

filter {} -- Clear the filters
