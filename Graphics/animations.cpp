#include <array>
#include <cmath>

#include "animations.h"
#include "Utility/asserts.h"

struct AnimationData{
	AnimationData(const char *path, int columns, float animation_length_seconds)
		:columns(columns)
		,animation_length_seconds(animation_length_seconds)
	{
		auto success = texture.loadFromFile(path);
		assert_fast(success);
		auto texture_size = texture.getSize();
		width = texture_size.x / columns;
		height = texture_size.y;
	}
	sf::Texture texture;
	int width;
	int height;
	int columns;
	float animation_length_seconds;
	sf::IntRect get_rect(float frame)
	{
		sf::IntRect rect;
		int frame_index = static_cast<int>(std::fmod(frame / animation_length_seconds, 1) * columns);
		rect.left = frame_index * width;
		rect.top = 0;
		rect.width = width;
		rect.height = height;
		return rect;
	}
};

static std::array<AnimationData, Animations::Animation::size> animations = {
	AnimationData{"Art/feuerballanimation3.png", 9, 2}, //Animations::Animation::fireball
	AnimationData{"Art/zombie.png", 16, 1}, //Animations::Animation::zombie
};

void Animations::set_texture(sf::Sprite &sprite, sf::Vector2u &sprite_size, Animations::Animation animation, float frame)
{
	sprite.setTexture(animations[animation].texture);
	const auto &tex_rect = animations[animation].get_rect(frame);
	sprite_size.x = tex_rect.width;
	sprite_size.y = tex_rect.height;
	sprite.setTextureRect(tex_rect);
}
