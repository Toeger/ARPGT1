#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>

namespace Animations
{
	struct Animation_data{
		Animation_data(const char *path, int columns, float animation_length_seconds);
		sf::Texture texture;
		int width;
		int height;
		int columns;
		float animation_length_seconds;
		sf::IntRect get_rect(float frame);
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
