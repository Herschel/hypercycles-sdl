# hypercycles-sdl 
![Hypercycles Title Screen](https://www.mobygames.com/images/shots/l/697354-hypercycles-dos-screenshot-the-game-s-title-screen.png)<br/>
![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/9a798t07lc9sl5be?svg=true)
![Travis Build Status](https://travis-ci.org/Herschel/hypercycles-sdl.svg?branch=master)

A port of the 1995 DOS game [Hypercycles](https://www.mobygames.com/game/hypercycles) to modern platforms.

Hypercycles was created by [Bob Hays](http://bobhays.com), and the source was generously released under GPLv3 in 2017.

## Status
The project compiles on all platforms, but the game is not yet fully playable.

## Building
hypercycles-sdl uses the [SDL](https://www.libsdl.org/) and [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/) libraries. [premake](https://premake.github.io/) is used to generate project files for all platforms.

### Windows
 * Download [premake5](https://github.com/premake/premake-core/releases/download/v5.0.0-alpha12/premake-5.0.0-alpha12-windows.zip) for Windows  and extract it in the project folder.
 * Run `premake5 vs2017` to generate a Visual Studio project file.
 * Build `hypercycles.sln` with Visual Studio.

### macOS
 * Install [Homebrew](https://brew.sh).
 * Run `brew install sdl2 sdl2_mixer`
 * Download [premake5](https://github.com/premake/premake-core/releases/download/v5.0.0-alpha12/premake-5.0.0-alpha12-macosx.tar.gz) and extract it in the project folder.
 * Run `premake5 gmake2` to generate a makefile.
 * Run `make` to build the game.
 * Alternatively, use `premake5 xcode4` to generate an Xcode project.

### Linux
 * Run `sudo apt-get install libsdl2-dev libsdl2-mixer-dev` (or use the appropiate package manager for your distro).
 * Download [premake5](https://github.com/premake/premake-core/releases/download/v5.0.0-alpha12/premake-5.0.0-alpha12-linux.tar.gz) and extract it in the project folder.
 * Run `premake5 gmake2` to generate a makefile.
 * Run `make` to build the game.
