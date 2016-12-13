




#ifndef SS_GAME_H
#define SS_GAME_H

#include <SFML\Graphics.hpp>
#include <random>
#include <list>

#define SNAKE_MAX_LENGTH 2000
#define BOX_SIZE 20 

namespace game {
	

	class Random {
		std::random_device rd;
		std::mt19937 gen;
	public:

		Random() {
			gen.seed(rd());
		}

		int getRandomInt(int l, int u) {
			std::uniform_int_distribution<int> dist(l,u);
			return dist(gen);
		}

	};

	

	class game {
	public:
		game(sf::RenderWindow *w);

		void start();

	private:
		void gameLoop();
		void setupScene();
		void getNewFood();
		void gameOver();

		inline void drawSnake();
		bool SnakeCheckKilledSelf();
		bool checkCollision(const sf::Vector2f&, const sf::Vector2f&);
		void drawRectangleAt(sf::Vector2f &, sf::Color);
		void loadResources();
		
		
		Random rand;

		sf::RenderWindow *screen;
		int snake_length;

		float scale;
		std::list<sf::Vector2<int>> snake_direction_list;
		sf::Vector2f snake[SNAKE_MAX_LENGTH];
		sf::Vector2f food;
		int score = 0;
		//resources		
		enum Fonts {

		};
		sf::Font fontList[3];
		sf::Color snake_color;
		sf::Color food_color;
		

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
