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

#include "food.h"

game::Food::Food(sf::RenderWindow *w, sf::Vector2f loc)
{
	location = loc;
	screen = w;
	color = sf::Color::Red;
	food = getRectangleAt( location, color);
	
}

sf::RectangleShape game::Food::getFood()
{
	return food;
}



void game::Food::drawFood()
{
	screen->draw( food );
}
