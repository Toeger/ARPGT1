#include "physicals.h"
#include "entity.h"
#include "Physics/body.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c){
	sf::CircleShape s(c.radius);
	s.setPosition(c.position.x, c.position.y);
	s.setFillColor(sf::Color::Magenta);
	window.draw(s);
}

void Graphics::draw_physicals(sf::RenderWindow &window)
{
	(void)draw_physical;
	for (auto sit = System::range<Physical::Body>(); sit; sit.advance()){
		sit.get<Physical::Body>().apply(
			[&window](auto &physical_object){
			//draw_physical(window, physical_object);
			(void)physical_object;
		});
	}
}
