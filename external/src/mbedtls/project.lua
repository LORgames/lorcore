project ("mbedtls" .. (projectsuffix or ""))
  dofile "../../../common-proj.lua"
  removeflags "FatalWarnings"
  warnings "Off"

  -- Settings
  kind "StaticLib"
  language "C"

  -- This Project
  files { "library/*", "*.h" }
  files { "project.lua" }

  includedirs { ".." }

  filter { }
