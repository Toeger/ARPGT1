#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>

namespace Animations
{
	enum Animation{
		fireball,
		size
	};
	void set_texture(sf::Shape &shape, Animation animation, float frame);
}

#endif // ANIMATIONS_H
