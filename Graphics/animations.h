#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>

namespace Animations
{
	namespace Direction{
		enum Direction{
		up,
		down,
		left,
		right,
		};
	}

	struct Animation_data{
		Animation_data(int columns, float animation_length_seconds, const char *path);
		sf::IntRect get_rect(float frame);

		sf::Texture texture;
		int width;
		int height;
		int columns;
		int rows;
		float animation_length_seconds;
	};

	enum Animation{
		fireball,
		zombie,
		turtle,
		turtleexplode,
		size
	};
	void set_texture(sf::Sprite &sprite, sf::Vector2u &sprite_size, Animation animation, float frame);
	const Animation_data &get_animation_data(Animation animation);
}

#endif // ANIMATIONS_H
