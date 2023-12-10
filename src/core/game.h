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

#ifndef SS_GAME_H
#define SS_GAME_H

#include "snake.h"
#include "food.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <list>

#define BOX_SIZE 20 

namespace game {
	/*
	GameController is the central interface of the game.
	This class will monitor the working of game:
	*	Movement of snake
	*	Checking collisions
	*	When to play sound etc
	*/	

	const std::string MenuStrings[5] = {
		"Game Over!",
		"Do you wish to continue?",
		"Yes",
		"No",
		"Exit"
	}; //menu strings

	enum MenuText {
		GameOver,
		Continue,
		Yes,
		No,
		Exit,
	}; //menuText

	enum Fonts {

	};

	/*==================================================*
	*			The main game controller				*
	*===================================================*/
	class GameController {
	public:
		/* Constructors */
		GameController(sf::RenderWindow *w);

		/* Public methods */
		void start();
		/* methods to access the resources */
		sf::Font* getFont( Fonts font ); // no definition yet

	private:
		void gameLoop();
		void setupScene();
		void gameOver(); // no definition yet
		void loadResources(); // no definition yet

		/*==================================================*
		*					Data Members					*
		*===================================================*/
		Snake snake;
		sf::RenderWindow *screen;
		float scale;
		int score ;

		/* Resources that the controller retrieves from Disk */
		sf::Font fontList[3];

	}; // class game
	
	
}

#endif // !SS_GAME_H
