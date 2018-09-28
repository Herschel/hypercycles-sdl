-- premake5.lua

workspace "hypercycles"
   configurations { "debug", "release" }
   platforms { "win64" }

project "hypercycles"
   kind "ConsoleApp"
   language "C"

   targetdir "bin/%{cfg.buildcfg}"

   nuget { "sdl2:2.0.5", "sdl2.redist:2.0.5" }

   files { "src/*.c", "src/*.h" }
   excludes { "src/build1.c" }

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"

   filter "platforms:win64"
      system "Windows"
      architecture "x64"
