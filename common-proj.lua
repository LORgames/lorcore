filter { }

local proj = project()
targetname(string.gsub(proj.name, projectsuffix, ""))
flags { "NoPCH", "NoIncrementalLink", "FatalWarnings" }
cppdialect "C++11"
--exceptionhandling "On"
warnings "Extra"
editandcontinue "Off"

-- Directories
objdir "intermediate/obj"
targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin"
debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin"

filter { "system:ios" }
  defines { "IOS" }
  objdir "intermediate/obj_ios"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_ios"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_ios"
  links { "CoreAudio.framework", "AudioToolbox.framework", "CoreFoundation.framework", "CoreGraphics.framework", "Foundation.framework", "UIKit.framework", "OpenGLES.framework", "QuartzCore.framework" }

filter { "architecture:x86", "system:android" }
  defines { "ANDROID" }
  objdir "intermediate/obj_androidx86"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_androidx86"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_androidx86"
  links { "m" }

filter { "architecture:ARM", "system:android" }
  defines { "ANDROID" }
  objdir "intermediate/obj_androidARM"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_androidARM"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_androidARM"
  links { "m" }

filter { "system:linux" }
  defines { "LINUX" }
  objdir "intermediate/obj_linux"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_linux"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_linux"

filter { "system:macosx" }
  defines { "MACOSX" }
  objdir "intermediate/obj_macosx"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_macosx"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_macosx"
  removelinks { "SDL2" }
  links { "SDL2.framework" }

filter { "architecture:x64", "system:windows" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_win64"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_win64"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_win64"

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
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81win64"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81win64"

filter { "architecture:x86", "system:windowsphone8.0" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_wp80win32"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp80win32"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp80win32"

filter { "architecture:x86", "system:windowsphone8.1" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_wp81win32"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp81win32"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp81win32"

filter { "architecture:x86", "system:windowsstore8.1" }
  defines { "WIN32", "_WINDOWS" }
  objdir "intermediate/obj_winRT81win32"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81win32"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81win32"

filter { "architecture:arm", "system:windowsphone8.0" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_wp80arm"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp80arm"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp80arm"

filter { "architecture:arm", "system:windowsphone8.1" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_wp81arm"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp81arm"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_wp81arm"

filter { "architecture:arm", "system:windowsstore8.1" }
  defines { "WINRT", "_WINDOWS" }
  objdir "intermediate/obj_winRT81arm"
  targetdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81arm"
  debugdir "%{_MAIN_SCRIPT_DIR}/builds/bin_winRT81arm"

filter {} -- Clear the filters
