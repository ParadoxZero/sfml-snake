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

#ifndef SS_UI_MAIN_MENU_H_
#define SS_UI_MAIN_MENU_H_

#include <SFML/Graphics.hpp>
#include <game_menu/game_menu.h>
#include <memory>

namespace game {

class MainMenu {
public:
  MainMenu(sf::RenderWindow &w);
  void start();

private:
  void setup_menu_context();

  sf::RenderWindow &_window;
  sf::Font _font;
  game_menu::MENU *_current_menu;
  std::unique_ptr<game_menu::MENU, std::function<void(game_menu::MENU *)>>
      _main_menu_context;
  bool _is_exit_requested = false;
};
} // namespace game

#endif // !SS_UI_MAIN_MENU_H_
