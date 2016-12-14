




#ifndef SS_GAME_H
#define SS_GAME_H

#include "snake.h"
#include "food.h"

#include <SFML\Graphics.hpp>
#include <random>
#include <list>

#define BOX_SIZE 20 

namespace gameSpace {
	


	class Game {
	public:
		
		

		
		Game(sf::RenderWindow *w);

		void start();

	private:
		void gameLoop();
		void setupScene();
		void gameOver();

		
		void loadResources();
		
		
		Snake snake;

		sf::RenderWindow *screen;
		float scale;
		int score = 0;
		//resources		
		enum Fonts {

		};
		sf::Font fontList[3];
		
		

		sf::Font* getFont(Fonts font);

		

	}; // class game
	
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


	
}

#endif // !SS_GAME_H
