#ifndef SENSOR_H
#define SENSOR_H

#include "body.h"
#include "ECS/system.h"

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
		template <class Transformator, class F> //Transformator can be anything that can be added to an actual Physical::Transformator such as a Physical::Vector and Physical::Direction
		void move(const Transformator &offset, F &&f){
			Body::move(offset, [&](auto &transformer, auto &other_entity){
				return std::forward<F>(f)(ECS::System::component_to_entity_handle(*this), transformer, other_entity);
			});
		}
	};
}

#endif // SENSOR_H
