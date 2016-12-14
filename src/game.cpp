


#include "game.h"


#include <random>
#include <iostream>
namespace gameSpace{

	// constructor
	Game::Game(sf::RenderWindow * w) : snake(w)
	{
		screen = w;
		
	}



	void Game::start()
	{
		loadResources();
		// TODO 
		gameLoop();
	}


	void Game::gameLoop()
	{
		bool loopInvarient = true;
		sf::Vector2<int> direction(-1,0);
		scale = 5;
		Food *food = new Food(screen,snake.getNextFoodLocation());
		while (loopInvarient) {
			setupScene();
			food->drawFood();
			sf::Event event;
			while (screen->pollEvent(event)) {
				if (event.type == sf::Event::KeyReleased) {
					if (event.key.code == sf::Keyboard::Up) {
						direction.y = -1;
						direction.x = 0;
					} else if (event.key.code == sf::Keyboard::Down) {
						direction.y = 1;
						direction.x = 0;
					}
					else if (event.key.code == sf::Keyboard::Left) {
						direction.x = -1;
						direction.y = 0;
					}
					else if (event.key.code == sf::Keyboard::Right) {
						direction.x = 1;
						direction.y = 0;
					}
				}
				if (event.type == sf::Event::Closed) {
					loopInvarient = false;
				}
			} //event loop
			snake.moveSnake(direction);
			if (snake.died()) {
				//game over
				exit(0);
			}
			if (snake.ateFood(food)) {
				score++;
				delete food;
				food = new Food(screen, snake.getNextFoodLocation());
			}
			screen->display();
			screen->setFramerateLimit(60);
		}
	} //gameLoop()

	void Game::setupScene()
	{
		screen->clear();
		snake.drawSnake();
	}

	bool checkCollision(const sf::Vector2f& a, const sf::Vector2f& b) {
		
		return (((a.x < b.x) && (b.x < (a.x + BOX_SIZE))) || ((b.x < a.x) && (a.x < (b.x + BOX_SIZE)))) &&
			(((a.y < b.y) && (b.y < (a.y + BOX_SIZE))) || ((b.y < a.y) && (a.y < (b.y + BOX_SIZE))));
		
	}

	
	void drawRectangleAt(sf::RenderWindow *screen,sf::Vector2f &location, sf::Color color)
	{
		sf::RectangleShape box;
		box.setSize(sf::Vector2f(BOX_SIZE,BOX_SIZE));
		box.setPosition(location);
		box.setFillColor(color);
		screen->draw(box);

	}
	void Game::loadResources()
	{
		//TODO
		
	}
	sf::Font * Game::getFont(Fonts font)
	{
		return &fontList[font];
	}

}