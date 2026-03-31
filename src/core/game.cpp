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

#include "game.h"
#include <array>
#include <iostream>
#include <memory>
#include <random>
namespace game {

// constructor
GameController::GameController(sf::RenderWindow *w) : snake(w) {
  screen = w;
  score = 0;
}
  // snake location
  sf::Vector2<int> direction(-1, 0);
  std::unique_ptr<Food>  food;;
  bool game_over = false;
  bool food_ate = false;
  // game loop
void GameController::start() {
  loadResources();
  // TODO
  gameLoop();
}
 void GameController::reset() {
  //food location init
  food = std::make_unique<Food>(screen, snake.getNextFoodLocation());
  direction.x = -1;
  direction.y = 0;
  score = 0;
  snake.snake_reset();
  game_over = false;
  food_ate = false;
}
// tip window generates
  TipWindow::win_val GameController::tipwindow_generate() {

  TipWindow::win_val rv;
  sf::RenderWindow window(sf::VideoMode(300, 300), "Snake", sf::Style::Close);
  game::TipWindow tip_window(window);

  tip_window.start();
  if (tip_window.win_return == game::TipWindow::Exit) {
    rv = TipWindow::Exit;
  }
  else {
    rv = TipWindow::Continue;
  }
  return rv;
}
  //key board control
void GameController::gamekeyboard_control() {
  sf::Event event;
  //input and location update
  while (screen->pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Up) {
        direction.y = -1;
        direction.x = 0;
      } else if (event.key.code == sf::Keyboard::Down) {
        direction.y = 1;
        direction.x = 0;
      } else if (event.key.code == sf::Keyboard::Left) {
        direction.x = -1;
        direction.y = 0;
      } else if (event.key.code == sf::Keyboard::Right) {
        direction.x = 1;
        direction.y = 0;
      }
    }
    if (event.type == sf::Event::Closed) {
      exit(0);
    }
  } // event loop
}
  //game loop
void GameController::gameLoop() {
  bool loopInvarient = true;

  //food location get
   food = std::make_unique<Food>(screen, snake.getNextFoodLocation());
// gameplay
  while (loopInvarient) {
    //snake draw
    setupScene();
    //food draw
    food->drawFood();
    gamekeyboard_control();
    // snake location update
    snake.moveSnake(direction);
    // game over
    if (snake.died()) {
      //loopInvarient = false;
      game_over = true;

      TipWindow::win_val win_rv = this->tipwindow_generate();
      if (win_rv == TipWindow::Exit) {
        loopInvarient = false;
      }
      else {
        this->reset();
      }
     // this->reset();
    }

    // get socre
    if (snake.ateFood(food.get())) {
      score++;
      food_ate = true;
      food.reset(new Food(screen, snake.getNextFoodLocation()));
    }

    // UI update
    if (snake.died()!= true){
      screen->display();
      screen->setFramerateLimit(60);
    }

  }
} // gameLoop()

void GameController::setupScene() {
  screen->clear();
  snake.drawSnake();
}

bool checkCollision(const sf::RectangleShape &a, const sf::RectangleShape &b) {
  return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

sf::RectangleShape getRectangleAt(sf::Vector2f location, sf::Color color) {
  sf::RectangleShape box;
  box.setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
  box.setPosition(location);
  box.setFillColor(color);
  return box;
}
void GameController::loadResources() {
  // TODO
}
sf::Font *GameController::getFont(Fonts font) { return &fontList[font]; }



/******************AI function*****************/
  float game::GameController::AI_Reward() {
  if (game_over == true) return -10.0f;
  if (food_ate == true) return 10.0f;
  game::game_over = false;
  game::food_ate = false;
  return -0.01f; // 鼓励快速找到食物
}
  // AI move
  void game::GameController::AI_Move_Action(int action) {
  switch(action) {
    case 0:
      direction.y = -1;
      direction.x = 0; break;
    case 1:
      direction.y = 1;
      direction.x = 0;break;
    case 2:
      direction.x = -1;
      direction.y = 0;break;
    case 3:
      direction.x = 1;
      direction.y = 0;break;
  }
}


  // 提取状态向量
  State game::GameController::AI_GetState() {
  State s{};

  auto head_for_ai = snake.Get_Snake_loction();
  auto food_for_ai = food->getFood();

  s[0] = head_for_ai.getPosition().x;
  s[1] = head_for_ai.getPosition().y;
  s[2] = food_for_ai.getPosition().x;
  s[3] = food_for_ai.getPosition().y;

  // 蛇头周围是否有墙或自己
  s[4] = snake.Hit_Warning_AI(Up); // 上
  s[5] = snake.Hit_Warning_AI(Down); // 下
  s[6] = snake.Hit_Warning_AI(Left); // 左
  s[7] = snake.Hit_Warning_AI(Right); // 右
  return s;
}

  // 执行动作并返回 (next_state, reward, done)
  std::tuple<State, float, bool> game::GameController::Ai_Action_Step(int action) {
  AI_Move_Action(action);
  float reward = AI_Reward();
  bool done = game_over;
  return { AI_GetState(), reward, done };
}
  // Headless step: moves the snake, checks collisions/food, returns (state, reward, done).
  // Does not render anything — safe to call in a tight training loop.
  std::tuple<State, float, bool> GameController::AI_HeadlessStep(int action) {
    AI_Move_Action(action);
    snake.moveSnake(direction);

    game_over = snake.died();
    food_ate  = false;

    if (!game_over && snake.ateFood(food.get())) {
      score++;
      food_ate = true;
      food.reset(new Food(screen, snake.getNextFoodLocation()));
    }

    float reward = game_over ? -10.0f : (food_ate ? 10.0f : -0.01f);
    bool  done   = game_over;
    State next   = done ? State{} : AI_GetState();

    return { next, reward, done };
  }

} // namespace game


