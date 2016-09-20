project "lorcore"
  -- Settings
  kind "StaticLib"
  language "C++"

  --flags { "OmitDefaultLibrary" }

  -- This Project
  if isWinRT then
    kind "StaticLib"
    if isWP80 then
      system "windowsphone8.0"
    elseif isWP81 then
      system "windowsphone8.1"
	  files { "Assets_wp81/**.png" }
      files { "CastleRushClient_wp81.appxmanifest" }
    elseif isWinRT81 then
      system "windowsstore8.1"
      files { "Assets_w81/**.png" }
      files { "CastleRushClient_w81.appxmanifest" }
	  files { "CastleRushClient_TemporaryKey.pfx" }
	  certificatefile "CastleRushClient_TemporaryKey.pfx"
	  certificatethumbprint "A408A41CFBC0CA22B0BFF7F73C930BF475B79187"
    end

    disablewarnings { "4530" } -- Ignore exception handler warnings...

    editandcontinue "Off"
    consumewinrtextension "false"
    generatewinmd "false"

    includedirs { "libraries/src/SDL2/include" }
    files { "Assets/**.png", "Assets/**.wav", "Assets/**.ogg", "Assets/**.txt" }
    files { "libraries/src/SDL2/src/main/winrt/SDL_winrt_main_NonXAML.cpp", "SDL2.dll" }
    defines { "ASSETDIR=\"Assets/\"", "lorLogToFile" }
  elseif isAndroid then
    system "android"
	kind "SharedLib"

	toolset "clang"
	toolchainversion "3.6"
	stl "gnu stl static"

    includedirs { "libraries/src/SDL2/include" }
    files { "libraries/src/SDL2/src/main/android/SDL_android_main.c" }
	defines { "ASSETDIR=\"\"" }
  elseif isIOS then
    system "ios"
    kind "WindowedApp"
  end

  files { "clientsrc/**.cpp", "clientsrc/**.h", "clientsrc/**rc" }
  files { "clientproject.lua" }

  includedirs { "libraries/src" }
  includedirs { "clientsrc" }
  includedirs { "clientsrc/comms" }
  includedirs { "clientsrc/game" }
  includedirs { "clientsrc/game/physics" }
  includedirs { "clientsrc/game/units" }
  includedirs { "clientsrc/profile" }
  includedirs { "clientsrc/gl" }

  -- SDL_mixer
  includedirs { "libraries/src/SDL2_mixer" }
  files { "libraries/src/SDL2_mixer/**.h", "libraries/src/SDL2_mixer/**.c" }
  defines { "OGG_MUSIC" }

  -- Common settings
  defines "SDLRENDER"
  links { "SDL2" }

  -- Configurations
  if isWinRT then
    filter { "files:**SDL_winrt_main_NonXAML.cpp" }
      consumewinrtextension "true"

    filter { "files:**.png" }
      deploy "true"

    filter { "files:SDL2.dll" }
      deploy "true"
  end

  configuration { "Release" }
    defines { "NDEBUG" }
	  flags { "Optimize", "Symbols", "NoFramePointer" }
	--flags { "NoBufferSecurityCheck" }

  filter { "configurations:Debug", "system:not android" }
    defines { "_DEBUG" }
	  flags { "Symbols" }

  filter { "system:windows*", "system:not windows" }
    prebuildcommands { "xcopy /y $(OutDir)SDL2.dll" }

  filter { "architecture:x64", "system:windows" }
    defines { "WIN64" }
	  libdirs { "libraries/win64" }
	  links { "libcurl" }

  filter { "architecture:x64", "system:windowsstore8.1" }
    defines { "WIN64" }
	  libdirs { "libraries/winRT81win64" }

  filter { "architecture:arm" }
    defines { "WINARM" }

  filter { "architecture:arm", "system:windowsphone8.0" }
      libdirs { "libraries/wp80arm" }
  filter { "architecture:arm", "system:windowsphone8.1" }
      libdirs { "libraries/wp81arm" }
  filter { "architecture:arm", "system:windowsstore8.1" }
      libdirs { "libraries/winRT81arm" }

  filter { "architecture:x86", "system:windowsphone8.0" }
      libdirs { "libraries/wp80win32" }
  filter { "architecture:x86", "system:windowsphone8.1" }
      libdirs { "libraries/wp81win32" }
  filter { "architecture:x86", "system:windowsstore8.1" }
      libdirs { "libraries/winRT81win32" }

  filter { "system:linux" }
    libdirs { "libraries/linux" }

  filter { "system:android", "architecture:x86" }
    libdirs { "libs/x86" }
  filter { "system:android", "architecture:ARM" }
    libdirs { "libs/armeabi-v7a" }

  filter { "system:ios" }
    libdirs { "libraries/iOSx86" }

  configuration { "windows", "Release", "vs2013" }
    buildoptions { "/Zo" }

  dofile "common-proj.lua"
