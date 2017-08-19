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
	using Supported_types = std::tuple<Physical::Circle, Physical::Line>; //TODO: add Physical::Polygon
	static const constexpr std::size_t number_of_supported_types = std::tuple_size<Supported_types>::value;

	namespace { //helper
		//from https://stackoverflow.com/a/18063608/3484570
		//this is used to get the index for a type in a tuple
		//example: Index<int, std::tuple<float, char, int, double>>::value returns 2 at compile time
		//if the type does not exist you get a compilation error
		//if the type exists multiple times you get the first index with a match
		template <class T, class Tuple>
		struct Index;

		template <class T, class... Types>
		struct Index<T, std::tuple<T, Types...>> {
			static const std::size_t value = 0;
		};

		template <class T, class U, class... Types>
		struct Index<T, std::tuple<U, Types...>> {
			static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
		};
	} // namespace
	//A DynamicBody consists of one of the supported shapes with collision detection and a Transformator that translates and rotates the shape
	template <class Shape>
	class DynamicBody {
		static_assert(Index<Shape, Supported_types>::value >= 0, //comparison always true, but we only care about if it compiles, not the actual value
					  "Unsupported type for DynamicBody, add it to Physical::Supported_types to fix");

		public:
		//special member functions
		DynamicBody(Shape &&shape, const Transformator &transformator)
			: current_transformator(transformator)
			, next_transformator(transformator)
			, shape(std::move(shape)) {}
		DynamicBody(Shape &&shape, const Vector &position = {}, const Direction &direction = {})
			: DynamicBody(std::move(shape), Transformator{position, direction}) {}
		DynamicBody(DynamicBody &&) = default;
		DynamicBody(const DynamicBody &) = delete;
		DynamicBody &operator=(const DynamicBody &) = delete;
		DynamicBody &operator=(DynamicBody &&) = default;

		//operators
		DynamicBody &operator+=(const Transformator &transformator) {
			auto new_transformator = next_transformator + transformator;
			if (!colliding<0>(new_transformator)) {
				next_transformator = new_transformator;
			}
			return *this;
		}
		DynamicBody &operator+=(const Vector &vector) {
			return *this += Transformator(vector);
		}
		DynamicBody &operator+=(const Direction &direction) {
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
		//colliding checks if this body is colliding with any other body instanciated with any of the supported types
		//end of recursion
		template <bool compare_other_to_this, class OtherShape>
		std::enable_if_t<compare_other_to_this, ECS::Entity_handle> colliding_helper(const Transformator &new_transformator) {
			using Body = DynamicBody<OtherShape>;
			for (auto r = ECS::System::range<Body>(); r; r.advance()) {
				auto &other = r.template get<Body>();
				if (&other == this) { //can we optimize the branch out somehow? By getting the range from begin to this and this to end?
					continue;
				}
				auto had_collision =
					//collides(other.shape, other.current_transformator, shape, new_transformator) ||
					collides(other.shape, other.next_transformator, shape, new_transformator);
				if (had_collision) {
					return ECS::System::component_to_entity_handle(other);
				}
			}
			return {};
		}
		template <bool compare_other_to_this, class OtherShape>
		std::enable_if_t<!compare_other_to_this, ECS::Entity_handle> colliding_helper(const Transformator &new_transformator) {
			using Body = DynamicBody<OtherShape>;
			for (auto r = ECS::System::range<Body>(); r; r.advance()) {
				auto &other = r.template get<Body>();
				auto had_collision =
					//collides(other.get_shape(), other.get_current_transformator(), shape, new_transformator) ||
					collides(other.get_shape(), other.get_next_transformator(), shape, new_transformator);
				if (had_collision) {
					return ECS::System::component_to_entity_handle(other);
				}
			}
			return {};
		}

		//end of recursion
		template <std::size_t type_index>
		std::enable_if_t<type_index == number_of_supported_types, ECS::Entity_handle> colliding(const Transformator &t) {
			if (Map::current_map->collides(shape, t)) {
				return ECS::System::component_to_entity_handle(*Map::current_map);
			}
			return {};
		}
		template <std::size_t type_index>
			std::enable_if_t < type_index<number_of_supported_types, ECS::Entity_handle> colliding(const Transformator &new_transformator) {
			//wish I had static_if which would allow me to remove colliding_helper
			auto eh = colliding_helper<std::is_same<std::tuple_element_t<type_index, Supported_types>, Shape>::value,
									   std::tuple_element_t<type_index, Supported_types>>(new_transformator);
			return eh ? eh : colliding<type_index + 1>(new_transformator);
		}
		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		Transformator current_transformator, next_transformator;
		Shape shape;
	};
} // namespace Physical

#endif // BODY_H
