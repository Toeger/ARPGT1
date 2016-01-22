#ifndef BODY_H
#define BODY_H

#include <vector>
#include <limits>
#include <array>
#include <cassert>
#include <tuple>

#include "utility.h"
#include "Physics/physics_utility.h"
#include "Physics/shapes.h"
#include "ECS/entity.h"
#include "aabb.h"
#include "collision.h"

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
		DynamicBody(const Vector &position = {}, const Direction &direction = {})
			:DynamicBody(Transformator{position, direction})
		{}
		DynamicBody(const Direction &direction)
			:DynamicBody({}, direction)
		{}
		DynamicBody(DynamicBody &&) = default;
		DynamicBody(const DynamicBody &) = delete;
		DynamicBody &operator =(const DynamicBody &other) = delete;
		DynamicBody &operator =(DynamicBody &&other) = default;

		//operators
		DynamicBody &operator +=(const Vector &vector){
			auto new_transformator = next_transformator + vector;
			if (!colliding(new_transformator)){
				next_transformator = new_transformator;
			}
			return *this;
		}
		DynamicBody &operator +=(const Direction &direction){
			auto new_transformator = next_transformator + direction;
			if (!colliding(new_transformator)){
				next_transformator = new_transformator;
			}
			return *this;
		}
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
	private:
		template <size_t type_index = 0>
		bool colliding(const Transformator &new_transformator){
			using Shape_type = std::tuple_element_t<type_index, Supported_types>;
			using Body_type = DynamicBody<Shape_type>;
			for (auto r = ECS::System::range<Body_type>(); r; r.advance()){
				//auto &other = r.get<Body_type>();
				auto &other = r.get();
				if (&other == this) //can we optimize the branch out somehow? //TODO: at least if the type of *this != Body_type we can skip the check
					continue;
				if (collides(other, *this, new_transformator - current_transformator))
					return true;
			}
			return colliding<type_index + 1>();
		}
		//a Body holds 2 transformators: the current one where the object is and the next one where it will be next frame
		Transformator current_transformator, next_transformator;
		Shape shape;
	};
	//end_frame is called after all physics has been resolved and we switch from one logical frame to the next
	namespace {
		template <size_t type_index = 0>
		void end_frame(){

		}
		template <>
		void end_frame<number_of_supported_types>()
		{}
	}
	inline void end_frame(){
//		for (auto r = ECS::System::range<DynamicBody>(); r; r.advance()){
//			auto &body = r.get<DynamicBody>();
//			body.current_transformator = body.next_transformator;
//		}
	}
}

#endif // BODY_H
