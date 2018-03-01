project ("lorcore" .. (projectsuffix or ""))
  dofile "common-proj.lua"

  -- Settings
  kind "StaticLib"
  language "C++"

  -- This Project
  files { "src/**.cpp", "src/**.h", "src/**rc" }
  files { "project.lua", "common-proj.lua" }

  includedirs { "external/src" }
  includedirs { "src" }
  includedirs { "src/comms" }
  includedirs { "src/profile" }
  includedirs { "src/gl" }
  includedirs { "src/ui" }

  -- Common settings
  defines "RENDERER_SDL"
  links { "SDL2" }

  -- Configurations
  filter { "configurations:Release" }
    defines { "NDEBUG" }
    flags { "NoFramePointer" }
    optimize "On"
    symbols "On"
    --flags { "NoBufferSecurityCheck" }

  filter { "configurations:Debug", "system:not android" }
    defines { "_DEBUG" }
    symbols "On"

  filter { "system:windows*", "system:not windows" }
    disablewarnings { "4127", "4530" } -- Ignore conditional expression is constant and exception handler warnings
    consumewinrtextension "false"
    generatewinmd "false"
    defaultlanguage "en-AU"
    defines { "ASSETDIR=\"Assets/\"", "lorLogToFile" } -- Is ASSETDIR required here?

  filter { "architecture:x64", "system:windows" }
    defines { "WIN64" }
    libdirs { "external/libs/win64" }
    --links { "libcurl" } -- Add this back when the lib is in the repo

  filter { "architecture:x64", "system:windowsstore8.1" }
    defines { "WIN64" }
    libdirs { "external/libs/winRT81win64" }

  filter { "architecture:arm" }
    defines { "WINARM" }

  filter { "architecture:arm", "system:windowsphone8.0" }
    libdirs { "external/libs/wp80arm" }

  filter { "architecture:arm", "system:windowsphone8.1" }
    libdirs { "external/libs/wp81arm" }

  filter { "architecture:arm", "system:windowsstore8.1" }
    libdirs { "external/libs/winRT81arm" }

  filter { "architecture:x86", "system:windowsphone8.0" }
    libdirs { "external/libs/wp80win32" }

  filter { "architecture:x86", "system:windowsphone8.1" }
    libdirs { "external/libs/wp81win32" }

  filter { "architecture:x86", "system:windowsstore8.1" }
    libdirs { "external/libs/winRT81win32" }

  filter { "system:linux" }
    libdirs { "external/libs/linux" }

  filter { "system:linux", "action:vs*" }
    location "."
    warnings "Off"
    files { "external/src/**.h" }

  filter { "system:android" }
    toolset "clang"
    toolchainversion "3.8"
    stl "gnu stl static"
    defines { "ASSETDIR=\"\"" } -- Is ASSETDIR required here?

  filter { "system:android", "architecture:x86" }
    libdirs { "external/libs/androidx86" }

  filter { "system:android", "architecture:ARM" }
    libdirs { "external/libs/androidarm" }

  filter { "system:ios" }
    libdirs { "external/libs/iOSx86" }

  filter { "system:windows", "configurations:Release", "action:vs2013" }
    buildoptions { "/Zo" }

  filter { }
