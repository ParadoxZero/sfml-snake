# SFML-snake 
![Language (C++)](https://img.shields.io/badge/powered_by-C++-brightgreen.svg?style=flat-square)  [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)  [![Build Status](https://travis-ci.org/ParadoxZero/sfml-snake.svg?branch=master)](https://travis-ci.org/ParadoxZero/sfml-snake)  [![Gitter chat](https://badges.gitter.im/sfml-snake/gitter.png)](https://gitter.im/sfml-snake)


A classic snake game made in C++ using sfml library.
It will be a good way to learn about different aspect of development with C++ for a beginer.

```
 Currently the build system available are:
 * MS Build (Visual Studio)
 * GNU Make
```
If you are a beginer, try to 
  * Contribute and fix the issues that will be posted.
  * Add new issues.
  * Create a CMAKE file


##Screenshots

![Screenshot](Screenshots/2016-12-19.png?raw=true "Sample Main Menu")

![Screenshot](Screenshots/2016-12-17.png?raw=true "Sample Main Menu")

![Screenshot](Screenshots/2016-12-17%20(1).png?raw=true "Sample Main Menu")

##Requirements

####Must Have (dependencies)
  * SFML 2.4.1 or above - http://www.sfml-dev.org/
   #####Linux
    
    ```
     sudo apt-get install libsfml-dev
    ```
    
    #####Windows
     * Vist www.sfml-dev.org and download the lastest version of the library.
     * In project settings,
      * Add the `</path/to/sfml/>/include` to your **c++ include path**.
      * Then add `</path/to/sfml/>/lib` to the **linker extra library path**.
      
   * GameMenu library : 
     * Link : https://github.com/ParadoxZero/GameMenu-cpp

####MS Build
 * Visual Studio 15 or above
 * VC++ 15 or above

####Linux
 * g++ 4.8 or above
 * GNU Make
  
##Building from source

###First Method (prefered and used) MS Build

  * Download the source code.
  * Import into Visual Studio
  * Build according to your need (x64 or x86 or whatever)
   
    ```
    Don't forget to link the sfml libraries - graphics, window and system.
    put the following files in linker input:
     sfml-system-d.lib, sfml-window-d.lib, sfml-graphics-d.lib and GameMenu.lib
    ```
  * Copy the sfml dlls (window, graphics and system) along with the executable
  * Then Run and enjoy
  
###Second Method Makefile
(Build will work, but the application is untested)

 * Download source code
 * cd to directory
  eg
  
  ```
   $ cd sfml-snake
  ```
 * Run make
 
  ```
   $ make
  ```
 * the exectable `bin/game` should apear. The directory will be created if doesn't exist.
 * Run and enjoy

 
##Contribution
 
  * Lot's of work remaining.
     * GUI Menu (see https://github.com/ParadoxZero/GameMenu-cpp)
     * Settings and user data save system.
     * Sound
  * In case you are woundering about the extra complication in moving snake -
      That is done in order to smoothen the animation of movement. To have slower/ variable speed of snake
      in *Higher framerate*

Feel free to fork, fix the issues and contribute. Help is always welcome.
