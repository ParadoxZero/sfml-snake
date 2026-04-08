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

#include "snake.h"
#include "game.h"
//#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX13.1.sdk/System/Library/Frameworks/AppKit.framework/Versions/C/Headers/NSGraphics.h"

game::Snake::Snake(sf::RenderWindow *w) {
  // Templ
  colorBody = sf::Color::White;
  colorHead = sf::Color::Blue;
  movementScale = 5;
  screen = w;

  snake_length = 1;
  int x =
      rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4);
  int y =
      rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4);
  {
    body.push_back(getRectangleAt(sf::Vector2f(x, y), colorHead));
    snake_direction_list.push_front(sf::Vector2<int>(-1, 0));
  }
  updateLegth = false;
}

void game::Snake::drawSnake() {
  for (int i = 0; i < snake_length; ++i) {
    screen->draw(body[i]);
  }
  screen->draw(body[0]);
}


sf::RectangleShape game::Snake:: Get_Snake_loction() {
  return body[0];
}
bool game::Snake::died() {
  int rv;

  for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
    if (checkCollision(body[0], body[i]))
      return true;
  }
  // hitting walls chec
  if (body.size() == 0) {
    rv = false;
  }
  else {
    int x = body[0].getGlobalBounds().left, y = body[0].getGlobalBounds().top;
    int mx = screen->getSize().x, my = screen->getSize().y;
    rv = (x > mx || x < 0) || (y > my || y < 0);

  }
   return rv;
}
// ai get the envirnoment around snake head
bool game::Snake::Hit_Warning_AI(game::direction_base dir) {

  bool rv = false;
  //head location
  int snake_x = body[0].getPosition().x, snake_y = body[0].getPosition().y;
  //wall size
  int wall_x = screen->getSize().x, wall_y = screen->getSize().y;
  switch (dir) {
    case game::Up:
      for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
        if (snake_y < (body[i].getPosition().y - 10))
          rv = true;
      }
      if ((snake_y - 10 < 0)) {
        rv = true;
      }
      break;
    case game::Down:
      for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
        if (snake_y > (body[i].getPosition().y + 10))
          rv = true;
      }
      if ((snake_y + 10 > wall_y)) {
        rv = true;
      }
      break;
    case game::Left:
      for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
        if (snake_x < (body[i].getPosition().x - 10))
          rv = true;
      }
      if ((snake_x - 10  < 0)) {
        rv = true;
      }
      break;
      break;
    case game::Right:
      for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
        if (snake_x > (body[i].getPosition().x + 10))
          rv = true;
      }
      if ((snake_x + 10 > wall_x)) {
        rv = true;
      }
      break;
      default:
      break;

  }
  return rv;
}
bool game::Snake::ateFood(Food *fd) {
  if (updateLegth) {
    snake_length++;
    updateLegth = false;
  }
  if (checkCollision(body[0], fd->getFood())) {
    updateLegth = true;
    sf::Vector2f new_location = body[snake_length - 1].getPosition();
    body.push_back(getRectangleAt(new_location, colorBody));
    snake_direction_list.push_back(lastDirection);
    return true;
  }
  return false;
}

void game::Snake::moveSnake(sf::Vector2<int> direction) {
  snake_direction_list.push_front(direction);
  lastDirection = snake_direction_list.back();
  snake_direction_list.pop_back();
  std::list<sf::Vector2<int>>::iterator i = snake_direction_list.begin();
  int index = 0;
  while (i != snake_direction_list.end() && index < snake_length) {
    body[index].move(movementScale * (*i).x, movementScale * (*i).y);
    index++;
    i++;
  }
}

//food generate
sf::Vector2f game::Snake::getNextFoodLocation() {
  bool okay = true;
  int x = rand.getRandomInt(0, screen->getSize().x - 4 * BOX_SIZE);
  int y = rand.getRandomInt(0, screen->getSize().y - 4 * BOX_SIZE);
  sf::Vector2f food_loc(x, y);
  while (okay) {
    /* This loop exists to make sure the new food does not spawn inside the
     * snake */
    okay = true;
    for (int i = 0; i < snake_length; ++i) {
      if (body[i].getGlobalBounds().contains(food_loc)) {
        okay = false;
      }
    }
    if (okay)
      return food_loc;
  } // while(okay)
  return food_loc;
}

//snake len init
void game::Snake::snake_reset() {
  snake_length = 1;
  body.resize(1);
  int x =
      rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4);
  int y =
      rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4);
    body[0].setPosition(x,y);
    snake_direction_list.push_front(sf::Vector2<int>(-1, 0));
}