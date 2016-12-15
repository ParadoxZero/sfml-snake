# SFML-snake 
![Language (C++)](https://img.shields.io/badge/powered_by-C++-brightgreen.svg?style=flat-square)  [![Build Status](https://travis-ci.org/ParadoxZero/sfml-snake.svg?branch=master)](https://travis-ci.org/ParadoxZero/sfml-snake)

> Extremly beginer friendly

A classic snake game made in C++ using sfml library.
It will be a good way to learn about different aspect of development with C++ for a beginer.

If you are a beginer, try to 
  * Contribute and fix the issues that will be posted.
  * Add new issues.
  * Create a CMAKE file

` **Warning: This is untested in linux. Even though it should work as is**`

##Screenshots

![2016-12-14 7](https://cloud.githubusercontent.com/assets/14165258/21160053/7d8ab5bc-c1a9-11e6-922e-b77e09ce3b70.png)
-----
![2016-12-14](https://cloud.githubusercontent.com/assets/14165258/21160036/6fd6ecb0-c1a9-11e6-9f51-fe70c4db79c2.png)


##Requirements

####Must Have (dependencies)
  * SFML 2.4.1 or above - http://www.sfml-dev.org/

####MS Build
 * Visual Studio 15 or above
 * VC++ 15 or above

####Linux
 * g++ 4.8 or above
 * GNU Make
  
##Installing

###First Method (prefered and used)

  * Download the source code.
  * Import into Visual Studio
  * Build according to your need (x64 or x86 or whatever)
   > Don't forget to link the sfml libraries - graphics, window and system.
  * Copy the sfml dlls (window, graphics and system) along with the executable
  * Then RUN
  
###Second Method
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
 * the exectable by name `game` should apear.
 * Copy sfml binaries (dlls) grahpics, window and system to the directory
 * Run and enjoy

 
##Contribution
 
  * Lot's of work remaining.
     * GUI Menu
     * Settings and user data save system.
     * Sound
  * In case you are woundering about the extra complication in moving snake -
      That is done in order to smoothen the animation of movement. To have slower/ variable speed of snake
      in *Higher framerate*

Feel free to fork, fix the issues and contribute. Help is always welcome.
