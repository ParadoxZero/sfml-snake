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


#ifndef SS_HEADER_FOOD_H_
#define SS_HEADER_FOOD_H_

#include "engine.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
namespace game {
	/*
	Food: represents the Food object snake eats.
	It is represented by a sf::RectangleShape
	*/
	
	class Food {
	public:
		Food(sf::RenderWindow *, sf::Vector2f loc);
		sf::RectangleShape getFood();
		void drawFood();
		~Food() {
			std::cout << "Food\n";
		}
	private:
		sf::Color color;
		sf::Vector2f location;
		sf::RectangleShape food;
		sf::RenderWindow *screen;
	};

}

#endif // !SS_HEADER_FOOD_H_
