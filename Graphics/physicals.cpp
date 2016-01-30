#include "physicals.h"
#include "ECS/entity.h"
#include "Physics/body.h"
#include "Physics/sensor.h"
#include "Physics/aabb.h"
#include "utility.h"
#include "ECS/common_components.h"
#include "player.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c, const Physical::Transformator &t){
	sf::CircleShape s(c.radius);
	auto pos = t.vector;
	s.setPosition(pos.x - c.radius, -pos.y - c.radius);
	auto r = (const unsigned char *)&c.radius;
	s.setFillColor(sf::Color(r[0], r[1] + 127, r[2]));
	window.draw(s);
}

static void draw_physical(sf::RenderWindow &window, const Physical::Line &l, const Physical::Transformator &t){
	const auto &p1 = t + l.vector;
	const auto &p2 = t.vector;
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.vector.x, -p1.vector.y)),
		sf::Vertex(sf::Vector2f(p2.x, -p2.y)),
	};
	window.draw(line, Utility::element_count(line), sf::Lines);
}

void Graphics::draw_physicals(sf::RenderWindow &window)
{
	auto &p = Player::player;
	const auto &player_pos = p.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
	const auto &top = player_pos + Physical::Vector{0, 10000};
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(player_pos.vector.x, -player_pos.vector.y), sf::Color{255, 0, 0, 127}),
		sf::Vertex(sf::Vector2f(top.vector.x, -top.vector.y), sf::Color{255, 0, 0, 0}),
	};
	window.draw(line, Utility::element_count(line), sf::Lines);

	//TODO: combine copy + pasted for loops
	Physical::apply_to_physical_bodies([&window](auto &body){
		draw_physical(window, body.get_shape(), body.get_current_transformator());
		auto entity = ECS::System::component_to_entity_handle(body);
		auto hp = entity.template get<Common_components::HP>();
		if (hp){
			//TODO: Draw HP bar
		}
	});
//	for (auto sit = ECS::System::range<Physical::Sensor>(); sit; sit.advance()){
//		sit.get<Physical::Sensor>().apply(
//					[&window](auto &physical_object, const Physical::Transformator &t){
//			draw_physical(window, physical_object, t);
//		});
//		const auto &aabb = sit.get<Physical::Sensor>().get_aabb();
//		sf::Vertex line[] =
//		{
//			sf::Vertex(sf::Vector2f(aabb.left, -aabb.top), sf::Color::Yellow),
//			sf::Vertex(sf::Vector2f(aabb.right, -aabb.top), sf::Color::Yellow),
//			sf::Vertex(sf::Vector2f(aabb.right, -aabb.bottom), sf::Color::Yellow),
//			sf::Vertex(sf::Vector2f(aabb.left, -aabb.bottom), sf::Color::Yellow),
//			sf::Vertex(sf::Vector2f(aabb.left, -aabb.top), sf::Color::Yellow),
//		};
//		window.draw(line, Utility::element_count(line), sf::LinesStrip);
//	}
}
