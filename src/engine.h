

#ifndef SS_HEADER_ENGINE_H_
#define SS_HEADER_ENGINE_H_

#include <SFML/Graphics.hpp>

namespace gameSpace {

	bool checkCollision(const sf::Vector2f&, const sf::Vector2f&);
	void drawRectangleAt(sf::RenderWindow *, sf::Vector2f &, sf::Color);

}


#endif // !SS_HEADER_ENGINE_H_
