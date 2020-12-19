#include "MainMenu.h"
#include "SFML/Graphics.hpp"
#include "Game-Core/game.h"
#include <iostream>

void game::MainMenu::start( sf::RenderWindow * w ) {
	gmenu::Menu men( w );  // remeber to change this back to menu after completion
	action[0] = new StartGameAction( w );
	action[3] = new ExitAction( w );
	gmenu::MenuItem items[4];
	for ( int i = 0; i < 4; ++i ) {
		items[i].title = MenuText[i];
		items[i].action = action[0];
	}
	items[3].action = action[3];
/*	menu.setMenuItems( items, 4 );
	menu.setTitle( "Sfml-Snake" );
	menu.createMenu(); */
	if (!font.loadFromFile("sansation.ttf"))
	{
		std::cout << "please give the correct font address" << std::endl;
	}
	
	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("start");
	menu[0].setPosition(sf::Vector2f(800 / 2, 800 / (4 + 1) * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Options");
	menu[1].setPosition(sf::Vector2f(800 / 2, 800 / (4 + 1) * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(800 / 2, 800 / (4 + 1) * 3));

	selectedItemIndex = 0;
}

void game::MainMenu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < 4; i++)
	{
		window.draw(menu[i]);
	}
}

void game::MainMenu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void game::MainMenu::MoveDown()
{
	
	if (selectedItemIndex + 1 < 4)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
		
	}
}
game::StartGameAction::StartGameAction( sf::RenderWindow *w ) {
	window = w;
}

bool game::StartGameAction::start() {
	game::GameController gameController( window );
	gameController.start();
	return true;
}



game::HighScoreAction::HighScoreAction( sf::RenderWindow *w ) {
	window = w;
}

bool game::HighScoreAction::start() {
	return false;
}

game::OptionsAction::OptionsAction( sf::RenderWindow *w ) {
	window = w;
}

bool game::OptionsAction::start() {
	return false;
}

game::ExitAction::ExitAction( sf::RenderWindow *w ) {
	window = w;
}

bool game::ExitAction::start() {
	if ( getConfirmation() )
		return false; // means exit
	return true;
}

bool game::ExitAction::getConfirmation() {
	bool confirm = false;
	gmenu::MenuItem items[2]; //Creating a list of Menu Items for the Dialog
	items[0].title = "Yes";
	items[0].action = new ConfirmationMenuAction( &confirm );
	items[1].title = "No";
	items[1].action = new DeclineMenuAction( &confirm );
	gmenu::Menu dialog( window, "Are you sure?", items, 2 ); //Creating the confirmation dialog
	dialog.createMenu();
	return confirm;
}


