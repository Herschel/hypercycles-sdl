-- premake5.lua

workspace "hypercycles"
   configurations { "debug", "release" }
   if os.target() == "windows" then
      platforms { "win64" }
   else
      platforms { "osx" }
   end

   -- Windows SDK version.
   -- Must be set outside filter.
   systemversion "10.0.15063.0"

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"

    filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"

    filter "platforms:win64"
      system "Windows"
      architecture "x64"

    filter "platforms:osx"
      system "macosx"
      libdirs { "/usr/local/lib" }
      includedirs { "/usr/local/include/SDL2" }
      links { "SDL2", "SDL2_mixer" }


project "hypercycles"
  kind "ConsoleApp"
  language "C"

  targetdir "bin/%{cfg.buildcfg}"
  debugdir "assets"

  nuget {
    "sdl2.nuget:2.0.8.2",
    "sdl2.nuget.redist:2.0.8.2",
    "sdl2_mixer.nuget:2.0.2.2",
    "sdl2_mixer.nuget.redist:2.0.2.2"
  }

  files { "src/*.c", "src/*.cpp", "src/*.h" }
  excludes { "src/build1.cpp" }
