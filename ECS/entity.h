#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <type_traits>
#include <limits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cassert>

#include "utility.h"
#include "ECS/system.h"

/*
Overhead of the Entity Component System:
	One Id per Entity and component
	Components are stored in one vector per type
	Ids are stored in one vector per component type
	Putting components and Ids into the same vector would work, not sure if it would be better. Probably depends on component size.
*/

namespace ECS{
	//an Entity can have any type of component added to it
	//note that you cannot add multiple components with the same type, use vector<component> or array<component> to get around that
	struct Entity
	{
		Entity(){
			id = id_counter++;
		}
		//add a component to an Entity
		template<class Component>
		void add(Component &&c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert(*insert_position != id);
			components.insert(begin(components) + (insert_position - begin(ids)), std::forward<Component>(c));
			ids.insert(insert_position, id);
		}
		//emplace a component into an Entity
		template<class Component, class... Args>
		void emplace(Args &&...c){
			auto &ids = System::get_ids<Component>();
			auto &components = System::get_components<Component>();
			auto insert_position = std::lower_bound(begin(ids), end(ids), id);
			assert(*insert_position != id); //disallow multiple components of the same type for the same entity
			components.emplace(begin(components) + (insert_position - begin(ids)), std::forward<Args>(c)...);
			ids.insert(insert_position, id);
		}
		//get the component of a given type or nullptr if the Entity has no such component
		template<class Component>
		Component *get(){
			auto &ids = System::get_ids<Component>();
			auto id_it = lower_bound(begin(ids), end(ids), id);
			if (*id_it != id)
				return nullptr;
			auto pos = id_it - begin(ids);
			auto &components = System::get_components<Component>();
			return &components.at(pos);
		}
		//TODO: Copy constructor (?) move constructor, assignment operator, destructor
		~Entity(){
			assert(!"TODO");
		}

	private:
		static Impl::Id id_counter;
		Impl::Id id;
	};

}

#endif // ENTITY_H
