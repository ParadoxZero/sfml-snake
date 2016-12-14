#include "snake.h"
#include "game.h"


gameSpace::Snake::Snake(sf::RenderWindow *w)
{
	//Templ
	colorBody = sf::Color::Green;
	colorHead = sf::Color::Yellow;
	movementScale = 5;
	screen = w;

	snake_length = 1;
	int x = rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4);
	int y = rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4);
	for (int i = 0; i < snake_length; ++i) {
		body[i].x = x;
		body[i].y = y;
		snake_direction_list.push_front(sf::Vector2<int>(-1, 0));
	}
}

void gameSpace::Snake::drawSnake()
{
	for (int i = 1; i < snake_length; ++i) {
		drawRectangleAt(screen, body[i], colorBody);
	}
	drawRectangleAt(screen, body[0], colorHead);
}

bool gameSpace::Snake::died()
{
	for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
		if (checkCollision(body[0], body[i]))
			return true;

	}
	// hitting walls check
	return (body[0].x < 0 || body[0].y < 0 || body[0].x > screen->getSize().x || body[0].y > screen->getSize().y);
}

bool gameSpace::Snake::ateFood(Food *fd)
{
	if (checkCollision(body[0], fd->getFoodLocation())) {
		snake_length++;
		body[snake_length - 1] = body[snake_length - 2] - movementScale * sf::Vector2f(lastDirection.x, lastDirection.y);
		snake_direction_list.push_back(lastDirection);
		return true;

	}
	return false; 
}

void gameSpace::Snake::moveSnake(sf::Vector2<int> direction)
{
	snake_direction_list.push_front(direction);
	lastDirection = snake_direction_list.back();
	snake_direction_list.pop_back();
	std::list<sf::Vector2<int>>::iterator i = snake_direction_list.begin();
	int index = 0;
	while (i != snake_direction_list.end() && index < snake_length) {
		body[index].x += movementScale * (*i).x;
		body[index].y += movementScale * (*i).y;
		index++;
		i++;
	}
}

sf::Vector2f gameSpace::Snake::getNextFoodLocation()
{
	bool okay = true;
	while (okay) {
		int x = rand.getRandomInt(0, screen->getSize().x - BOX_SIZE);
		int y = rand.getRandomInt(0, screen->getSize().y - BOX_SIZE);
		sf::Vector2f food_loc(x, y);
		okay = true;
		for (int i = 0; i < snake_length; ++i) {
			if (body[i] == food_loc) {
				okay = false;
			}
		}
		if (okay) return food_loc;
	} // while(okay)
}
