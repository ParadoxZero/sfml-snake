

#ifndef SS_HEADER_FOOD_H_
#define SS_HEADER_FOOD_H_

#include "engine.h"

#include <SFML/Graphics.hpp>
#include <random>

namespace gameSpace {

	class Food {
	public:
		Food(sf::RenderWindow *, sf::Vector2f loc);
		sf::Vector2f getFoodLocation();
		void drawFood();
	private:
		sf::Color color;
		sf::Vector2f location;

		sf::RenderWindow *screen;
	};

}

#endif // !SS_HEADER_FOOD_H_
