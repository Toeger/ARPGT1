#include "textures.h"

sf::Texture Textures::textures[Textures::size];

void Textures::fill_textures()
{
	Textures::textures[fireball].loadFromFile("Art/fireball.png");
	Textures::textures[player].loadFromFile("Art/Dragon.png");
}
