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

#include "snake.h"
#include "game.h"


game::Snake::Snake(sf::RenderWindow *w)
{
	//Templ
	colorBody = sf::Color::Green;
	colorHead = sf::Color::Yellow;
	movementScale = 5;
	screen = w;

	snake_length = 1;
	int x = rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4);
	int y = rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4);
	{
		body.push_back( getRectangleAt( sf::Vector2f( x, y ), colorHead));
		snake_direction_list.push_front(sf::Vector2<int>(-1, 0));
	}
	updateLegth = false;
}

void game::Snake::drawSnake()
{
	for (int i = 1; i < snake_length; ++i) {
		screen->draw( body[i]);
	}
	screen->draw( body[0] );
}

bool game::Snake::died()
{
	for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i) {
		if (checkCollision(body[0], body[i]))
			return true;

	}
	// hitting walls check
	int x = body[0].getGlobalBounds().left, y = body[0].getGlobalBounds().top;
	int mx = screen->getSize().x, my = screen->getSize().y;
	return (x > mx || x < 0) || (y > my || y < 0);
}

bool game::Snake::ateFood(Food *fd)
{
	if ( updateLegth ) {
		snake_length++;
		updateLegth = false;
	}
	if (checkCollision(body[0], fd->getFood())) {
		updateLegth = true;
		sf::Vector2f new_location = body[snake_length - 1].getPosition();
		body.push_back( getRectangleAt( new_location, colorBody) );
		snake_direction_list.push_back(lastDirection);
		return true;
	}
	return false; 
}

void game::Snake::moveSnake(sf::Vector2<int> direction)
{
	snake_direction_list.push_front(direction);
	lastDirection = snake_direction_list.back();
	snake_direction_list.pop_back();
	std::list<sf::Vector2<int>>::iterator i = snake_direction_list.begin();
	int index = 0;
	while (i != snake_direction_list.end() && index < snake_length) {
		body[index].move( movementScale*(*i).x, movementScale*(*i).y );
		index++;
		i++;
	}
}

sf::Vector2f game::Snake::getNextFoodLocation()
{
	bool okay = true;
	while (okay) {
		/* This loop exists to make sure the new food does not spawn inside the snake */
		int x = rand.getRandomInt(0, screen->getSize().x - 4 * BOX_SIZE);
		int y = rand.getRandomInt(0, screen->getSize().y - 4 * BOX_SIZE);
		sf::Vector2f food_loc(x, y);
		okay = true;
		for (int i = 0; i < snake_length; ++i) {
			if (body[i].getGlobalBounds().contains(food_loc)) {
				okay = false;
			}
		}
		if (okay) return food_loc;
	} // while(okay)
}
