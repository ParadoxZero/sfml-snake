

#include "game.h"


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "Snake", sf::Style::Default);
	gameSpace::Game gameController(&window);
	gameController.start();
	return 0;
}