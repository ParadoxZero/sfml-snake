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
/*
#include "UI/MainMenu.h"
 
int main() {
	sf::RenderWindow window( sf::VideoMode( 800, 800 ), "Snake", sf::Style::Close );
	game::MainMenu menu;
	menu.start(&window);
	return 0;
}*/

#include <GameMenu.h>
#include <SFML/Graphics.hpp>
#include <memory>

#include "Game-core/game.h"


namespace test {
    class testAction : public gmenu::Action {
    public:
        void start() {
            exit(0);
        }
    };
}

class StartGameAction2 : public gmenu::Action {
public:
    StartGameAction2( sf::RenderWindow * );
    void start();
private:
    sf::RenderWindow *window;
};

StartGameAction2::StartGameAction2( sf::RenderWindow *w ) {
	window = w;
}

void StartGameAction2::start() {
	game::GameController gameController( window );
	gameController.start();
}

int main() {
    sf::RenderWindow w( sf::VideoMode( 800, 600 ), "Sample Title", sf::Style::Close);
    std::vector<gmenu::MenuItem> itemList;
    std::string text[4] = { "Start", "Options","High Score", "Exit" };
    gmenu::MenuItem item;
    sf::Font font;
    font.loadFromFile( "sansation.ttf" );
    gmenu::Style style( font, font );
    style.layout = gmenu::Layout::TitleLeft|gmenu::Layout::ItemLeft;
    style.TitleColor = sf::Color::White;
    style.ItemColor = sf::Color::Cyan;
    style.Selected = sf::Color::Yellow;
    style.PaddingTitle.top = 250;
    style.PaddingItems.top = 30;
    style.PaddingItems.left = -100;
    for ( int i = 0; i < 4; ++i ) {
        item.title = text[i];
        item.action = std::make_shared < StartGameAction2>(&w);
        itemList.push_back( item );
    }
    gmenu::Menu menu( w, "Sample Menu", itemList, style );
    while ( w.isOpen()  ) {
        w.clear();
        sf::Event event;
        while ( w.pollEvent( event ) ) {
            menu.handleEvent( event );
        }
        menu.drawMenu();
        w.display();
    }
    return 0;
}
