#include "physicals.h"
#include "entity.h"
#include "Physics/body.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c){
	sf::CircleShape s(c.radius);
	auto pos = c.get_translation();
	s.setPosition(pos.x, pos.y);
	s.setFillColor(sf::Color::Magenta);
	window.draw(s);
}

void Graphics::draw_physicals(sf::RenderWindow &window)
{
	for (auto sit = System::range<Physical::Body>(); sit; sit.advance()){
		sit.get<Physical::Body>().apply(
					[&window](auto &physical_object){
			draw_physical(window, physical_object);
		});
	}
}
