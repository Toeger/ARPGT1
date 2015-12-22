#ifndef BODY_H
#define BODY_H

#include <vector>
#include <limits>
#include <array>

#include "utility.h"
#include "Physics/physics_utility.h"
#include "Physics/circle.h"

namespace Physical{
	template <std::size_t id>
	struct Type_holder;
	template <>
	struct Type_holder<0>{
		using type = Physical::Circle;
	};
	template <std::size_t id>
	using Type_holder_t = typename Type_holder<id>::type;
	//A physical body consists of basic physical objects such as circles and polygons
	//you create a physical body by attaching multiple such primitives to it by specifying the offset vector and direction vector
	class Body
	{
	public:
		Vector position;
		Direction direction;
		Body(){
			std::fill(begin(attached_objects), end(attached_objects), invalid);
		}

		//attach an object to this Body with the given offset and Direction
		template <class T>
		void attach(T &&t, Vector offset, Direction direction){
			get_attached<Utility::remove_cvr<T>>().emplace_back(std::forward<T>(t), std::move(offset), std::move(direction));
			auto pos = std::find_if(begin(attached_objects), end(attached_objects), [](auto &id){
				return id == invalid;
			});
			if (pos == end(attached_objects))
				throw std::length_error("Trying to attach more than maximum allowed number of objects");
			*pos = get_attached<Utility::remove_cvr<T>>().size() * number_of_types + type_id<Utility::remove_cvr<T>>;
		}
		//iterate over all objects of this Body and call given function
		template<class Function>
		void apply(Function &&f){
			for (auto id : attached_objects){
				if (id == invalid)
					continue;
				auto type = id % number_of_types;
				auto index = id / number_of_types;
				switch (type){
				case 0:
					f(get_attached<Type_holder_t<0>>()[index]);
				}
			}
			//TODO
			//f();
			(void)f;
		}

		~Body(){}
	private:
		static const int max_attached_objects = 8; //maximum number of objects you can attach to a body
		static const std::size_t number_of_types = 4; //the number of types supported by Body
		static constexpr const std::size_t invalid = std::numeric_limits<std::size_t>::max();
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

		template<class Attached_object>
		static std::vector<Attached<Attached_object>> &get_attached(){
			static std::vector<Attached<Attached_object>> attached;
			return attached;
		}

		std::array<std::size_t, max_attached_objects> attached_objects; //the attached objects
	};
}

#endif // BODY_H
