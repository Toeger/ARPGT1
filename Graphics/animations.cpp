#include <array>
#include <cmath>

#include "animations.h"

struct AnimationData{
	AnimationData(const char *path, int columns, float frames)
		:columns(columns)
		,frames(frames)
	{
		texture.loadFromFile(path);
		auto texture_size = texture.getSize();
		width = texture_size.x / columns;
		height = texture_size.y;
	}
	sf::Texture texture;
	int width;
	int height;
	int columns;
	float frames;
	sf::IntRect get_rect(float frame)
	{
		sf::IntRect rect;
		int frame_index = static_cast<int>(std::fmod(frame, frames));
		rect.left = frame_index * width;
		rect.top = 0;
		rect.width = width;
		rect.height = height;
		return rect;
	}
};

static std::array<AnimationData, Animations::Animation::size> animations = {
	AnimationData{"Art/feuerballanimation2.png", 17, 17}, //Animations::Animation::fireball
};

void Animations::set_texture(sf::Shape &shape, Animations::Animation animation, float frame)
{
	shape.setTexture(&animations[animation].texture);
	shape.setTextureRect(animations[animation].get_rect(frame));
}
