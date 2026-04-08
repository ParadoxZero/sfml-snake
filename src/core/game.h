/*
 *	Copyright (C) 2016 Sidhin S Thomas
 *
 *	This file is part of sfml-snake.
 *
 *    sfml-snake is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   sfml-snake is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with sfml-snake.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SS_GAME_H
#define SS_GAME_H

#include "food.h"
#include "snake.h"
#include "ui/TipWindow.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <list>
#include <random>

#define BOX_SIZE 20

namespace game {
/*
GameController is the central interface of the game.
This class will monitor the working of game:
*	Movement of snake
*	Checking collisions
*	When to play sound etc
*/

const std::string MenuStrings[5] = {"Game Over!", "Do you wish to continue?",
                                    "Yes", "No", "Exit"}; // menu strings

enum MenuText {
  GameOver,
  Continue,
  Yes,
  No,
  Exit,
}; // menuText

enum Fonts {

};
  // define AI array
  using State = std::array<int, 8>;
  // 状态向量，自己根据需要调整
  static constexpr int ACTION_COUNT = 4; // 上下左右

/*==================================================*
 *			The main game controller *
 *===================================================*/
class GameController {


public:
  /* Constructors */
  GameController(sf::RenderWindow *w);

  /* Public methods */
  void start();
  void reset();
  std::tuple<State, float, bool> Ai_Action_Step(int action);
  // Headless step for AI training: advances game state without rendering
  std::tuple<State, float, bool> AI_HeadlessStep(int action);
  // Rendered game loop driven by an AI policy (for watching the trained agent)
  void AI_GameLoop(std::function<int(State)> policy);
  /* methods to access the resources */
  sf::Font *getFont(Fonts font); // no definition yet
  bool loopInvarient;

  //AI function
  float AI_Reward();
  State AI_GetState() ;
  void AI_Move_Action(int action);
private:
  void gameLoop();
  void setupScene();
  void gamekeyboard_control();
  void gameOver();      // no definition yet
  void loadResources(); // no definition yet
  TipWindow::win_val tipwindow_generate();
  Snake snake;
  sf::RenderWindow *screen;
  float scale;
  int score;
  bool game_over;
  bool food_ate;

  /* Resources that the controller retrieves from Disk */
  sf::Font fontList[3];

}; // class game

} // namespace game

#endif // !SS_GAME_H
