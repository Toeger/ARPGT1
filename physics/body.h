#ifndef BODY_H
#define BODY_H

#include "aabb.h"
#include "collision.h"
#include "ecs/entity.h"
#include "ecs/utility.h"
#include "gameplay/map.h"
#include "physics/physics_utility.h"
#include "physics/shapes.h"

#include <array>
#include <cassert>
#include <limits>
#include <tuple>
#include <typeinfo>
#include <vector>

namespace ECS {
	struct Entity_handle;
}

namespace Physical {
	//add shapes here that a Body can be
	using Supported_types = Utility::Type_list<Physical::Circle, Physical::Line>; //TODO: add Physical::Polygon

	//A DynamicBody consists of one of the supported shapes with collision detection and a Transformator that translates and rotates the shape
	template <class Shape>
	class Dynamic_body {
		static_assert(Supported_types::has<Shape>(), "Unsupported type for DynamicBody, add it to Physical::Supported_types to fix");

		public:
		//special member functions
		Dynamic_body(Shape &&shape, const Transformator &transformator)
			: current_transformator(transformator)
			, next_transformator(transformator)
			, shape(std::move(shape)) {}
		Dynamic_body(Shape &&shape, const Vector &position = {}, const Direction &direction = {})
			: Dynamic_body(std::move(shape), Transformator{position, direction}) {}
		Dynamic_body(Dynamic_body &&) = default;
		Dynamic_body(const Dynamic_body &) = delete;
		Dynamic_body &operator=(const Dynamic_body &) = delete;
		Dynamic_body &operator=(Dynamic_body &&) = default;

		//operators
		Dynamic_body &operator+=(const Transformator &transformator) {
			auto new_transformator = next_transformator + transformator;
			if (!colliding<0>(new_transformator)) {
				next_transformator = new_transformator;
			}
			return *this;
		}
		Dynamic_body &operator+=(const Vector &vector) {
			return *this += Transformator(vector);
		}
		Dynamic_body &operator+=(const Direction &direction) {
			return *this += Transformator(direction);
		}
		template <class Transformator>
		void force_move(const Transformator &offset) {
			move(offset, [](const Transformator &t, ECS::Entity_handle) { return t; });
		}
		template <class Transformator, class F>
		void move(const Transformator &offset, F &&f) {
			//Transformator can be anything that can be added to an actual Physical::Transformator such as a Physical::Vector and Physical::Direction
			auto new_transformator = next_transformator + offset;
			auto colliding_entity = colliding<0>(new_transformator);
			if (colliding_entity) {
				next_transformator += std::forward<F>(f)(offset, colliding_entity);
			} else {
				next_transformator = new_transformator;
			}
		}

		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		const Transformator &get_current_transformator() const {
			return current_transformator;
		}
		const Transformator &get_next_transformator() const {
			return next_transformator;
		}

		AABB get_aabb() const {
			return AABB(shape, current_transformator);
		}

		const Shape &get_shape() const {
			return shape;
		}
		//only supposed to be called by Physical::end_frame()
		void end_frame() {
			current_transformator = next_transformator;
		}

		private:
		//get the entity that we collide with if we move to the position given by new_transformator
		template <std::size_t type_index>
		ECS::Entity_handle colliding(const Transformator &new_transformator) {
			using Body = Dynamic_body<Supported_types::nth<type_index>>;
			for (auto &other : ECS::System::get_range<Body>()) {
				if constexpr (std::is_same<Supported_types::nth<type_index>, Shape>::value) {
					if (&other == this) { //can we optimize the branch out somehow? By getting the range from begin to this and this to end?
						continue;
					}
				}
				const auto had_collision =
					//collides(other.get_shape(), other.get_current_transformator(), shape, new_transformator) ||
					collides(other.get_shape(), other.get_next_transformator(), shape, new_transformator);
				if (had_collision) {
					return ECS::System::component_to_entity_handle(other);
				}
			}
			if constexpr (type_index + 1 < Supported_types::size) {
				return colliding<type_index + 1>(new_transformator);
			} else {
				if (Map::current_map->collides(shape, new_transformator)) {
					return ECS::System::component_to_entity_handle(*Map::current_map);
				}
				return {};
			}
		}
		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		Transformator current_transformator, next_transformator;
		Shape shape;
	};
} // namespace Physical

#endif // BODY_H
