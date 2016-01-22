#ifndef SENSOR_H
#define SENSOR_H

#include "body.h"

namespace Physical{
	//a sensor is essentially the same as a Body, except you cannot collide with it
	//instead a sensor will "activate" when a Body is in its range
//	struct Sensor : private Body{
//		Sensor(const Transformator &transformator)
//			:Body(transformator)
//		{}
//		using Body::attach;
//		using Body::operator +=;
//		template <class Function>
//		void apply(Function &&f){
//			Body::apply(f);
//		}
//		const Physical::AABB &get_aabb() const{
//			return Body::get_aabb();
//		}
//	};
}

#endif // SENSOR_H
