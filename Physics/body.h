#ifndef BODY_H
#define BODY_H

#include <vector>
#include "utility.h"
#include "Physics/physics_utility.h"
#include <limits>
#include <array>

namespace Physical{
	//A physical body consists of basic physical objects such as circles and polygons
	//you create a physical body by attaching multiple such primitives to it by specifying the offset vector and direction vector
	class Body
	{
	public:
		Body(){
			std::fill(begin(attached_objects), end(attached_objects), invalid);
		}

		//attach an object to this Body with the given offset and Direction
		template <class T>
		void attach(T &&t, Vector offset, Direction direction){
			get_attached<Utility::remove_cvr<T>>().emplace_back(std::forward<T>(t), std::move(offset), std::move(direction));
			auto pos = std::find_if_not(begin(attached_objects), end(attached_objects), invalid);
			if (pos == end(attached_objects))
				throw std::length_error("Trying to attach more than maximum allowed number of objects");
			*pos = get_attached<Utility::remove_cvr<T>>().size() * number_of_types + type_id<Utility::remove_cvr<T>>;
		}
		//iterate over all objects of this Body and call given function
		template<class Function>
		void apply(Function &&f){
			//TODO
			//f();
			(void)f;
		}

		~Body();
	private:
		static const int max_attached_objects = 8; //maximum number of objects you can attach to a body
		static const std::size_t number_of_types = 4; //the number of types supported by Body
		static const std::size_t invalid = std::numeric_limits<std::size_t>::max();
		template <class T>
		static const std::size_t type_id;
		//type_id<Circle> = 0;
		//type_id<Square> = 1;
		template <class Attached_object>
		struct Attached{
			Vector offset;
			Direction direction;
			Attached_object ao;
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
