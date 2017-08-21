#ifndef SENSOR_H
#define SENSOR_H

#include "body.h"
#include "ecs/system.h"

namespace Physical {
	//a sensor is essentially the same as a Physical::DynamicBody, except you cannot collide with it
	//instead a sensor will "activate" when something is in its range
	template <class Shape>
	struct Sensor : private Physical::Dynamic_body<Shape> {
		using Body = Physical::Dynamic_body<Shape>;
		Sensor(Shape &&shape, const Transformator &transformator)
			: Body(std::move(shape), transformator) {}
		using Body::operator+=;
		using Body::end_frame;
		using Body::get_current_transformator;
		using Body::get_shape;
		using Body::move;
		template <class Transformator, //Transformator can be anything that can be added to an actual
									   //Physical::Transformator such as a Physical::Vector and Physical::Direction
				  class F>
		void move(const Transformator &offset, F &&f) {
			Body::move(offset, [&](auto &transformer, auto &other_entity) {
				return std::forward<F>(f)(ECS::System::component_to_entity_handle(*this), transformer, other_entity);
			});
		}
	};

	template <int type_index = 0, class Function>
	void apply_to_physical_bodies(Function &&f) {
		using Shape_type = Supported_types::nth<type_index>;
		using Dynamic_Body_type = Dynamic_body<Shape_type>;
		for (auto &body : ECS::System::get_range<Dynamic_Body_type>()) {
			f(body);
		}
		using Sensor_Body_type = Sensor<Shape_type>;
		for (auto &sensor : ECS::System::get_range<Sensor_Body_type>()) {
			f(sensor);
		}
		if constexpr (type_index + 1 < Supported_types::size) {
			apply_to_physical_bodies<type_index + 1>(std::forward<Function>(f));
		}
	}
	inline void end_frame() {
		apply_to_physical_bodies([](auto &body) { body.end_frame(); });
	}
} // namespace Physical

#endif // SENSOR_H
