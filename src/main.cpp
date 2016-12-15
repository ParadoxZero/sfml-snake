

#include "game.h"


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "Snake", sf::Style::Close);
	gameSpace::Game gameController(&window);
	gameController.start();
	return 0;
}