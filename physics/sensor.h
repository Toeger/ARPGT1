#ifndef SENSOR_H
#define SENSOR_H

#include "body.h"
#include "ecs/system.h"

namespace Physical {
	//a sensor is essentially the same as a Physical::DynamicBody, except you cannot collide with it
	//instead a sensor will "activate" when something is in its range
	template <class Shape>
	struct Sensor : private Physical::DynamicBody<Shape> {
		using Body = Physical::DynamicBody<Shape>;
		Sensor(Shape &&shape, const Transformator &transformator)
			: Body(std::move(shape), transformator) {}
		using Body::operator+=;
		using Body::end_frame;
		using Body::get_current_transformator;
		using Body::get_shape;
		using Body::move;
		template <
			class Transformator,
			class F> //Transformator can be anything that can be added to an actual Physical::Transformator such as a Physical::Vector and Physical::Direction
		void
		move(const Transformator &offset, F &&f) {
			Body::move(offset, [&](auto &transformer, auto &other_entity) {
				return std::forward<F>(f)(ECS::System::component_to_entity_handle(*this), transformer, other_entity);
			});
		}
	};

	namespace {
		template <std::size_t type_index, class Function>
		std::enable_if_t<type_index == number_of_supported_types> apply_to_physical_bodies_impl(Function && /*unused*/) { /*end of recursion*/
		}

		template <std::size_t type_index, class Function>
			std::enable_if_t < type_index<number_of_supported_types> apply_to_physical_bodies_impl(Function &&f) {
			using Shape_type = std::tuple_element_t<type_index, Supported_types>;
			using Dynamic_Body_type = DynamicBody<Shape_type>;
			for (auto &body : ECS::System::get_range<Dynamic_Body_type>()) {
				f(body);
			}
			using Sensor_Body_type = Sensor<Shape_type>;
			for (auto &sensor : ECS::System::get_range<Sensor_Body_type>()) {
				f(sensor);
			}
			apply_to_physical_bodies_impl<type_index + 1>(std::forward<Function>(f));
		}
	} // namespace
	template <class Function>
	void apply_to_physical_bodies(Function &&f) {
		apply_to_physical_bodies_impl<0>(std::forward<Function>(f));
	}
	inline void end_frame() {
		apply_to_physical_bodies([](auto &body) { body.end_frame(); });
	}
} // namespace Physical

#endif // SENSOR_H
