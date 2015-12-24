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

namespace Physical{
	//add types here to allow attaching them to a body
	//all attachable types must be listed here
	using supported_types = std::tuple<Physical::Circle>;

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
	}

	static const constexpr std::size_t number_of_supported_types = std::tuple_size<supported_types>::value;
	//This class represents a reference to an attached object and its type number
	struct Attached_Object_Reference{
		bool is_valid() const{
			return id != invalid;
		}
		bool is_invalid() const{
			return id == invalid;
		}
		std::size_t index() const{
			assert(is_valid());
			return id / number_of_supported_types;
		}
		std::size_t type_number() const{
			assert(is_valid());
			return id % number_of_supported_types;
		}
		template <class T>
		void set(std::size_t index){
			id = index * number_of_supported_types + Helper::Index<Utility::remove_cvr<T>, supported_types>::value;
		}
		void set_invalid(){
			id = invalid;
		}

		static void swap(Attached_Object_Reference &lhs, Attached_Object_Reference &rhs){
			std::swap(lhs.id, rhs.id);
		}
	private:
		static constexpr const std::size_t invalid = std::numeric_limits<std::size_t>::max();
		std::size_t id = invalid;
	};

	//A physical body consists of basic physical objects such as circles and polygons
	//you create a physical body by attaching multiple such primitives to it by specifying the offset vector and direction vector
	class Body
	{
	public:
		Vector position;
		Direction direction;
		Body(const Vector &position, const Direction &direction) :
			position(position),
			direction(direction)
		{
		}
		Body() : Body({}, {}){}
		Body(const Vector &position) : Body(position, {}){}
		Body(const Direction &direction) : Body({}, direction){}
		Body(Body &&other) :
			attached_objects(other.attached_objects){
			swap(attached_objects, other.attached_objects);
		}
		Body &operator =(Body &&other){
			swap(attached_objects, other.attached_objects);
			return *this;
		}
		~Body(){
			for (auto &ao : attached_objects){
				if (ao.is_valid())
					remove_attached(ao);
			}
		}

		//attach an object to this Body with the given offset and Direction
		template <class T>
		void attach(T &&t, Vector offset, Direction direction){
			auto &attached_vector = get_attached<Utility::remove_cvr<T>>();
			attached_vector.emplace_back(std::forward<T>(t), std::move(offset), std::move(direction));
			auto pos = std::find_if(begin(attached_objects), end(attached_objects), [](auto &id){
				return id.is_invalid();
			});
			if (pos == end(attached_objects))
				throw std::length_error("Trying to attach more than maximum allowed number of objects");
			pos->template set<T>(attached_vector.size() - 1);
		}

		template<class Function>
		void apply(Function &&f){
			for (auto &ao : attached_objects){
				if (ao.is_valid()){
					private_apply(ao, [fun = std::forward<Function>(f)](const auto &v, std::size_t index){
						fun(v[index].ao);
					});
				}
			}
		}

	private:
		//iterate over all objects of this Body and call given function
		template<class Function>
		void private_apply(Attached_Object_Reference &ao, Function &&f){
			//this switch essentially converts a runtime variable to a compile time variable
			//need to match the cases with the number of supported_types
			//should find a way to auto-generate these cases
			switch (ao.type_number()){
			case 0:
				assert((get_attached<std::tuple_element<0, supported_types>::type>().size() > ao.index()));
				f(get_attached<std::tuple_element<0, supported_types>::type>(), ao.index());
			break;
			default:
				throw std::invalid_argument("Invalid type");
			}
		}

		static const int max_attached_objects = 8; //maximum number of objects you can attach to a body
		template <class T>
		static const std::size_t type_id;
		//type_id<Square> = 1;
		template <class Attached_object>
		struct Attached{
			Attached(const Attached_object &ao, Vector offset, Direction direction) :
				ao(ao),
				offset(offset),
				direction(direction)
			{
			}
			Attached(Attached_object &&ao, Vector offset, Direction direction) :
				ao(std::move(ao)),
				offset(offset),
				direction(direction)
			{
			}

			Attached_object ao;
			Vector offset;
			Direction direction;
		};

		void remove_attached(Attached_Object_Reference &ao){
			private_apply(ao, [](auto &vec, std::size_t index){
				vec.erase(begin(vec) + index);
			});
			ao.set_invalid();
		}

		template<class Attached_object>
		static std::vector<Attached<Attached_object>> &get_attached(){
			static_assert(Helper::Index<Attached_object, supported_types>::value < 1000000, "Requesting unsupported type"); //just need to instantiate the template, comparison doesn't matter
			static std::vector<Attached<Attached_object>> attached;
			return attached;
		}

		//data members
		std::array<Attached_Object_Reference, max_attached_objects> attached_objects;
	};
}

#endif // BODY_H
