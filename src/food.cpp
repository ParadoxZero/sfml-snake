#include "food.h"

gameSpace::Food::Food(sf::RenderWindow *w, sf::Vector2f loc)
{
	location = loc;
	screen = w;

	//TODO temp
	color = sf::Color::Red;
}

sf::Vector2f gameSpace::Food::getFoodLocation()
{
	return location;
}



void gameSpace::Food::drawFood()
{
	drawRectangleAt(screen, location, color);

}
