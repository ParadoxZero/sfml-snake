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

#include "MainMenu.h"

#include "core/game.h"

namespace game {

MainMenu::MainMenu(sf::RenderWindow &w)
    : _window(w), _main_menu_context(nullptr, menu_destroy_context) {
  setup_menu_context();
}

void MainMenu::start() {
  _window.setFramerateLimit(60);
  while (_window.isOpen()) {
    if (_is_exit_requested) {
      _window.close();
      break;
    }
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        _is_exit_requested = true;
        break;
      }
      menu_handle_event(_current_menu, event);
    }
    _window.clear();
    menu_render(_current_menu);
    _window.display();
  }
}

void MainMenu::setup_menu_context() {
  _font.loadFromFile("sansation.ttf");
  game_menu::Style style{.TitleFont = &_font,
                         .ItemFont = &_font,
                         .TitleFontSize = 36,
                         .ItemFontSize = 24,
                         .MenuTitleScaleFactor = 1,
                         .MenuItemScaleFactor = 1.5,
                         .colorScheme = {.titleColor = 0xFFFFFF,
                                         .itemColor = 0xFFFFFF,
                                         .selectedColor = 0xFF22F1},
                         .PaddingTitle =
                             {
                                 .top = 100,
                                 .left = 0,
                             },
                         .PaddingItems =
                             {
                                 .top = 40,
                             },
                         .TitleAlign = game_menu::Align::Center,
                         .ItemAlign = game_menu::Align::Center};

  std::vector<game_menu::MenuItem> items{
      {"New Game",
       [&](sf::RenderTarget &target) {
         game::GameController gameController(&_window);
         gameController.start();
       }},
      {"Leaderboard", [](sf::RenderTarget &target) {}},
      {"Settings", [](sf::RenderTarget &target) {}},
      {"Exit",
       [&](sf::RenderTarget &target) { this->_is_exit_requested = true; }}};

  game_menu::MenuConfig config{
      .title = "Snake", .items = items, .style = style};
  _main_menu_context.reset(create_menu_context(_window, config));
  _current_menu = _main_menu_context.get();
}

} // namespace game
