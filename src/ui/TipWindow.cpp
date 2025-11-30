//
// Created by pc on 29.11.25.
//

#include "TipWindow.h"

namespace game {
    TipWindow::TipWindow(sf::RenderWindow &w)
    : _window(w), _tip_menu_context(nullptr, menu_destroy_context) {
        this->win_return = Exit;
        setup_window_context();
    }

    //windows menu update
    void TipWindow::start() {
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
    //menu
void TipWindow::setup_window_context() {
    _font.loadFromFile("sansation.ttf");
    game_menu::Style style{.TitleFont = &_font,
                           .ItemFont = &_font,
                           .TitleFontSize = 20,
                           .ItemFontSize = 20,
                           .MenuTitleScaleFactor = 1,
                           .MenuItemScaleFactor = 1.5,
                           .colorScheme = {.titleColor = 0xFFFFFF,
                                           .itemColor = 0xFFFFFF,
                                           .selectedColor = 0xFF22F1},
                           .PaddingTitle =
                               {
        .top = 5,
        .left = 100,
    },
.PaddingItems =
    {
        .top = 40,
    },
.TitleAlign = game_menu::Align::Center,
.ItemAlign = game_menu::Align::Center};

    std::vector<game_menu::MenuItem> items{
     {"Continue",
         [&](sf::RenderTarget &target) {
             this->_is_exit_requested = true;
             this->win_return = Continue;
     }},
     {"Exit",
      [&](sf::RenderTarget &target) {
          this->_is_exit_requested = true;
          this->win_return = Exit;
      }}};

    game_menu::MenuConfig config{
        .title = "", .items = items, .style = style};
    _tip_menu_context.reset(create_menu_context(_window, config));
    _current_menu = _tip_menu_context.get();
}
}