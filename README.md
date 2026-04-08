# SFML-snake 
![Language (C++)](https://img.shields.io/badge/powered_by-C++-brightgreen.svg?style=flat-square)  [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0) 


A classic snake game made in C++ using sfml library.
It will be a good way to learn about different aspect of development with C++ for a beginer.

If you are a **beginer**, try to 
- Contribute and fix the issues that will be posted.
- Add new issues.
- Create a CMAKE file


## Screenshots
![Screenshot](Screenshots/2016-12-19.png?raw=true "Sample Main Menu")
![Screenshot](Screenshots/2016-12-17.png?raw=true "Sample gameplay")

## Requirements

#### Must Have (dependencies)
- A valid C++ compiler - clang, gcc, msvc that supports c++20
- cmake >= v3.16
  
## Building

The CMAKE is set up to automatically download the dependencies and link against it. It will be done when configuring cmake.

```
git clone https://github.com/ParadoxZero/sfml-snake.git
cd sfml-snake
make build
```

### Manual build steps using cmake

- `git clone https://github.com/ParadoxZero/sfml-snake.git`
- Create a directory for cmake , e.g. - `mkdir build`
- `cd build`
- `cmake .. -G Ninja` or `cmake ..` (to use the default build system)
- `ninja snake` or `make snake` (depending on your selected build system)
 
## Contribution
 
  * Lot's of work remaining.
     * GUI Menu (see https://github.com/ParadoxZero/GameMenu-cpp)
     * Settings and user data save system.
     * Sound
  * In case you are woundering about the extra complication in moving snake -
      That is done in order to smoothen the animation of movement. To have slower/ variable speed of snake
      in *Higher framerate*

Feel free to fork, fix the issues and contribute. Help is always welcome.
----------------------------------------------------------------------------------
* the original version is above, including the git address and some tips.

## new features from lukas
1. Add the game over menu
   * Now when game over, that will not return to the main menu directly.
   * Attention: add the h file define in game_menu.h, otherwise there is a compile problem.
2. Add the RL learning tools, AKA the AI mode.