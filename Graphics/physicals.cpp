#include "physicals.h"
#include "entity.h"
#include "Physics/body.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c, const Physical::Transformator &t){
	sf::CircleShape s(c.radius);
	auto pos = t.get_translation();
	//s.setRotation(t.get_rotation().to_angle()); //don't need to set the rotation when drawing a circle because it doesn't do anything
	s.setPosition(pos.x - c.radius, pos.y - c.radius);
	s.setFillColor(sf::Color::Magenta);
	window.draw(s);
}

void Graphics::draw_physicals(sf::RenderWindow &window)
{
	for (auto sit = System::range<Physical::Body>(); sit; sit.advance()){
		sit.get<Physical::Body>().apply(
					[&window](auto &physical_object, const Physical::Transformator &t){
			draw_physical(window, physical_object, t);
		});
	}
}
