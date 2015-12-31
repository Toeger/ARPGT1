#ifndef BODY_H
#define BODY_H

#include <vector>
#include <limits>
#include <array>
#include <cassert>
#include <tuple>

#include "utility.h"
#include "Physics/physics_utility.h"
#include "Physics/circle.h"
#include "entity.h"

namespace Physical{
	//add types here to allow attaching them to a body
	//all attachable types must be listed here
	using Supported_types = std::tuple<Physical::Circle>;
	static const constexpr std::size_t number_of_supported_types = std::tuple_size<Supported_types>::value;

	namespace Helper{
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

		//this essentially declares a "template<class T> std::vector<T> my_vectors;", but that is not allowed in C++14
		//instead we end up with this clusterfuck:
		template <class ...Types>
		struct VectorHolder;
		template <>
		struct VectorHolder<>{};
		template <class T, class ...Rest>
		struct VectorHolder<T, Rest...>{
			template <class U>
			std::enable_if_t<std::is_same<T, U>::value, std::vector<std::pair<U, Transformator>> &>
			get(){
				return data;
			}
			template <class U>
			std::enable_if_t<!std::is_same<T, U>::value, std::vector<std::pair<U, Transformator>> &>
			get(){
				return rest.template get<U>();
			}

			std::vector<std::pair<T, Transformator>> data;
			struct VectorHolder<Rest...> rest;
		};

		template <class>
		struct VectorHolderFromTuple;
		template <class ...Types>
		struct VectorHolderFromTuple<std::tuple<Types...>>{
			using type = VectorHolder<Types...>;
		};
	}

	//A physical body consists of basic physical objects such as circles and polygons
	//you create a physical body by attaching multiple such primitives to it by specifying the offset vector and direction vector
	class Body
	{
	public:
		Transformator &current_transformator(){
			return current_is_1 ? transformator1 : transformator2;
		}
		Transformator &next_transformator(){
			return current_is_1 ? transformator2 : transformator1;
		}
		static void end_frame(){
			for (auto r = System::range<Body>(); r; r.advance()){
				r.get<Body>().next_transformator() = r.get<Body>().current_transformator();
			}
			current_is_1 = !current_is_1;
		}

		Body(const Vector &position, const Direction &direction) :
			transformator1(position, direction),
			transformator2(transformator1)
		{
		}
		Body() : Body({}, {}){}
		Body(const Vector &position) : Body(position, {}){}
		Body(const Direction &direction) : Body({}, direction){}
		Body(Body &&other) :
			transformator1(other.transformator1),
			transformator2(other.transformator2)
		{
			using std::swap;
			swap(attached_objects, other.attached_objects);
		}
		Body &operator =(Body &&other){
			using std::swap;
			swap(attached_objects, other.attached_objects);
			return *this;
		}
		~Body() = default;

		//attach an object to this Body with the given offset and Direction
		template <class T>
		void attach(T &&t, Vector offset, Direction direction){
			static_assert(Helper::Index<T, Supported_types>::value < 1000000, "Requesting unsupported type"); //just need to instantiate the template, comparison doesn't matter
			auto &attached_vector = attached_objects.get<Utility::remove_cvr<T>>();
			attached_vector.emplace_back(std::make_pair<Utility::remove_cvr<T>, Transformator>(std::forward<T>(t), Transformator{offset, direction}));
		}

		template<class Function, class T>
		void apply(Function &&f){
			for (auto &ao : attached_objects.get<Utility::remove_cvr<T>>()){
				f(ao.first, current_transformator() + ao.second);
			}
		}
		template <class Function, class T1, class T2, class... Rest>
		void apply(Function &&f){
			apply<T1>(f);
			apply<T2, Rest...>(f);
		}
		template <class Function, std::size_t type_number>
		std::enable_if_t<type_number == number_of_supported_types>
		apply(Function &&){}

		template <class Function, std::size_t type_number>
		std::enable_if_t<type_number < number_of_supported_types>
		apply(Function &&f){
			apply<decltype((f)), decltype(std::get<type_number>(std::declval<Supported_types>()))>(f);
			apply<decltype((f)), type_number + 1>(f);
		}

		template <class Function>
		void apply(Function &&f){
			apply<decltype((f)), 0>(f);
		}
		//operators
		Body &operator +=(const Vector &vector){
			auto next = current_transformator() + vector;
			if (!colliding()){
				next_transformator() = next;
			}
			return *this;
		}
		Body &operator +=(const Direction &direction){
			auto next = current_transformator() += direction;
			if (!colliding()){
				next_transformator() = next;
			}
			return *this;
		}

	private:
		bool colliding(){
			//TODO:
			/*
			for (auto r = System::range<Body>(); r; r.advance()){
				if (collides(r.get<Body>().aabb, this->aabb)){
					if (collides(r.get<Body>(), *this)){
						return true;
					}
				}
			}
			*/
			return false;
		}

		Transformator transformator1, transformator2;
		static bool current_is_1; //true when currently using transformator1, else using transformator2
		//storing attached objects
		Helper::VectorHolderFromTuple<Supported_types>::type attached_objects;
	};
}

#endif // BODY_H
