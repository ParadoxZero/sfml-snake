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




#ifndef SS_HEADER_ENGINE_H_
#define SS_HEADER_ENGINE_H_

#include <SFML/Graphics.hpp>

namespace game {
	/*
	Contains functions that will be used to perform certain
	actions that will be needed that will be needed by game

	*/

	bool checkCollision(const sf::RectangleShape&, const sf::RectangleShape& );

	/* Returns a rectangle at the specified coordinates */
	sf::RectangleShape getRectangleAt(sf::Vector2f location, sf::Color);

}


#endif // !SS_HEADER_ENGINE_H_
