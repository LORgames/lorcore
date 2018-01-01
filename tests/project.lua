project ("lorcoretests" .. (projectsuffix or ""))
  dofile "../common-proj.lua"
  removeflags "FatalWarnings"

  -- Settings
  kind "ConsoleApp"
  language "C++"

  -- This Project
  files { "src/**.cpp", "src/**.h", "project.lua" }

  includedirs { "src" }

  links { "googletest" .. (projectsuffix or ""), "mbedtls" .. (projectsuffix or "") }
  includedirs { "../external/src/googletest/include" }

  links { "lorcore" .. (projectsuffix or "") }
  includedirs { "../src" }

  filter { }
