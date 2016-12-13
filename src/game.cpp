


#include "../header/game.h"


#include <random>
#include <iostream>
namespace game{

	// constructor
	game::game(sf::RenderWindow * w)
	{
		screen = w;
		snake_length = 1 ;
		int x = rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4);
		int y = rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4);
		for (int i = 0; i < snake_length; ++i) {
			snake[i].x = x;
			snake[i].y = y;
			snake_direction_list.push_front(sf::Vector2<int>(-1,0));
		}
	}



	void game::start()
	{
		loadResources();
		// TODO 
		gameLoop();
	}


	void game::gameLoop()
	{
		bool loopInvarient = true;
		sf::Vector2<int> direction(-1,0);
		scale = 5;
		getNewFood();
		sf::Vector2<int> temp = direction;
		while (loopInvarient) {
			setupScene();
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
			snake_direction_list.push_front(direction);
			temp = snake_direction_list.back();
			  // move snake
			std::list<sf::Vector2<int>>::iterator i = snake_direction_list.begin();
			int index = 0;
			while (i != snake_direction_list.end() && index < snake_length) {
				snake[index].x = snake[index].x + scale * (*i).x;
				snake[index].y = snake[index].y + scale * (*i).y;
				index++;
				i++;
			}
			snake_direction_list.pop_back();
			if (SnakeCheckKilledSelf()) {
				//game over
				exit(0);
			}
			if (checkCollision(snake[0], food)) {
				snake_length++;
				snake_direction_list.push_back(temp);
				snake[snake_length-1]= snake[snake_length-2] - scale * sf::Vector2f (temp.x,temp.y);
				getNewFood();
			}
			screen->display();
			screen->setFramerateLimit(60);
		}
	} //gameLoop()

	void game::setupScene()
	{
		screen->clear();
		drawSnake();
		drawRectangleAt(food, food_color);
	}

	void game::getNewFood()
	{
		bool okay = true;
		while (okay) {
			int x = rand.getRandomInt(0, screen->getSize().x -BOX_SIZE);
			int y = rand.getRandomInt(0, screen->getSize().y - BOX_SIZE);
			sf::Vector2f food_loc(x, y);
			okay = true;
			for (int i = 0; i < snake_length; ++i) {
				if (snake[i] == food_loc) {
					okay = false;
				}
			}
			if (okay) food = food_loc;
		} // while(okay)

	}

	bool game::checkCollision(const sf::Vector2f& a, const sf::Vector2f& b) {
		
		return (((a.x < b.x) && (b.x < (a.x + BOX_SIZE))) || ((b.x < a.x) && (a.x < (b.x + BOX_SIZE)))) &&
			(((a.y < b.y) && (b.y < (a.y + BOX_SIZE))) || ((b.y < a.y) && (a.y < (b.y + BOX_SIZE))));
		
	}

	
	void game::drawRectangleAt(sf::Vector2f &location, sf::Color color)
	{
		sf::RectangleShape box;
		box.setSize(sf::Vector2f(BOX_SIZE,BOX_SIZE));
		box.setPosition(location);
		box.setFillColor(color);
		screen->draw(box);

	}

	void game::drawSnake() {
		for (int i = 1; i < snake_length; ++i) {
			drawRectangleAt(snake[i], snake_color);
		}
		drawRectangleAt(snake[0], sf::Color::Cyan);

	}
	bool game::SnakeCheckKilledSelf() {
		for (int i = BOX_SIZE/(scale/10); i < snake_length; ++i) {
			if (checkCollision(snake[0], snake[i]))
				return true;
				
		}
		// hitting walls check
		return (snake[0].x < 0 || snake[0].y < 0 || snake[0].x > screen->getSize().x || snake[0].y > screen->getSize().y);
	}

	void game::loadResources()
	{
		//TODO
		food_color = sf::Color::Red;
		snake_color = sf::Color::Green;
	}
	sf::Font * game::getFont(Fonts font)
	{
		return &fontList[font];
	}

}