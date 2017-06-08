project ("googletest" .. (projectsuffix or ""))
  dofile "../../../common-proj.lua"
  removeflags "FatalWarnings"
  warnings "Off"

  -- Settings
  kind "StaticLib"
  language "C++"

  -- This Project
  files { "src/**.cc", "src/**.h", "include/**h" }
  files { "project.lua" }

  includedirs { "include", "src", "." }

  filter { }
