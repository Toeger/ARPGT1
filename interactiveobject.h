#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

/*
An InteractiveObject is an object in the game that can be collided with or that moves.
An InteractiveObject has a position, a direction, a speed and a weight that influences collision results.
You can attach InteractiveObjects to InteractiveObjects, like having a character hold a sword.
An InteractiveObject encapsulates collision detection and graphics.
*/

#include <vector>
#include <SFML/Graphics.hpp>

#include "physical.h"

class InteractiveObject
{
public:
	//used to specify how attached InteractiveObjects are positioned relative to the attachee
	struct Position{
		Position(float x, float y, float angle) : x(x), y(y), angle(angle){}
		float x, y;
		float angle; //use a vector instead of an angle?
	};
	//attach an object to another
	void attach(InteractiveObject &&io, Position &&position);

	b2Body *body;
	sf::Shape *shape;
	std::vector<std::pair<InteractiveObject, Position>> attached_objects;
};

#endif // INTERACTIVEOBJECT_H
