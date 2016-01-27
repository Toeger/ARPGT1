#ifndef BODY_H
#define BODY_H

#include <vector>
#include <limits>
#include <array>
#include <cassert>
#include <tuple>
#include <typeinfo>

#include "utility.h"
#include "Physics/physics_utility.h"
#include "Physics/shapes.h"
#include "ECS/entity.h"
#include "aabb.h"
#include "collision.h"

namespace ECS{
	struct Entity_handle;
}

namespace Physical{
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
	}
	//A DynamicBody consists of one of the supported shapes with collision detection and a Transformator that translates and rotates the shape
	template <class Shape>
	class DynamicBody
	{
		static_assert(Index<Shape, Supported_types>::value >= 0, //comparison always true, but we only care about if it compiles, not the actual value
					  "Unsupported type for DynamicBody, add it to Physical::Supported_types to fix");
	public:
		//special member functions
		DynamicBody(Shape &&shape, const Transformator &transformator)
			:current_transformator(transformator)
			,next_transformator(transformator)
			,shape(std::move(shape))
		{}
		DynamicBody(Shape &&shape, const Vector &position = {}, const Direction &direction = {})
			:DynamicBody(std::move(shape), Transformator{position, direction})
		{}
		DynamicBody(DynamicBody &&) = default;
		DynamicBody(const DynamicBody &) = delete;
		DynamicBody &operator =(const DynamicBody &) = delete;
		DynamicBody &operator =(DynamicBody &&) = default;

		//operators
		DynamicBody &operator +=(const Vector &vector){
			auto new_transformator = next_transformator + vector;
			if (!colliding<0>(new_transformator)){
				next_transformator = new_transformator;
			}
			return *this;
		}
		DynamicBody &operator +=(const Direction &direction){
			auto new_transformator = next_transformator + direction;
			if (!colliding<0>(new_transformator)){
				next_transformator = new_transformator;
			}
			return *this;
		}
//		template <class Transformator, class F> //Transformator can be anything that can be added to an actual Physical::Transformator such as a Physical::Vector and Physical::Direction
//		void move(const Transformator &offset, F &&f){
//			auto new_transformator = next_transformator + offset;
//			bool is_colliding = colliding<0>(new_transformator);
//			if (is_colliding)
//				next_transformator = std::forward<f>(Utility::make_const(current_transformator), next_transformator, entity_handle);
//			return *this;
//		}

		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		const Transformator &get_current_transformator() const{
			return current_transformator;
		}
		const Transformator &get_next_transformator() const{
			return next_transformator;
		}

		AABB get_aabb() const{
			return AABB(shape, current_transformator);
		}

		const Shape &get_shape() const{
			return shape;
		}
		//only supposed to be called by Physical::end_frame()
		void end_frame(){
			current_transformator = next_transformator;
		}

	private:
		//colliding checks if this body is colliding with any other body instanciated with any of the supported types
		//end of recursion
		template <bool compare_other_to_this, class OtherShape>
		std::enable_if_t<compare_other_to_this, ECS::Entity_handle>
		colliding_helper(const Transformator &new_transformator){
			using Body = DynamicBody<OtherShape>;
			for (auto r = ECS::System::range<Body>(); r; r.advance()){
				auto &other = r.template get<Body>();
				if (&other == this) //can we optimize the branch out somehow? By getting the range from begin to this and this to end?
					continue;
				if (collides(other.shape, other.current_transformator, shape, new_transformator))
					return ECS::System::component_to_entity_handle(r);
			}
			return {};
		}
		template <bool compare_other_to_this, class OtherShape>
		std::enable_if_t<!compare_other_to_this, ECS::Entity_handle>
		colliding_helper(const Transformator &new_transformator){
			using Body = DynamicBody<OtherShape>;
			for (auto r = ECS::System::range<Body>(); r; r.advance()){
				auto &other = r.template get<Body>();
				auto colliding_entity = collides(other.get_shape(), other.get_current_transformator(), shape, new_transformator);
				if (colliding_entity)
					return colliding_entity;
			}
			return {};
		}

		//end of recursion
		template <size_t type_index>
		std::enable_if_t<type_index == number_of_supported_types, ECS::Entity_handle>
		colliding(const Transformator &){
			return {};
		}
		template <size_t type_index>
		std::enable_if_t<type_index < number_of_supported_types, bool>
		colliding(const Transformator &new_transformator){
			//wish I had static_if which would allow me to remove colliding_helper
			return colliding_helper<
					std::is_same<std::tuple_element_t<type_index, Supported_types>, Shape>::value,
					std::tuple_element_t<type_index, Supported_types>
			>(new_transformator) || colliding<type_index + 1>(new_transformator);
		}
		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		Transformator current_transformator, next_transformator;
		Shape shape;
	};
	namespace {
		template <size_t type_index, class Function>
		std::enable_if_t<type_index == number_of_supported_types>
		apply_to_physical_bodies_impl(Function &&){/*end of recursion*/}

		template <size_t type_index, class Function>
		std::enable_if_t<type_index < number_of_supported_types>
		apply_to_physical_bodies_impl(Function &&f){
			using Shape_type = std::tuple_element_t<type_index, Supported_types>;
			using Body_type = DynamicBody<Shape_type>;
			for (auto sit = ECS::System::range<Body_type>(); sit; sit.advance()){
				f(sit.template get<Body_type>());
			}
			apply_to_physical_bodies_impl<type_index + 1>(std::forward<Function>(f));
		}
	}
	template <class Function>
	void apply_to_physical_bodies(Function &&f){
		apply_to_physical_bodies_impl<0>(std::forward<Function>(f));
	}
	inline void end_frame(){
		apply_to_physical_bodies([](auto &body){
			body.end_frame();
		});
	}
}

#endif // BODY_H
