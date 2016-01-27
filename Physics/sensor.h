#ifndef SENSOR_H
#define SENSOR_H

#include "body.h"

namespace Physical{
	//a sensor is essentially the same as a Physical::DynamicBody, except you cannot collide with it
	//instead a sensor will "activate" when something is in its range
	template <class Shape>
	struct Sensor : private Physical::DynamicBody<Shape>{
		using Body = Physical::DynamicBody<Shape>;
		Sensor(Shape &&shape, const Transformator &transformator)
			:Body(std::move(shape), transformator)
		{}
		using Body::operator +=;
		using Body::get_current_transformator;
		using Body::end_frame;
		using Body::get_shape;
		using Body::move;
	};
}

#endif // SENSOR_H
