#include "textures.h"
#include "Utility/asserts.h"

sf::Texture Textures::textures[Textures::size];

void Textures::fill_textures()
{
	auto success = Textures::textures[fireball].loadFromFile("Art/fireball.png");
	assert_fast(success);
	success = Textures::textures[player].loadFromFile("Art/Dragon.png");
	assert_fast(success);
}
