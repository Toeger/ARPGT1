#include "physicals.h"
#include "entity.h"
#include "Physics/body.h"
#include "Physics/aabb.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c, const Physical::Transformator &t){
	sf::CircleShape s(c.radius);
	auto pos = t.vector;
	s.setPosition(pos.x - c.radius, pos.y - c.radius);
	auto r = (const unsigned char *)&c.radius;
	s.setFillColor(sf::Color(r[0], r[1] + 127, r[2]));
	window.draw(s);
}

void Graphics::draw_physicals(sf::RenderWindow &window)
{
	for (auto sit = System::range<Physical::Body>(); sit; sit.advance()){
		sit.get<Physical::Body>().apply(
					[&window](auto &physical_object, const Physical::Transformator &t){
			draw_physical(window, physical_object, t);
		});
		const auto &aabb = sit.get<Physical::Body>().get_aabb();
		sf::RectangleShape r({aabb.right - aabb.left, aabb.top - aabb.bottom});
		r.setOutlineColor(sf::Color::Red);
		r.setOutlineThickness(1);
		r.setPosition(aabb.left, aabb.bottom);
		window.draw(r);
	}
}
