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

#ifndef SS_HEADER_SNAKE_H_
#define SS_HEADER_SNAKE_H_



#include <SFML/Graphics.hpp>
#include <list>
#include <random>

#include "food.h"


#define SNAKE_MAX_LENGTH 2000

namespace game {

	class Random {
		/* A utility class that will be used to generate random numbers */
		std::random_device rd;
		std::mt19937 gen;
	public:

		Random() {
			gen.seed(rd());
		}

		int getRandomInt(int l, int u) {
			std::uniform_int_distribution<int> dist(l, u);
			return dist(gen);
		}

	};


	class Snake {
		/*
		This class represents the snake.
		It is represented as a Vector of sf::RectangleShape objects.
		*/
	public:
		Snake(sf::RenderWindow *);
		void drawSnake();
		bool died();
		bool ateFood(Food *fd);
		void moveSnake(sf::Vector2<int> direction);

		sf::Vector2f getNextFoodLocation();
		
	private:
		sf::RenderWindow *screen;
		Random rand;

		/* Used to determine whether or not to increment length of snake */
		bool updateLegth;

		/* The rate of movement of snake */
		float movementScale;

		/* Snake parameters */
		int snake_length;
		std::list<sf::Vector2<int> > snake_direction_list;
		sf::Vector2<int> lastDirection;
		std::vector<sf::RectangleShape> body;

		/* Load from options */
		sf::Color colorBody;
		sf::Color colorHead;
	};

}

#endif // !SS_HEADER_SNAKE_H_
