//
// Created by pc on 29.11.25.
//

#ifndef TIPWINDOW_H
#define TIPWINDOW_H
#include <SFML/Graphics.hpp>
#include <game_menu/game_menu.h>

namespace  game{

class TipWindow {
public:
    enum win_val {Continue = 0, Exit = 1};
    win_val win_return;
    TipWindow(sf::RenderWindow &w);
    void start();

private:
    void setup_window_context();
    sf::RenderWindow &_window;
    sf::Font _font;
    game_menu::MENU *_current_menu;
    std::unique_ptr<game_menu::MENU, std::function<void(game_menu::MENU *)>>
        _tip_menu_context;
    bool _is_exit_requested = false;
};

}


#endif //TIPWINDOW_H
