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
#include "entity.h"
#include "aabb.h"
#include "collision.h"

namespace Physical{
	//add types here to allow attaching them to a body
	//all attachable types must be listed here
	using Supported_types = std::tuple<Physical::Circle, Physical::Line>;
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

			template <class U>
			std::enable_if_t<std::is_same<T, U>::value, const std::vector<std::pair<U, Transformator>> &>
			get() const{
				return data;
			}
			template <class U>
			std::enable_if_t<!std::is_same<T, U>::value, const std::vector<std::pair<U, Transformator>> &>
			get() const{
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
		const Transformator &current_transformator() const{
			return current_is_1 ? transformator1 : transformator2;
		}
		const Transformator &next_transformator() const{
			return current_is_1 ? transformator2 : transformator1;
		}
		static void end_frame(){
			for (auto r = System::range<Body>(); r; r.advance()){
				auto &body = r.get<Body>();
				body.current_transformator() = body.next_transformator();
				body.update_aabb();
			}
			current_is_1 = !current_is_1;
		}

		Body(const Vector &position, const Direction &direction) :
			transformator1(position, direction),
			transformator2(transformator1)
		{
		}
		Body() = default;
		Body(const Vector &position) : Body(position, {}){}
		Body(const Direction &direction) : Body({}, direction){}
		Body(Body &&other) = default;
		Body &operator =(Body &&other) = default;
		~Body() = default;

		//attach an object to this Body with the given offset and Direction
		template <class T>
		void attach(T &&t, Vector offset, Direction direction){
			static_assert(Helper::Index<T, Supported_types>::value < 1000000, "Requesting unsupported type"); //just need to instantiate the template, comparison doesn't matter
			auto &attached_vector = attached_objects.get<Utility::remove_cvr<T>>();
			Transformator transformator{offset, direction};
			aabb.combine(AABB(t, transformator));
			attached_vector.emplace_back(std::make_pair<Utility::remove_cvr<T>, Transformator>(std::forward<T>(t), std::move(transformator)));
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
		apply(Function &&)
		{}
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

		template<class Function, class T>
		void apply(Function &&f) const{
			for (auto &ao : attached_objects.get<Utility::remove_cvr<T>>()){
				f(ao.first, current_transformator() + ao.second);
			}
		}
		template <class Function, class T1, class T2, class... Rest>
		void apply(Function &&f) const{
			apply<T1>(f);
			apply<T2, Rest...>(f);
		}
		template <class Function, std::size_t type_number>
		std::enable_if_t<type_number == number_of_supported_types>
		apply(Function &&) const
		{}
		template <class Function, std::size_t type_number>
		std::enable_if_t<type_number < number_of_supported_types>
		apply(Function &&f) const{
			apply<decltype((f)), decltype(std::get<type_number>(std::declval<Supported_types>()))>(f);
			apply<decltype((f)), type_number + 1>(f);
		}
		template <class Function>
		void apply(Function &&f) const{
			apply<decltype((f)), 0>(f);
		}

		//operators
		Body &operator +=(const Vector &vector){
			auto new_transformator = next_transformator() + vector;
			if (!colliding(new_transformator)){
				next_transformator() = new_transformator;
			}
			return *this;
		}
		Body &operator +=(const Direction &direction){
			auto new_transformator = next_transformator() + direction;
			if (!colliding(new_transformator)){
				next_transformator() = new_transformator;
			}
			return *this;
		}
		const AABB &get_aabb() const{
			return aabb;
		}
	private:
		bool colliding(const Transformator &new_transformator){
			auto new_aabb = get_aabb(new_transformator);
			for (auto r = System::range<Body>(); r; r.advance()){
				auto &other = r.get<Body>();
				if (&other == this)
					continue;
				if (Physical::collides(other.aabb, new_aabb)){
					if (collides(other, *this, new_transformator - current_transformator())){
						return true;
					}
				}
			}
			return false;
		}
		AABB get_aabb(const Transformator &t) const{
			AABB aabb;
			apply([&aabb, &t](auto &type, const Transformator &){
				aabb.combine(AABB(type, t));
			});
			return aabb;
		}
		void update_aabb(){
			aabb.clear();
			apply([this](auto &type, const Transformator &t){
				aabb.combine(AABB(type, t));
			});
		}

		Transformator transformator1, transformator2;
		static bool current_is_1; //true when currently using transformator1, else using transformator2
		//storing attached objects
		Helper::VectorHolderFromTuple<Supported_types>::type attached_objects;
		AABB aabb;
		static bool collides(const Body &b1, const Body &b2){
			bool collided = false;
			b1.apply([&collided, &b2](const auto &obj1, const Transformator &t1){
				if (!collided){
					b2.apply([&collided, &obj1, &t1](const auto obj2, const Transformator &t2){
						if (!collided){
							collided = Physical::collides(obj1, t1, obj2, t2);
						}
					});
				}
			});
			return collided;
		}
		static bool collides(const Body &b1, const Body &b2, const Transformator &offset){
			//checks collision, except that the transformator for b2 has the given offset from b2.current_transformator()
			bool collided = false;
			b1.apply([&collided, &b2, &offset](const auto &obj1, const Transformator &t1){
				if (!collided){
					b2.apply([&collided, &obj1, &t1, &offset](const auto obj2, const Transformator &t2){
						if (!collided){
							collided = Physical::collides(obj1, t1, obj2, t2 + offset);
						}
					});
				}
			});
			return collided;
		}
	};
}

#endif // BODY_H
