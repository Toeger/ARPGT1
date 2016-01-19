#ifndef SYSTEM_H
#define SYSTEM_H

#include "ecs_impl.h"
#include "utility.h"

#include <limits>
#include <vector>

namespace ECS{
	template<class H, class... T>
	struct System_iterator;

	constexpr Impl::Id max_id = std::numeric_limits<Impl::Id>::max();

	/*
	System keeps the components of all Entitys in a vector per component type and allows to iterate over Entitys with specified components.
	You only use System to iterate, use Entitys to add components.
	Limitations:
		Cannot have multiple components of the same type in one Entity. You can get around that with an array or vector of components
	*/
	namespace System{
		namespace{
			//vector to store the components
			template<class Component>
			static std::vector<Component> components{};
			//vector to store the IDs. ids and components are locked, so components<CTYPE>[x] is the component that belongs to entity ids<CTYPE>[x]
			template<class Component>
			static std::vector<Impl::Id> ids{max_id};
			//TODO: could make components and ids use the same memory since they reallocate at the same time, but this only saves a few memory allocations and is probably not worth it
		}
		template<class Component>
		std::vector<Utility::remove_cvr<Component>> &get_components(){
			return components<Utility::remove_cvr<Component>>;
		}
		template<class Component>
		std::vector<Impl::Id> &get_ids(){
			return ids<Utility::remove_cvr<Component>>;
		}
		//get a range iterator for a list of components, range<Position, Direction> iterates over all Entities with both a Position and a Direction component
		template<class... Components>
		System_iterator<Components...> range(){
			System_iterator<Components...> si;
			si.advance(0);
			return si;
		}
	}
}

#include "systemiterator.h"

#endif // SYSTEM_H
