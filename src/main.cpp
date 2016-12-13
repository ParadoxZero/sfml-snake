

#include "../header/game.h"


void main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "Snake", sf::Style::Default);
	game::game gameController(&window);
	gameController.start();
}