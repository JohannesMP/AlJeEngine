-- A solution contains projects, and defines the available configurations
solution "AlJeEngine"
  configurations { "Debug", "Release" }

  -- A project defines one build target
  project "AlJeEngine"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files
    {
      "../core/**.cpp",
      "../lib/**.cpp"
    }
    removefiles
    {
      "../core/systems/Graphics/src/Render.cpp",
      "../lib/math/glm/detail/dummy.cpp"
    }


    includedirs
    {
      "../dependencies/include",
      "../lib/math"
    }

    libdirs
    {
      "../dependencies/lib/osx_64"
    }

    toolset "clang"
    buildoptions {"-std=c++11"}
    -- mac uses frameworks for 
    linkoptions
    {
      "-framework SDL2",
      "-F../dependencies/lib/osx_64",
      "-framework OpenGL",
      "-framework glut",
      "../dependencies/lib/osx_64/libGLEW.a",
      "../dependencies/lib/osx_64/libSDL.a",
      "../dependencies/lib/osx_64/libSDLmain.a"
    }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      optimize "On"

