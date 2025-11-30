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
  std::unique_ptr<Food> food;

  // game loop
void GameController::start() {
  loadResources();
  // TODO
  gameLoop();
}
 void GameController::reset() {
  //food location init
  std::unique_ptr<Food> food =
    std::make_unique<Food>(screen, snake.getNextFoodLocation());
  direction.x = -1;
  direction.y = 0;
  score = 0;
  snake.snake_reset();
}

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
  std::unique_ptr<Food> food =
    std::make_unique<Food>(screen, snake.getNextFoodLocation());
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

} // namespace game