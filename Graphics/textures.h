#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>

namespace Textures
{
	enum Texture_ids{
		fireball,
		player,
		zombie,
		size
	};
	extern sf::Texture textures[size];
	void fill_textures();
}

#endif // TEXTURES_H
