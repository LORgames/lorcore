project ("lorcoretests" .. (projectsuffix or ""))
  dofile "../common-proj.lua"
  removeflags "FatalWarnings"

  -- Settings
  kind "ConsoleApp"
  language "C++"

  -- This Project
  files { "src/**.cpp", "src/**.h", "project.lua" }

  includedirs { "../external/src" }
  includedirs { "src" }

  links { "googletest" .. (projectsuffix or ""), "mbedtls" .. (projectsuffix or "") }
  includedirs { "../external/src/googletest/include" }

  links { "lorcore" .. (projectsuffix or "") }
  includedirs { "../src" }

  links { "SDL2" }

  -- Configurations
  filter { "configurations:Release" }
    defines { "NDEBUG" }
    flags { "NoFramePointer" }
    optimize "On"
    symbols "On"

  filter { "configurations:Debug", "system:not android" }
    defines { "_DEBUG" }
    symbols "Full"

  filter { "system:windows*", "system:not windows" }
    disablewarnings { "4127", "4530" }
    consumewinrtextension "false"
    generatewinmd "false"
    defaultlanguage "en-AU"
    defines { "ASSETDIR=\"Assets/\"", "lorLogToFile" } -- Is ASSETDIR required here?

  filter { "architecture:x64", "system:windows" }
    defines { "WIN64" }
    prebuildcommands { "{COPY} ../external/dlls/win64 %{cfg.targetdir}" }
    libdirs { "../external/libs/win64" }

  filter { "architecture:x64", "system:windowsstore8.1" }
    defines { "WIN64" }

  filter { "architecture:arm" }
    defines { "WINARM" }

  filter { "system:linux", "action:vs*" }
    location "."
    warnings "Off"

  filter { "system:linux" }
    linkgroups "On"
    links { "pthread" }

  filter { "system:macosx" }
    removelinks { "SDL2.framework" }

  filter { "system:ios" }
    kind "WindowedApp"
    libdirs { "../external/libs/iOS" }
    files { "Info.plist" }

  filter { "system:android" }
    toolset "clang"
    toolchainversion "3.8"
    stl "gnu stl static"
    defines { "ASSETDIR=\"\"" } -- Is ASSETDIR required here?

  filter { "system:windows", "configurations:Release", "action:vs2013" }
    buildoptions { "/Zo" }

  filter { "options:coverage", "system:linux", "configurations:debug", "toolset:gcc" }
    symbols "On"
    optimize "Off"
    buildoptions { "-fprofile-arcs", "-ftest-coverage" }
    linkoptions { "--coverage" }

  filter { }

