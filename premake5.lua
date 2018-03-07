-- Requires
require "vstudio"
dofile "tools/premake/modules/android/_preload.lua"
require "android"
dofile "tools/premake/modules/winrt/_preload.lua"
require "winrt"
require "ios"
require "vslinux"

newoption
{
  trigger = "system",
  description = "System to target",
  allowed =
  {
    { "windowsphone8.0", "Windows Phone 8.0 Apps" },
    { "windowsphone8.1", "Windows Phone 8.1 Apps" },
    { "windowsstore8.1", "Windows Store 8.1 Apps" },
    { "android", "Android Apps" },
    { "ios", "iOS Apps" },
    { "linux", "Linux Apps" },
  }
}

workspace "lorcore"
  if _ACTION == "gmake" then
    configurations { "Release", "Debug" }
  else
    configurations { "Debug", "Release" }
  end

  editorintegration "On"
  projectsuffix = ""
  platforms { "x64" }

  filter { "system:windowsstore8.1" }
    platforms { "x86", "x64", "ARM" }
  filter { "system:windowsphone8.1 or android or ios" }
    removeplatforms { "x64" }
    platforms { "x86", "ARM" }
  filter { }

  startproject "lorcoretests"

  -- Do some magic
  if _OPTIONS.system then
    system(_OPTIONS.system)
  end

  dofile "project.lua"
  dofile "external/src/mbedtls/project.lua"
  dofile "external/src/googletest/project.lua"
  dofile "tests/project.lua"
