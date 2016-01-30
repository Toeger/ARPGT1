#include "physicals.h"
#include "ECS/entity.h"
#include "Physics/body.h"
#include "Physics/sensor.h"
#include "Physics/aabb.h"
#include "utility.h"
#include "ECS/common_components.h"
#include "player.h"
#include "textures.h"

static void draw_physical(sf::RenderWindow &window, const Physical::Circle &c, const Physical::Transformator &t, sf::Texture *texture){
	auto radius = c.radius * 2;
	sf::CircleShape s(radius);
	auto pos = t.vector;
	s.setPosition(pos.x, -pos.y);
	s.setOrigin({radius, radius});
	s.setRotation(-t.direction.to_degrees());
	//auto r = (const unsigned char *)&c.radius;
	//s.setFillColor(sf::Color(r[0], r[1] + 127, r[2]));
	s.setTexture(texture);
	window.draw(s);
}

static void draw_physical(sf::RenderWindow &window, const Physical::Line &l, const Physical::Transformator &t, sf::Texture *){
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
		auto entity = ECS::System::component_to_entity_handle(body);
		sf::Texture *texture = nullptr;
		auto texture_index_ptr = entity.template get<Textures::Texture_ids>();
		if (texture_index_ptr){
			texture = &Textures::textures[*texture_index_ptr];
		}
		draw_physical(window, body.get_shape(), body.get_current_transformator(), texture);
//		auto hp = entity.template get<Common_components::HP>();
//		if (hp){
//			//TODO: Draw proper HP bar
//			const auto &pos = entity.template get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
//			sf::RectangleShape r({100, 20});
//			r.setPosition(pos.vector.x - 50, -pos.vector.y - 80);
//			window.draw(r);
//			auto hp_size = hp->hp * 100.f / hp->max_hp;
//			r.setSize({hp_size, 18.f});
//			r.setPosition(r.getPosition().x, r.getPosition().y + 1);
//			r.setFillColor(sf::Color::Green);
//			window.draw(r);
//			r.setPosition(r.getPosition().x + (hp->max_hp - hp_size) * 100.f / hp->max_hp, r.getPosition().y);
//			r.setSize({hp->max_hp - hp_size, 18.f});
//			r.setFillColor(sf::Color::Red);
//			window.draw(r);
//		}
	});
}
